#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#ifndef BSWABE_DEBUG
#define NDEBUG
#endif
//#define PBC_DEBUG

#include <assert.h>

#include <openssl/sha.h>
#include <glib.h>
#include <pbc.h>

#include "bswabe.h"
#include "private.h"

#define TYPE_A_PARAMS \
		"type a\n" \
		"q 87807107996633125224377819847540498158068831994142082" \
		"1102865339926647563088022295707862517942266222142315585" \
		"8769582317459277713367317481324925129998224791\n" \
		"h 12016012264891146079388821366740534204802954401251311" \
		"822919615131047207289359704531102844802183906537786776\n" \
		"r 730750818665451621361119245571504901405976559617\n" \
		"exp2 159\n" \
		"exp1 107\n" \
		"sign1 1\n" \
		"sign0 1\n"

#define TYPE_D159_PARAMS \
		"type d\n" \
		"q 625852803282871856053922297323874661378036491717\n" \
		"n 625852803282871856053923088432465995634661283063\n" \
		"h 3\n" \
		"r 208617601094290618684641029477488665211553761021\n" \
		"a 581595782028432961150765424293919699975513269268\n" \
		"b 517921465817243828776542439081147840953753552322\n" \
		"k 6\n" \
		"nk 60094290356408407130984161127310078516360031868417968262992864809623507269833854678414046779817844853757026858774966331434198257512457993293271849043664655146443229029069463392046837830267994222789160047337432075266619082657640364986415435746294498140589844832666082434658532589211525696\n" \
		"hk 1380801711862212484403205699005242141541629761433899149236405232528956996854655261075303661691995273080620762287276051361446528504633283152278831183711301329765591450680250000592437612973269056\n" \
		"coeff0 472731500571015189154958232321864199355792223347\n" \
		"coeff1 352243926696145937581894994871017455453604730246\n" \
		"coeff2 289113341693870057212775990719504267185772707305\n" \
		"nqr 431211441436589568382088865288592347194866189652\n"

int leaf_count2 = 0;
char last_error[256];

char*
bswabe_error()
{
	return last_error;
}

void
raise_error(char* fmt, ...)
{
	va_list args;

#ifdef BSWABE_DEBUG
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
#else
	va_start(args, fmt);
	vsnprintf(last_error, 256, fmt, args);
	va_end(args);
#endif
}

void
element_from_string( element_t h, char* s )
{
	unsigned char* r;

	r = malloc(SHA_DIGEST_LENGTH);
	SHA1((unsigned char*) s, strlen(s), r);
	element_from_hash(h, r, SHA_DIGEST_LENGTH);

	free(r);
}
void
eval_poly( element_t r, bswabe_polynomial_t* q, element_t x )
{
	int i;
	element_t s, t;

	element_init_same_as(s, r);
	element_init_same_as(t, r);

	element_set0(r);
	element_set1(t);

	for( i = 0; i < q->deg + 1; i++ )
	{
		/* r += q->coef[i] * t */
		element_mul(s, q->coef[i], t);
		element_add(r, r, s);

		/* t = t * x = x^(i+1) */
		element_mul(t, t, x);
	}

	element_clear(s);
	element_clear(t);
}

bswabe_polynomial_t*
rand_poly( int deg, element_t zero_val )
{
	int i;
	bswabe_polynomial_t* q;

	q = (bswabe_polynomial_t*) malloc(sizeof(bswabe_polynomial_t));
	q->deg = deg;
	q->coef = (element_t*) malloc(sizeof(element_t) * (deg + 1));

	for( i = 0; i < q->deg + 1; i++ )
		element_init_same_as(q->coef[i], zero_val);

	element_set(q->coef[0], zero_val);

	for( i = 1; i < q->deg + 1; i++ )
	{
		element_random(q->coef[i]);
	}

	return q;
}

/************************** Setup **************************/
void
bswabe_setup( bswabe_pub_t** pub, bswabe_msk_t** msk )
{
	element_t alpha;
	element_t p0;				// P(0), the secret and zero_val of rpt, the polynomial
	int rpt_t = REVOKE_T;		// degree of rpt

	/* initialize */

	*pub = malloc(sizeof(bswabe_pub_t));
	*msk = malloc(sizeof(bswabe_msk_t));

	(*pub)->pairing_desc = strdup(TYPE_D159_PARAMS);

	pairing_init_set_buf((*pub)->p, (*pub)->pairing_desc, strlen((*pub)->pairing_desc));

	element_init_G1((*pub)->g,           (*pub)->p);
	element_init_G1((*pub)->h,           (*pub)->p);
	element_init_G2((*pub)->gp,          (*pub)->p);
	element_init_GT((*pub)->g_hat_alpha, (*pub)->p);	//e(g,gp)

	element_init_Zr(alpha,               (*pub)->p);
	element_init_Zr((*msk)->beta,        (*pub)->p);
	element_init_G2((*msk)->g_alpha,     (*pub)->p);	// gp^alpha
	element_init_Zr(p0,                  (*pub)->p);

	/* compute */

	element_random(alpha);
	element_random((*msk)->beta);
	element_random((*pub)->g);
	element_random((*pub)->gp);

	element_random(p0);
	(*msk)->rpt = rand_poly(rpt_t, p0);

	element_pow_zn((*msk)->g_alpha, (*pub)->gp,	alpha);	// g_alpha = gp^alpha
	element_pow_zn((*pub)->h, (*pub)->g, (*msk)->beta);	// h = g^beta

	pairing_apply((*pub)->g_hat_alpha, (*pub)->g, (*msk)->g_alpha, (*pub)->p);	// g_hat_alpha = e(g, g_alpha) = e(g, gp^alpha)
}

/************************** KeyGen **************************/
bswabe_prv_t*
bswabe_keygen( bswabe_pub_t* pub, bswabe_msk_t* msk, char** attributes, element_t u_k )
{
	bswabe_prv_t* prv;

	element_t g_r;
	element_t r;
	element_t beta_inv;
	element_t p_uk;
	element_t p_0;

	/* initialize */

	prv = malloc(sizeof(bswabe_prv_t));

	element_init_G2(prv->d, pub->p);
	element_init_G2(g_r, pub->p);
	element_init_Zr(r, pub->p);
	element_init_Zr(beta_inv, pub->p);

	element_init_Zr(u_k, pub->p);
	element_init_Zr(p_uk, pub->p);
	element_init_Zr(p_0, pub->p);

	prv->comps = g_array_new(0, 1, sizeof(bswabe_prv_comp_t));

	/* compute */

	element_random(r);
	element_pow_zn(g_r, pub->gp, r);			// g_r = gp^r

	element_mul(prv->d, msk->g_alpha, g_r);		// d = gp^alpha * gp^r
	element_invert(beta_inv, msk->beta); 		// beta_inv = 1/beta
	element_pow_zn(prv->d, prv->d, beta_inv);	// d = gp^((alpha + r)/beta)

	element_random(u_k);
	eval_poly(p_uk, (bswabe_polynomial_t*) msk->rpt, u_k);	// p_k = P(u_k)
	element_set(p_0, (msk->rpt)->coef[0]); 					//p_0 = the secret

	while( *attributes )
	{
		bswabe_prv_comp_t c;
		element_t h_rp;
		element_t rp;

		c.attr = *(attributes++);

		element_init_G2(c.d,  pub->p);
		element_init_G1(c.dp, pub->p);	//dp, dpp from G1
		element_init_G1(c.dpp, pub->p);

		element_init_G2(h_rp, pub->p);
		element_init_Zr(rp,   pub->p);

		element_from_string(h_rp, c.attr);	// h_rp = H(j)
		element_random(rp);
		element_pow_zn(h_rp, h_rp, rp); 	// h_rp = H(j)^rj
		element_pow_zn(h_rp, h_rp, p_0);	// h_rp = (H(j)^rj)^p_0 = H(j)^(rj.p_0)

		element_mul(c.d, g_r, h_rp);		// d = gp^r * H(j)^(rj.p_0)
		element_pow_zn(c.dp, pub->g, rp); 	// dp = gp^rj //sonia: changed g to gp, since D is of G2
		element_pow_zn(c.dpp, c.dp, p_uk); 	// d"= d' ^ p(uk)

		element_clear(h_rp);
		element_clear(rp);
		g_array_append_val(prv->comps, c);
	}

	element_clear(g_r);
	element_clear(r);
	element_clear(beta_inv);
	element_clear(p_uk);
	element_clear(p_0);

	return prv;
}

bswabe_policy_t*
base_node( int k, char* s )
{
	bswabe_policy_t* p;

	p = (bswabe_policy_t*) malloc(sizeof(bswabe_policy_t));
	p->k = k;
	p->attr = s ? strdup(s) : 0;
	if(s && k)
	{
		leaf_count2++;
	}
	p->children = g_ptr_array_new();
	p->q = 0;

	return p;
}

/*
	TODO convert this to use a GScanner and handle quotes and / or
	escapes to allow attributes with whitespace or = signs in them
 */

bswabe_policy_t*
parse_policy_postfix( char* s )
{
	char** toks;
	char** cur_toks;
	char*  tok;
	GPtrArray* stack; /* pointers to bswabe_policy_t's */
	bswabe_policy_t* root;

	toks     = g_strsplit(s, " ", 0);
	cur_toks = toks;
	stack    = g_ptr_array_new();

	while( *cur_toks )
	{
		int i, k, n;

		tok = *(cur_toks++);

		if( !*tok )
			continue;

		if( sscanf(tok, "%dof%d", &k, &n) != 2 )
			/* push leaf token */
			g_ptr_array_add(stack, base_node(1, tok));
		else
		{
			bswabe_policy_t* node;

			/* parse "kofn" operator */

			if( k < 1 )
			{
				raise_error("error parsing \"%s\": trivially satisfied operator \"%s\"\n", s, tok);
				return 0;
			}
			else if( k > n )
			{
				raise_error("error parsing \"%s\": unsatisfiable operator \"%s\"\n", s, tok);
				return 0;
			}
			else if( n == 1 )
			{
				raise_error("error parsing \"%s\": identity operator \"%s\"\n", s, tok);
				return 0;
			}
			else if( n > stack->len )
			{
				raise_error("error parsing \"%s\": stack underflow at \"%s\"\n", s, tok);
				return 0;
			}

			/* pop n things and fill in children */
			node = base_node(k, 0);
			g_ptr_array_set_size(node->children, n);
			for( i = n - 1; i >= 0; i-- )
				node->children->pdata[i] = g_ptr_array_remove_index(stack, stack->len - 1);

			/* push result */
			g_ptr_array_add(stack, node);
		}
	}

	if( stack->len > 1 )
	{
		raise_error("error parsing \"%s\": extra tokens left on stack\n", s);
		return 0;
	}
	else if( stack->len < 1 )
	{
		raise_error("error parsing \"%s\": empty policy\n", s);
		return 0;
	}

	root = g_ptr_array_index(stack, 0);

	g_strfreev(toks);
	g_ptr_array_free(stack, 0);

	return root;
}

void
fill_policy( bswabe_policy_t* p, bswabe_pub_t* pub, element_t e)
{
	int i;
	element_t r;
	element_t t;
	element_t h;

	element_init_Zr(r, pub->p);
	element_init_Zr(t, pub->p);
	element_init_G2(h, pub->p);

	p->q = rand_poly(p->k - 1, e);

	if( p->children->len == 0 )
	{
		element_init_G1(p->c,  pub->p);
		element_init_G2(p->cp, pub->p);

		element_from_string(h, p->attr);
		element_pow_zn(p->c,  pub->g, p->q->coef[0]);
		element_pow_zn(p->cp, h,      p->q->coef[0]);
	}
	else
	{
		for( i = 0; i < p->children->len; i++ )
		{
			element_set_si(r, i + 1);
			eval_poly(t, p->q, r);
			fill_policy(g_ptr_array_index(p->children, i), pub, t);
		}
	}
	element_clear(r);
	element_clear(t);
	element_clear(h);
}

/************************** Encrypt **************************/
bswabe_cph_t*
bswabe_enc( bswabe_pub_t* pub, element_t m, char* policy )
{
	bswabe_cph_t* cph;
	element_t s;

	/* initialize */

	cph = malloc(sizeof(bswabe_cph_t));

	element_init_Zr(s, pub->p);
	element_init_GT(m, pub->p);
	element_init_GT(cph->cs, pub->p);
	element_init_G1(cph->c,  pub->p);
	cph->p = parse_policy_postfix(policy);

	/* compute */

	element_random(m);
	element_random(s);

	element_pow_zn(cph->cs, pub->g_hat_alpha, s); // C~ = e(g, gp)^(alpha * s)
	element_mul(cph->cs, cph->cs, m); // C~ = m * e(g, gp)^(alpha * s)

	element_pow_zn(cph->c, pub->h, s); // C = h^s

	fill_policy(cph->p, pub, s);

	return cph;
}

/************************** Functions for Decryption with Regular or Delegated Private Key **************************/

/*to check with regular or  delegated key*/
void
check_sat( bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_del_prv_t* del_prv )
{
	int i, l;

	p->satisfiable = 0;
	if( p->children->len == 0 )
	{
		if(prv)
		{
			for( i = 0; i < prv->comps->len; i++ )
				if( !strcmp(g_array_index(prv->comps, bswabe_prv_comp_t, i).attr, p->attr) )
				{
					p->satisfiable = 1;
					p->attri = i;
					break;
				}
		}
		else
		{
			//printf("len = %u\n", del_prv->comps->len);
			for( i = 0; i < del_prv->comps->len; i++ )
			{

				//printf("%s\t%s\n", p->attr, g_array_index(del_prv->comps, bswabe_del_prv_comp_t, i).attr);
				if( !strcmp(g_array_index(del_prv->comps, bswabe_del_prv_comp_t, i).attr, p->attr) )
				{
					p->satisfiable = 1;
					p->attri = i;
					break;
				}
			}
		}
	}
	else
	{
		for( i = 0; i < p->children->len; i++ )
		{
			if (prv) 
				check_sat(g_ptr_array_index(p->children, i), prv, NULL);
			else 
				check_sat(g_ptr_array_index(p->children, i), NULL, del_prv);
		}

		l = 0;
		for( i = 0; i < p->children->len; i++ )
			if( ((bswabe_policy_t*) g_ptr_array_index(p->children, i))->satisfiable )
				l++;

		if( l >= p->k )
			p->satisfiable = 1;
	}
}

/*regular or delegated private key*/
void
pick_sat_naive( bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_del_prv_t* del_prv )
{
	int i, k, l;

	assert(p->satisfiable == 1);

	if( p->children->len == 0 )
		return;

	p->satl = g_array_new(0, 0, sizeof(int));

	l = 0;
	for( i = 0; i < p->children->len && l < p->k; i++ )
		if( ((bswabe_policy_t*) g_ptr_array_index(p->children, i))->satisfiable )
		{
			if(prv)
				pick_sat_naive(g_ptr_array_index(p->children, i), prv, NULL);
			else
				pick_sat_naive(g_ptr_array_index(p->children, i), NULL, del_prv);				
			l++;
			k = i + 1;
			g_array_append_val(p->satl, k);
		}
}

/* TODO there should be a better way of doing this */
bswabe_policy_t* cur_comp_pol;
int
cmp_int( const void* a, const void* b )
{
	int k, l;

	k = ((bswabe_policy_t*) g_ptr_array_index(cur_comp_pol->children, *((int*)a)))->min_leaves;
	l = ((bswabe_policy_t*) g_ptr_array_index(cur_comp_pol->children, *((int*)b)))->min_leaves;

	return
			k <  l ? -1 :
					k == l ?  0 : 1;
}

void
pick_sat_min_leaves( bswabe_policy_t* p, bswabe_prv_t* prv )
{
	int i, k, l;
	int* c;

	assert(p->satisfiable == 1);

	if( p->children->len == 0 )
		p->min_leaves = 1;
	else
	{
		for( i = 0; i < p->children->len; i++ )
			if( ((bswabe_policy_t*) g_ptr_array_index(p->children, i))->satisfiable )
				pick_sat_min_leaves(g_ptr_array_index(p->children, i), prv);

		c = alloca(sizeof(int) * p->children->len);
		for( i = 0; i < p->children->len; i++ )
			c[i] = i;

		cur_comp_pol = p;
		qsort(c, p->children->len, sizeof(int), cmp_int);

		p->satl = g_array_new(0, 0, sizeof(int));
		p->min_leaves = 0;
		l = 0;

		for( i = 0; i < p->children->len && l < p->k; i++ )
			if( ((bswabe_policy_t*) g_ptr_array_index(p->children, c[i]))->satisfiable )
			{
				l++;
				p->min_leaves += ((bswabe_policy_t*) g_ptr_array_index(p->children, c[i]))->min_leaves;
				k = c[i] + 1;
				g_array_append_val(p->satl, k);
			}
		assert(l == p->k);
	}
}

void
lagrange_coef2( element_t r, GArray* s, element_t i )
{
	int k;
	element_t t;
	element_t j;

	element_init_same_as(t, r);
	element_init_same_as(j, r);

	element_set1(r);

	for( k = 0; k < s->len; k++ )
	{
		element_set(j, g_array_index(s, element_t, k));

		if( element_cmp(j,i) == 0)
			continue;
		element_neg(t, j);
		element_mul(r, r, t); /* num_muls++;*/
		element_sub(t, i, j);
		element_invert(t, t);
		element_mul(r, r, t); /* num_muls++; */
	}

	element_clear(t);
	element_clear(j);
}

void
lagrange_coef( element_t r, GArray* s, int i )
{
	int j, k;
	element_t t;

	element_init_same_as(t, r);

	element_set1(r);

	for( k = 0; k < s->len; k++ )
	{
		j = g_array_index(s, int, k);

		if( j == i )
			continue;
		element_set_si(t, - j);
		element_mul(r, r, t); /* num_muls++;*/
		element_set_si(t, i - j);
		element_invert(t, t);
		element_mul(r, r, t); /* num_muls++; */
	}

	element_clear(t);
}
/* decrypt with regular key*/
void
dec_leaf_naive( element_t r, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub, element_t lambda_k)
{
	element_t t1;
	element_t t2;

	bswabe_prv_comp_t* c;


	c = &(g_array_index(prv->comps, bswabe_prv_comp_t, p->attri));

	element_init_GT(t1, pub->p);
	element_init_GT(t2, pub->p);

	pairing_apply(r, p->c,  c->d,  pub->p); /* num_pairings++; */ // r = e(c,d)
	pairing_apply(t1, c->dpp, p->cp, pub->p); // e(c', d")
	element_pow_zn(t1, t1, lambda_k);
	pairing_apply(t2, c->dp, p->cpp, pub->p); // e(c", d')

	element_mul(t1, t1, t2); /* t1 =  e(c', d")^lambda_k * e(c", d')*/
	element_invert(t1, t1);
	element_mul(r, r, t1); // r = e(c,d) / (e(c', d") * e(c", d')*/)

	element_clear(t1);
	element_clear(t2);
}

/*delegated key*/
void
dec_leaf_naive_delegated( element_t r, bswabe_policy_t* pa, bswabe_policy_t* pb, bswabe_del_prv_t* del_prv, bswabe_pub_t* pub, element_t lambda_AB, element_t lambda_BC)
{
	element_t t1;
	element_t t2;
	element_t t3;

	bswabe_del_prv_comp_t* c;

	c = &(g_array_index(del_prv->comps, bswabe_del_prv_comp_t, pa->attri));

	element_init_GT(t1, pub->p);
	element_init_GT(t2, pub->p);
	element_init_GT(t3, pub->p);

	pairing_apply(r, pa->c,  c->d,  pub->p); /* num_pairings++; */ // r = e(c,d)
	pairing_apply(t1, c->dpp, pb->cpp, pub->p); // e(d"~, pb-c") = e(d"~, c'^XB,)
	element_pow_zn(t1, t1, lambda_AB); // e(d"~, c"XB)^lambda_AB

	pairing_apply(t3, c->d3p, pa->cp, pub->p); // e(D'''~, C')
	element_pow_zn(t3, t3, lambda_AB);// e(D'''~, C')^lambda_AB
	element_pow_zn(t3, t3, lambda_BC);  // e(D'''~, C')^lambda_AB . lambda_BC

	element_mul(t1, t1, t3);

	pairing_apply(t2, c->dp, pa->cpp, pub->p); // e(d', pa-c") = e( d', c'^XA)

	element_mul(t1, t1, t2); /* t1 =  e(c', d")^lambda_k * e(c", d')*/
	element_invert(t1, t1);
	element_mul(r, r, t1); // r = e(c,d) / (e(c', d") * e(c", d')*/)

	element_clear(t1);
	element_clear(t2);
}


/*regular key*/
void dec_node_naive( element_t r, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub, element_t lambda_k );

/*delegated key*/
void dec_node_naive_delegated( element_t r, bswabe_policy_t* pa, bswabe_policy_t* pb, bswabe_del_prv_t* del_prv, bswabe_pub_t* pub, element_t lambda_AB, element_t lambda_BC );

/*regular key*/
void
dec_internal_naive( element_t r, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub, element_t lambda_k )
{
	int i;
	element_t s;
	element_t t;

	element_init_GT(s, pub->p);
	element_init_Zr(t, pub->p);

	element_set1(r);
	for( i = 0; i < p->satl->len; i++ )
	{
		dec_node_naive (s, g_ptr_array_index(p->children, g_array_index(p->satl, int, i) - 1), prv, pub, lambda_k );
		lagrange_coef(t, p->satl, g_array_index(p->satl, int, i));
		element_pow_zn(s, s, t); /* num_exps++; */
		element_mul(r, r, s); /* num_muls++; */
	}

	element_clear(s);
	element_clear(t);
}

/*delegated key*/
void
dec_internal_naive_delegated( element_t r, bswabe_policy_t* pa, bswabe_policy_t* pb, bswabe_del_prv_t* del_prv, bswabe_pub_t* pub, element_t lambda_AB, element_t lambda_BC )
{
	int i;
	element_t s;
	element_t t;

	element_init_GT(s, pub->p);
	element_init_Zr(t, pub->p);

	element_set1(r);
	for( i = 0; i < pa->satl->len; i++ )
	{
		dec_node_naive_delegated (s, g_ptr_array_index(pa->children, g_array_index(pa->satl, int, i) - 1), 
				g_ptr_array_index(pb->children, g_array_index(pa->satl, int, i) - 1), del_prv, pub, lambda_AB, lambda_BC);
		lagrange_coef(t, pa->satl, g_array_index(pa->satl, int, i));
		element_pow_zn(s, s, t); /* num_exps++; */
		element_mul(r, r, s); /* num_muls++; */
	}

	element_clear(s);
	element_clear(t);
}

/*regular key*/
void
dec_node_naive( element_t r, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub, element_t lambda_k)
{
	assert(p->satisfiable);
	if( p->children->len == 0 )
	{
		dec_leaf_naive(r, p, prv, pub, lambda_k);
	}
	else
		dec_internal_naive(r, p, prv, pub, lambda_k);
}

/*delegated key*/
void
dec_node_naive_delegated( element_t r, bswabe_policy_t* pa, bswabe_policy_t* pb, bswabe_del_prv_t* del_prv, bswabe_pub_t* pub, element_t lambda_AB, element_t lambda_BC )
{
	assert(pa->satisfiable);
	if( pa->children->len == 0 )
	{
		dec_leaf_naive_delegated(r, pa, pb, del_prv, pub, lambda_AB, lambda_BC);
	}
	else
		dec_internal_naive_delegated(r, pa, pb, del_prv, pub, lambda_AB, lambda_BC);
}

/*regular key*/
void
dec_naive( element_t r, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub, element_t lambda_k)
{
	dec_node_naive(r, p, prv, pub, lambda_k);
}
/*delegated key*/
void
dec_naive_delegated( element_t r, bswabe_policy_t* pa, bswabe_policy_t* pb, bswabe_del_prv_t* del_prv, bswabe_pub_t* pub, element_t lambda_AB, element_t lambda_BC )
{
	dec_node_naive_delegated(r, pa, pb, del_prv, pub, lambda_AB, lambda_BC);
}

void
dec_leaf_merge( element_t exp, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	bswabe_prv_comp_t* c;
	element_t s;

	c = &(g_array_index(prv->comps, bswabe_prv_comp_t, p->attri));

	if( !c->used )
	{
		c->used = 1;
		element_init_G1(c->z,  pub->p);
		element_init_G1(c->zp, pub->p);
		element_set1(c->z);
		element_set1(c->zp);
	}

	element_init_G1(s, pub->p);

	element_pow_zn(s, p->c, exp); /* num_exps++; */
	element_mul(c->z, c->z, s); /* num_muls++; */

	element_pow_zn(s, p->cp, exp); /* num_exps++; */
	element_mul(c->zp, c->zp, s); /* num_muls++; */

	element_clear(s);
}

void dec_node_merge( element_t exp, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub );

void
dec_internal_merge( element_t exp, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	int i;
	element_t t;
	element_t expnew;

	element_init_Zr(t, pub->p);
	element_init_Zr(expnew, pub->p);

	for( i = 0; i < p->satl->len; i++ )
	{
		lagrange_coef(t, p->satl, g_array_index(p->satl, int, i));
		element_mul(expnew, exp, t); /* num_muls++; */
		dec_node_merge(expnew, g_ptr_array_index
				(p->children, g_array_index(p->satl, int, i) - 1), prv, pub);
	}

	element_clear(t);
	element_clear(expnew);
}

void
dec_node_merge( element_t exp, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	assert(p->satisfiable);
	if( p->children->len == 0 )
		dec_leaf_merge(exp, p, prv, pub);
	else
		dec_internal_merge(exp, p, prv, pub);
}

void
dec_merge( element_t r, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	int i;
	element_t one;
	element_t s;

	/* first mark all attributes as unused */
	for( i = 0; i < prv->comps->len; i++ )
		g_array_index(prv->comps, bswabe_prv_comp_t, i).used = 0;

	/* now fill in the z's and zp's */
	element_init_Zr(one, pub->p);
	element_set1(one);
	dec_node_merge(one, p, prv, pub);
	element_clear(one);

	/* now do all the pairings and multiply everything together */
	element_set1(r);
	element_init_GT(s, pub->p);
	for( i = 0; i < prv->comps->len; i++ )
		if( g_array_index(prv->comps, bswabe_prv_comp_t, i).used )
		{
			bswabe_prv_comp_t* c = &(g_array_index(prv->comps, bswabe_prv_comp_t, i));

			pairing_apply(s, c->z, c->d, pub->p); /* num_pairings++; */
			element_mul(r, r, s); /* num_muls++; */

			pairing_apply(s, c->zp, c->dp, pub->p); /* num_pairings++; */
			element_invert(s, s);
			element_mul(r, r, s); /* num_muls++; */
		}
	element_clear(s);
}

void
dec_leaf_flatten( element_t r, element_t exp,
		bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	bswabe_prv_comp_t* c;
	element_t s;
	element_t t;

	c = &(g_array_index(prv->comps, bswabe_prv_comp_t, p->attri));

	element_init_GT(s, pub->p);
	element_init_GT(t, pub->p);

	pairing_apply(s, p->c,  c->d,  pub->p); /* num_pairings++; */
	pairing_apply(t, p->cp, c->dp, pub->p); /* num_pairings++; */
	element_invert(t, t);
	element_mul(s, s, t); /* num_muls++; */
	element_pow_zn(s, s, exp); /* num_exps++; */

	element_mul(r, r, s); /* num_muls++; */

	element_clear(s);
	element_clear(t);
}

void dec_node_flatten( element_t r, element_t exp,
		bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub );

void
dec_internal_flatten( element_t r, element_t exp,
		bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	int i;
	element_t t;
	element_t expnew;

	element_init_Zr(t, pub->p);
	element_init_Zr(expnew, pub->p);

	for( i = 0; i < p->satl->len; i++ )
	{
		lagrange_coef(t, p->satl, g_array_index(p->satl, int, i));
		element_mul(expnew, exp, t); /* num_muls++; */
		dec_node_flatten(r, expnew, g_ptr_array_index
				(p->children, g_array_index(p->satl, int, i) - 1), prv, pub);
	}

	element_clear(t);
	element_clear(expnew);
}

void
dec_node_flatten( element_t r, element_t exp,
		bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	assert(p->satisfiable);
	if( p->children->len == 0 )
		dec_leaf_flatten(r, exp, p, prv, pub);
	else
		dec_internal_flatten(r, exp, p, prv, pub);
}

void
dec_flatten( element_t r, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub )
{
	element_t one;

	element_init_Zr(one, pub->p);

	element_set1(one);
	element_set1(r);

	dec_node_flatten(r, one, p, prv, pub);

	element_clear(one);
}

/************************** Decryption with Regular Private Key **************************/
/*for regular key, dummy is NULL so that we can use the same small functions in both dec and dec_delegate*/
int
bswabe_dec( bswabe_pub_t* pub, bswabe_prv_t* prv, bswabe_cph_t* cph, element_t m, element_t lambda_k)
{
	element_t t;

	element_init_GT(m, pub->p);
	element_init_GT(t, pub->p);

	check_sat(cph->p, prv, NULL);
	if( !cph->p->satisfiable )
	{
		raise_error("cannot decrypt, attributes in key do not satisfy policy\n");
		return 0;
	}

	/* 	if( no_opt_sat ) */
	pick_sat_naive(cph->p, prv, NULL); /**/
	/* 	else */
	//            pick_sat_min_leaves(cph->p, prv);

	/* 	if( dec_strategy == DEC_NAIVE ) */
	dec_naive(t, cph->p, prv, pub, lambda_k); /**/
	/* 	else if( dec_strategy == DEC_FLATTEN ) */
	//	dec_flatten(t, cph->p, prv, pub);
	/* 	else */
	/* 		dec_merge(t, cph->p, prv, pub); */

	element_mul(m, cph->cs, t); /* num_muls++; */

	pairing_apply(t, cph->c, prv->d, pub->p); /* num_pairings++; */
	element_invert(t, t);
	element_mul(m, m, t); /* num_muls++; */
	return 1;
}

/************************** Decryption with Delegated Private Key **************************/

/*the only differnce between cpha and cphb is p->cpp, one is from proxyA, another from proxyB*/
int
bswabe_dec_delegated( bswabe_pub_t* pub, bswabe_del_prv_t* del_prv, bswabe_cph_t* cpha, bswabe_cph_t* cphb, element_t m, element_t lambda_AB, element_t lambda_BC)
{
	element_t t;

	element_init_GT(m, pub->p);
	element_init_GT(t, pub->p);

	check_sat(cpha->p, NULL, del_prv);
	if( !cpha->p->satisfiable )
	{
		raise_error("cannot decrypt_delegated, attributes in key do not satisfy policy\n");
		return 0;
	}
	pick_sat_naive(cpha->p, NULL,  del_prv);
	dec_naive_delegated(t, cpha->p, cphb->p, del_prv, pub, lambda_AB, lambda_BC); /**/
	element_mul(m, cpha->cs, t); /* num_muls++; */

	pairing_apply(t, cpha->c, del_prv->d, pub->p); /* num_pairings++; */
	element_invert(t, t);
	element_mul(m, m, t); /* num_muls++; */
	return 1;
}


/************************** Revoke **************************/
bswabe_point*
bswabe_revoke(bswabe_pub_t* pub, bswabe_msk_t* msk, char** revoked, int count)
{
	bswabe_point* rl;
	int i=0;

	rl = (bswabe_point*) malloc(sizeof(bswabe_point));

	rl->count = REVOKE_T;
	rl->x = (element_t*) malloc(sizeof(element_t) * REVOKE_T);
	rl->p_x = (element_t*) malloc(sizeof(element_t) * REVOKE_T);

	for(i=0; i<count; i++)
	{
		element_init_Zr(rl->x[i], pub->p);
		element_init_Zr(rl->p_x[i], pub->p);

		element_set_str(rl->x[i], revoked[i] , 10);
		eval_poly(rl->p_x[i], msk->rpt, rl->x[i]);
	}

	for(; i<REVOKE_T; i++)
	{
		element_init_Zr(rl->x[i], pub->p);
		element_init_Zr(rl->p_x[i], pub->p);

		element_random(rl->x[i]);
		eval_poly(rl->p_x[i], msk->rpt, rl->x[i]);
	}

	return rl;
}

/************************** Convert **************************/
int size = 0;
void
transform(bswabe_policy_t* p, element_t sum, bswabe_pub_t* pub)
{
	int i;

	if(p->children->len == 0)
	{
		element_init_G2(p->cpp, pub->p);
		element_pow_zn(p->cpp, p->cp, sum);
		size += element_length_in_bytes(p->cpp);
	}
	else
	{
		for(i=0; i<p->children->len; i++)
			transform(g_ptr_array_index(p->children, i), sum, pub);
	}

}
void
calc_sum(bswabe_pub_t* pub, bswabe_point* rvk, element_t u_k, element_t* li_ps, element_t sum)
{
	int i;

	element_t t;
	element_t r;
	element_t ui;

	element_init_same_as(t, u_k);
	element_init_same_as(r, u_k);
	element_init_same_as(ui, u_k);

	element_set0(sum);

	for(i=0; i<rvk->count; i++)
	{
		element_set1(r);

		element_neg(t, u_k);
		element_mul(r, r, t);
		element_set(ui, rvk->x[i]);
		element_sub(t, ui, u_k);
		element_invert(t, t);
		element_mul(r, r, t);
		element_set(t, li_ps[i]);
		element_mul(r, r, t);

		element_add(sum, sum, r);
	}

	element_clear(t);
	element_clear(r);
	element_clear(ui);

}
void convert(bswabe_pub_t* pub, bswabe_cph_t *cph, bswabe_point* rvk, char* u_k_str, element_t lambda_k, element_t* li_ps)
{
	element_t u_k;
	element_t sum;
	GArray* s;
	int i;

	element_init_Zr(lambda_k, pub->p);
	element_init_Zr(sum, pub->p);
	element_init_Zr(u_k, pub->p);

	element_set_str(u_k, u_k_str, 10);

	calc_sum(pub, rvk, u_k, li_ps, sum);	//SUM(l_i * P(ui))

	transform(cph->p, sum, pub);

	s = g_array_new(0, 0, sizeof(element_t));

	for(i=0; i<rvk->count; i++)
	{
		g_array_append_val(s, rvk->x[i]);
	}
	g_array_append_val(s, u_k);
	lagrange_coef2( lambda_k, s , u_k);

}

element_t*
bswabe_convert(bswabe_pub_t* pub, bswabe_point* rvk)
{
	int i;

	element_t lambda_i_p;
	GArray* s;

	element_t* lambda_i_ps;

	element_init_Zr(lambda_i_p, pub->p);

	s = g_array_new(0, 0, sizeof(element_t));

	lambda_i_ps = (element_t*) malloc(sizeof(element_t) * (REVOKE_T));

	for(i=0; i<rvk->count; i++)
		g_array_append_val(s, rvk->x[i]);

	for(i=0; i<rvk->count; i++)
	{
		lagrange_coef2( lambda_i_p, s , rvk->x[i]);
		element_mul(lambda_i_p, lambda_i_p, rvk->p_x[i]); //l_i' * p(ui)

		element_init_same_as(lambda_i_ps[i], lambda_i_p);
		element_set(lambda_i_ps[i], lambda_i_p);
	}
	element_clear(lambda_i_p);
	return lambda_i_ps;
}

/************************** Delegate **************************/
bswabe_del_prv_t*
bswabe_delegate( bswabe_pub_t* pub, bswabe_msk_t* msk, bswabe_prv_t* prv, char* u_k_c_str, char** attributes)
{
	int i = 0;

	bswabe_del_prv_t* del_prv; 	//delegated private key from A->B-> C
	element_t p_0_inv; 			// 1/p(0) of B
	element_t u_k_c; 			// C's identifier
	element_t p_c; 				//p(C) calculated by B


	del_prv = malloc(sizeof(bswabe_del_prv_t));
	del_prv->comps = g_array_new(0, 1, sizeof(bswabe_del_prv_comp_t));

	element_init_G2(del_prv->d, pub->p);
	element_init_Zr(p_0_inv, pub->p); 
	element_init_Zr(u_k_c, pub->p);  
	element_init_Zr(p_c, pub->p);

	element_set(del_prv->d, prv->d);
	element_invert(p_0_inv, (msk->rpt)->coef[0]); // 1/p(0)
	element_set_str(u_k_c, u_k_c_str, 10);
	eval_poly(p_c, (bswabe_polynomial_t*) msk->rpt, u_k_c); // p_c = P(C)

	while(*attributes) // no. of attrs to delegate
	{
		//printf("%s\n%d\n", *attributes, prv->comps->len);
		for( i = 0; i < prv->comps->len; i++ ) // all the attrs in SK_B
		{
			bswabe_prv_comp_t* b = &g_array_index(prv->comps, bswabe_prv_comp_t, i);

			//printf("i=%d Key attribute %s\n", i, b->attr);

			if(!strcmp(b->attr, *attributes))
			{
				//printf("Matched %s\n", *attributes);
				bswabe_del_prv_comp_t c;

				element_init_G2(c.d,  pub->p);
				element_init_G1(c.dp, pub->p); //dp, dpp, d3p from G1
				element_init_G1(c.dpp, pub->p);
				element_init_G1(c.d3p, pub->p);

				c.attr = *attributes;
				element_set(c.d, b->d);
				element_set(c.dp, b->dp);
				element_pow_zn(c.dpp, b->dpp, p_0_inv);  //d"^1/P(0)
				element_pow_zn(c.d3p, c.dpp, p_c);//(d"^1/P(0))^P(C)

				g_array_append_val(del_prv->comps, c);

				break;
			}

		}

		attributes++;		
	}
	element_clear(p_0_inv); 
	element_clear(u_k_c);  
	element_clear(p_c);

	return del_prv;
}

