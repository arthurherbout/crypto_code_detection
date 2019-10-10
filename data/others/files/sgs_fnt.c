

#include "sgs_int.h"


static int isoneofN( char ch, const char* what, int size )
{
	const char* end = what + size;
	while( what < end )
	{
		if( ch == *what++ )
			return 1;
	}
	return 0;
}


/* debugging */


static void dumpnode( SGS_CTX, sgs_FTNode* N )
{
	switch( N->type )
	{
	case SGS_SFT_FCALL: sgs_ErrWritef( C, "FCALL" ); break;
	case SGS_SFT_INDEX: sgs_ErrWritef( C, "INDEX" ); break;
	case SGS_SFT_MIDXSET: sgs_ErrWritef( C, "MULTI_INDEX_SET" ); break;
	case SGS_SFT_MPROPSET: sgs_ErrWritef( C, "MULTI_PROPERTY_SET" ); break;
	case SGS_SFT_ARGMT: sgs_ErrWritef( C, "ARG " ); sgsT_DumpToken( C, N->token ); break;
	case SGS_SFT_ARGLIST: sgs_ErrWritef( C, "ARG_LIST" ); break;
	case SGS_SFT_VARLIST: sgs_ErrWritef( C, "VAR_LIST" ); break;
	case SGS_SFT_GVLIST: sgs_ErrWritef( C, "GLOBAL_VAR_LIST" ); break;
	case SGS_SFT_USELIST: sgs_ErrWritef( C, "USE_LIST" ); break;
	case SGS_SFT_EXPLIST: sgs_ErrWritef( C, "EXPR_LIST" ); break;
	case SGS_SFT_ARRLIST: sgs_ErrWritef( C, "ARRAY_LIST" ); break;
	case SGS_SFT_DCTLIST: sgs_ErrWritef( C, "DICT_LIST" ); break;
	case SGS_SFT_MAPLIST: sgs_ErrWritef( C, "MAP_LIST" ); break;
	case SGS_SFT_RETURN: sgs_ErrWritef( C, "RETURN" ); break;
	case SGS_SFT_BLOCK: sgs_ErrWritef( C, "BLOCK" ); break;
	case SGS_SFT_IFELSE: sgs_ErrWritef( C, "IF/ELSE" ); break;
	case SGS_SFT_WHILE: sgs_ErrWritef( C, "WHILE" ); break;
	case SGS_SFT_DOWHILE: sgs_ErrWritef( C, "DO/WHILE" ); break;
	case SGS_SFT_FOR: sgs_ErrWritef( C, "FOR" ); break;
	case SGS_SFT_FOREACH: sgs_ErrWritef( C, "FOR_EACH" ); break;
	case SGS_SFT_BREAK: sgs_ErrWritef( C, "BREAK" );
		if( *sgsT_Next( N->token ) == SGS_ST_NUMINT )
		{
			sgs_Int val;
			SGS_AS_INTEGER( val, sgsT_Next( N->token ) + 1 );
			sgs_ErrWritef( C, " %" PRId64, val );
		}
		break;
	case SGS_SFT_CONT: sgs_ErrWritef( C, "CONTINUE" );
		if( *sgsT_Next( N->token ) == SGS_ST_NUMINT )
		{
			sgs_Int val;
			SGS_AS_INTEGER( val, sgsT_Next( N->token ) + 1 );
			sgs_ErrWritef( C, " %" PRId64, val );
		}
		break;
	case SGS_SFT_DEFER: sgs_ErrWritef( C, "DEFER" ); break;
	case SGS_SFT_FUNC: sgs_ErrWritef( C, "FUNC" ); break;
	case SGS_SFT_CLASS: sgs_ErrWritef( C, "CLASS" ); break;
	case SGS_SFT_CLSINH: sgs_ErrWritef( C, "CLASS_INHERIT" ); break;
	case SGS_SFT_CLSINC: sgs_ErrWritef( C, "CLASS_INCLUDE" ); break;
	case SGS_SFT_CLSGLOB: sgs_ErrWritef( C, "CLASS_GLOBALS" ); break;
	case SGS_SFT_NEWCALL: sgs_ErrWritef( C, "NEW" ); break;
	case SGS_SFT_THRCALL: sgs_ErrWritef( C, "THREAD" ); break;
	case SGS_SFT_STHCALL: sgs_ErrWritef( C, "SUBTHREAD" ); break;
	default:
		if( N->token ) sgsT_DumpToken( C, N->token );
		if( N->type == SGS_SFT_OPER_P ) sgs_ErrWritef( C, " [post]" );
		break;
	}
}

static void ft_dump( SGS_CTX, sgs_FTNode* node, int level )
{
	int i;
	sgs_FTNode* N = node;
	if( !node ) return;

	for( i = 0; i < level; ++i ) sgs_ErrWritef( C, "  " );
	dumpnode( C, N );
	sgs_ErrWritef( C, "\n" );

	if( node->child )
	{
		for( i = 0; i < level; ++i ) sgs_ErrWritef( C, "  " );
		sgs_ErrWritef( C, "{\n" );

		ft_dump( C, node->child, level + 1 );

		for( i = 0; i < level; ++i ) sgs_ErrWritef( C, "  " );
		sgs_ErrWritef( C, "}\n" );
	}

	ft_dump( C, node->next, level );
}

void sgsFT_Dump( SGS_CTX, sgs_FTNode* tree )
{
	ft_dump( C, tree, 0 );
}


/*
// C O M P I L E R
*/


typedef struct _ftcomp
{
	SGS_CTX;
	sgs_TokenList at;
	sgs_FTNode* heap;
	sgs_FTNode* heapend;
}
FTComp;

#define SFTC FTComp* F
#define SFTRET static sgs_FTNode*
#define SFTC_AT F->at
#define SFTC_NEXT F->at = sgsT_Next( F->at )
#define SFTC_IS( type ) (*F->at == (type))
/* WP: char/uchar conversion */
#define SFTC_IN( buf, sz ) isoneofN( (char) *F->at, buf, sz )
#define SFTC_HASERR ( F->C->state & SGS_HAS_ERRORS )
#define SFTC_SETERR F->C->state |= SGS_HAS_ERRORS
#define SFTC_ISKEY( name ) sgsT_IsKeyword( F->at, name )
#define SFTC_IS_ID( name ) sgsT_IsIdent( F->at, name )
#define SFTC_LINENUM sgsT_LineNum( F->at )
#define SFTC_PRINTERR( what ) sgs_Msg( F->C, SGS_ERROR, "[line %d] " what, SFTC_LINENUM )
#define SFTC_UNEXP sgs_Msg( F->C, SGS_ERROR, "Unexpected end of code", SFTC_LINENUM )


static sgs_FTNode* _make_heap( SGS_CTX )
{
	sgs_FTNode* node = sgs_Alloc_n( sgs_FTNode, 33 );
	node->type = SGS_SFT_HEAPBIT;
	node->token = NULL;
	node->next = NULL;
	node->child = NULL;
	return node;
}

static sgs_FTNode* _make_node( SFTC, int type, sgs_TokenList token, sgs_FTNode* next, sgs_FTNode* child )
{
	sgs_FTNode* node = NULL;
	if( ( F->heapend->type >> 8 ) == 32 )
	{
		F->heapend = F->heapend->next = _make_heap( F->C );
	}
	node = F->heapend + 1 + ( F->heapend->type >> 8 );
	F->heapend->type += ( 1 << 8 );
	
	node->type = type;
	node->token = token;
	node->next = next;
	node->child = child;
	return node;
}
#define make_node( ty, tok, next, ch ) _make_node( F, ty, tok, next, ch )

void sgsFT_Destroy( SGS_CTX, sgs_FTNode* tree )
{
	while( tree )
	{
		sgs_FTNode* next = tree->next;
		/* if( tree->child ) sgsFT_Destroy( C, tree->child ); HEAP CHILD IS NOT A SEPARATE ALLOCATION */
		sgs_Dealloc( tree );
		tree = next;
	}
}
#define SFTC_DESTROY( node ) (void)0 /* sgsFT_Destroy( F->C, node ) */





SFTRET parse_exp( SFTC, char* endtoklist, int etlsize );
SFTRET parse_stmt( SFTC );
SFTRET parse_stmtlist( SFTC, char end );
SFTRET parse_function( SFTC, int inexp, sgs_TokenList namepfx );





SFTRET parse_arg( SFTC, int argid, char end )
{
	sgs_FTNode* node = NULL;
	char toks[ 3 ] = { ',', 0, 0 };
	toks[1] = end;

	if( SFTC_IS(0) )
	{
		SFTC_UNEXP;
		goto fail;
	}

	if( SFTC_IS( SGS_ST_KEYWORD ) )
	{
		SFTC_PRINTERR( "Argument name cannot be a reserved keyword" );
		goto fail;
	}

	if( !SFTC_IS( SGS_ST_IDENT ) && !SFTC_IS( SGS_ST_KEYWORD ) )
	{
		sgs_Msg( F->C, SGS_ERROR, "[line %d] Unexpected token while parsing argument %d", SFTC_LINENUM, argid );
		goto fail;
	}

	node = make_node( SGS_SFT_ARGMT, SFTC_AT, NULL, NULL );
	SFTC_NEXT;

	if( SFTC_IS( SGS_ST_OP_SET ) )
	{
		SFTC_NEXT;
		if( SFTC_IS( end ) || SFTC_IS( ',' ) )
		{
			SFTC_PRINTERR( "Expected initializing expression" );
			goto fail;
		}
		node->child = parse_exp( F, toks, 2 );
		if( !node->child )
			goto fail;
	}

	return node;

fail:
	if( node ) SFTC_DESTROY( node );
	SFTC_SETERR;
	return 0;
}

/*
	FUNC / parses a list of arguments
	ARGS / context, function tree, token stream
	ERRS / unexpected token, @parse_arg
*/
SFTRET parse_arglist( SFTC, char end )
{
	sgs_FTNode* arglist = make_node( SGS_SFT_ARGLIST, NULL, NULL, NULL );
	sgs_FTNode* curnode = NULL;
	sgs_FTNode* argnode;
	int id = 1;

	for(;;)
	{
		if( SFTC_IS( end ) )
		{
			break;
		}
		else if( SFTC_IS( 0 ) )
		{
			SFTC_UNEXP;
			goto fail;
		}
		else if( id == 1 || SFTC_IS( ',' ) )
		{
			if( id != 1 )
				SFTC_NEXT;
			argnode = parse_arg( F, id, end );
			if( !argnode && F->C->state & SGS_MUST_STOP )
			{
				goto fail;
			}
			if( curnode ) curnode->next = argnode;
			else arglist->child = argnode;
			curnode = argnode;

			id++;
		}
		else
		{
			sgs_Msg( F->C, SGS_ERROR, "[line %d] Expected ',' or '%c'", SFTC_LINENUM, end );
			goto fail;
		}
	}

	return arglist;

fail:
	SFTC_SETERR;
	SFTC_DESTROY( arglist );
	return NULL;
}

/*
	FUNC / calculates the weight of the part
	ARGS / context, part of function tree
	ERRS / none
*/
static int part_weight( sgs_FTNode* part, int isfcall, int binary )
{
	sgs_BreakIf( !part );

	if( part->type == SGS_SFT_OPER && SGS_ST_OP_ASSIGN( *part->token ) )
		return 40;

	if( isfcall )
		return 9;

	if( part->type == SGS_SFT_OPER )
	{
		sgs_TokenType type = *part->token;
		if( binary )
		{
			if( SGS_ST_OP_BOOL( type ) )	return 30;
			if( type == SGS_ST_OP_NLOR )    return 29;
			if( type == SGS_ST_OP_RWCMP )   return 28; /* lower split prio .. */
			if( SGS_ST_OP_COMP( type ) )	return 29; /* .. than other comp ops */
			if( type == SGS_ST_OP_CAT )		return 27;
			if( SGS_ST_OP_BINOPS( type ) )	return 26;
			if( SGS_ST_OP_BINADD( type ) )	return 25;
			if( SGS_ST_OP_BINMUL( type ) )	return 24;
			if( type == SGS_ST_OP_NOT )     return 8;
			if( type == SGS_ST_OP_MMBR )	return 7;
			return 11;
		}

		/* unary operators */
		return 10;
	}

	/* everything else */
	return -1;
}



static sgs_LineNum findlinenum( sgs_FTNode* node ) /* local, next, child */
{
	sgs_LineNum ln = -1;

	if( node->token )
		return sgsT_LineNum( node->token );

	ln = findlinenum( node->next );
	if( ln != -1 ) return ln;

	ln = findlinenum( node->child );
	if( ln != -1 ) return ln;

	return -1;
}

static sgs_LineNum predictlinenum( sgs_FTNode* node ) /* next, child, local */
{
	sgs_LineNum ln = -1;

	ln = findlinenum( node->next );
	if( ln != -1 ) return ln;

	ln = predictlinenum( node->child );
	if( ln != -1 ) return ln;

	if( node->token )
		return sgsT_LineNum( node->token );

	return -1;
}


#define PFXFUNC_NONE 0
#define PFXFUNC_THREAD 1
#define PFXFUNC_SUBTHREAD 2
#define PFXFUNC_NEW 3
static int level_exp( SFTC, sgs_FTNode** tree )
{
	sgs_FTNode* node = *tree, *prev = NULL, *mpp = NULL;
	int weight = 0, isfcall, binary, count = 0;
	int pfxfuncmode = PFXFUNC_NONE;

	sgs_BreakIf( !tree );

	if( !*tree )
	{
		return 0;
	}
	
	if( sgsT_IsKeyword( node->token, "thread" ) )
	{
		pfxfuncmode = PFXFUNC_THREAD;
		node = node->next;
		(*tree)->next = NULL;
		SFTC_DESTROY( *tree );
		*tree = node;
	}
	if( sgsT_IsKeyword( node->token, "subthread" ) )
	{
		pfxfuncmode = PFXFUNC_SUBTHREAD;
		node = node->next;
		(*tree)->next = NULL;
		SFTC_DESTROY( *tree );
		*tree = node;
	}
	if( sgsT_IsKeyword( node->token, "new" ) )
	{
		pfxfuncmode = PFXFUNC_NEW;
		node = node->next;
		(*tree)->next = NULL;
		SFTC_DESTROY( *tree );
		*tree = node;
	}
	
	/* find the most powerful part (mpp) */
	while( node )
	{
		int leftmostsplit, curwt;
		
		count++;
		
		/* only interested in operators and subexpressions */
		if( node->type != SGS_SFT_OPER && node->type != SGS_SFT_EXPLIST && node->type != SGS_SFT_ARRLIST )
			goto _continue;
		
		/* "fcall" = [..]/(..) at the end, preceded by a data source */
		isfcall = node->type == SGS_SFT_EXPLIST || node->type == SGS_SFT_ARRLIST;
		if( isfcall ) isfcall = !!prev;
		if( isfcall ) isfcall = !node->next || ( node->type == SGS_SFT_ARRLIST && node->next->type == SGS_SFT_DCTLIST );
		if( isfcall ) isfcall = prev->type != SGS_SFT_OPER || !SGS_ST_OP_BINARY( *prev->token );
		
		/* op tests */
		binary = node->type == SGS_SFT_OPER;
		if( binary ) binary = prev && node->next;
		if( binary ) binary = prev->type != SGS_SFT_OPER || 
			*prev->token == SGS_ST_OP_INC || *prev->token == SGS_ST_OP_DEC;
		
		/* weighting */
		curwt = part_weight( node, isfcall, binary );
		leftmostsplit = 
			( node->type == SGS_SFT_OPER && SGS_ST_OP_ASSIGN( *node->token ) ) || /* assignment ops */
			( node->type == SGS_SFT_OPER && !binary ); /* unary ops */
		if( ( leftmostsplit && curwt > weight ) || ( !leftmostsplit && curwt >= weight ) )
		{
			weight = curwt;
			mpp = node;
		}
		
		/* move to next */
_continue:
		prev = node;
		node = node->next;
	}
	
	if( mpp )
	{
		/* function call */
		if( mpp->type == SGS_SFT_EXPLIST || mpp->type == SGS_SFT_ARRLIST )
		{
			int ret1, ret2;
			sgs_TokenList mpp_token = mpp->token;
			sgs_FTNode* se1 = *tree, *se2 = mpp, *se1i = *tree;
			while( se1i )
			{
				if( se1i->next == mpp )
					se1i->next = NULL;
				se1i = se1i->next;
			}
			
			ret1 = level_exp( F, &se1 );
			if( !ret1 )
			{
				*tree = NULL;
				if( se1 ) SFTC_DESTROY( se1 );
				if( se2 ) SFTC_DESTROY( se2 );
				return 0;
			}
			
			if( mpp->type == SGS_SFT_ARRLIST && !mpp->child && mpp->next && mpp->next->type == SGS_SFT_DCTLIST )
			{
				/* a multiset (index) expression */
				mpp->type = SGS_SFT_MIDXSET;
				mpp->child = se1;
				mpp->child->next = mpp->next;
				mpp->next = NULL;
				*tree = mpp;
				goto retsuccess;
			}
			
			ret2 = level_exp( F, &se2 );
			if( !ret2 )
			{
				*tree = NULL;
				if( se1 ) SFTC_DESTROY( se1 );
				if( se2 ) SFTC_DESTROY( se2 );
				return 0;
			}
			if( *mpp_token == SGS_ST_SBRKL )
			{
				/* array */
				if( !se2->child || se2->child->next )
				{
					sgs_Msg( F->C, SGS_ERROR, "[line %d] Invalid number of arguments "
						"in an array accessor", sgsT_LineNum( mpp_token ) );
					*tree = NULL;
					if( se1 ) SFTC_DESTROY( se1 );
					/* if( se2 ) */ SFTC_DESTROY( se2 );
					return 0;
				}
				se1->next = se2->child;
				se2->child = NULL;
				SFTC_DESTROY( se2 );
				*tree = make_node( SGS_SFT_INDEX, mpp_token, NULL, se1 );
				goto retsuccess;
			}
			se1->next = se2;
			*tree = make_node( SGS_SFT_FCALL, mpp_token, NULL, se1 );
			goto retsuccess;
		}
		
		/* operators */
		if( mpp->type == SGS_SFT_OPER )
		{
			if( mpp == *tree )
				prev = NULL;
			else
			{
				prev = *tree;
				while( prev->next != mpp )
					prev = prev->next;
			}
			
			/* binary operators */
			if( mpp != *tree && mpp->next && 
				( prev->type != SGS_SFT_OPER || *prev->token == SGS_ST_OP_INC || *prev->token == SGS_ST_OP_DEC ) )
			{
				sgs_TokenList mpptoken = mpp->token;
				int ret1, ret2;
				sgs_FTNode* se1 = *tree, *se2 = mpp->next, *se1i = *tree;
				while( se1i )
				{
					if( se1i->next == mpp )
						se1i->next = NULL;
					se1i = se1i->next;
				}
				mpp->next = NULL;
				
				ret1 = level_exp( F, &se1 );
				if( !ret1 )
				{
					*tree = NULL;
					if( se1 ) SFTC_DESTROY( se1 );
					if( se2 ) SFTC_DESTROY( se2 );
					SFTC_DESTROY( mpp );
					return 0;
				}
				
				if( mpptoken && *mpptoken == SGS_ST_OP_MMBR && !mpp->child && se2->type == SGS_SFT_DCTLIST )
				{
					/* a multiset (property) expression */
					mpp->type = SGS_SFT_MPROPSET;
					mpp->child = se1;
					mpp->child->next = se2;
					mpp->next = NULL;
					*tree = mpp;
					goto retsuccess;
				}
				
				SFTC_DESTROY( mpp );
				
				ret2 = level_exp( F, &se2 );
				if( !ret2 )
				{
					*tree = NULL;
					if( se1 ) SFTC_DESTROY( se1 );
					if( se2 ) SFTC_DESTROY( se2 );
					return 0;
				}
				
				se1->next = se2;
				*tree = make_node( SGS_SFT_OPER, mpptoken, NULL, se1 );
				
				if( *mpptoken == SGS_ST_OP_CAT || *mpptoken == SGS_ST_OP_CATEQ )
				{
					/* merge in CAT on first operand (works only with non-assignment op) */
					if( *mpptoken == SGS_ST_OP_CAT && *se1->token == SGS_ST_OP_CAT )
					{
						/* target tree: tree { se1:children, se2 } */
						sgs_FTNode* tmp = se1->child;
						while( tmp->next )
							tmp = tmp->next;
						tmp->next = se2;
						(*tree)->child = se1->child;
						
						se1->child = NULL;
						se1->next = NULL;
						SFTC_DESTROY( se1 );
					}
					/* merge in CAT on second operand */
					if( *se2->token == SGS_ST_OP_CAT )
					{
						/* target tree: tree { children without se2, se2:children } */
						sgs_FTNode* tmp = (*tree)->child;
						while( tmp->next && tmp->next != se2 )
							tmp = tmp->next;
						sgs_BreakIf( tmp->next == NULL );
						tmp->next = se2->child;
						
						se2->child = NULL;
						SFTC_DESTROY( se2 );
					}
				}
				
				goto retsuccess;
			}
			/* unary operators
				must be one of these cases:
				- no tokens before operator
				- is inc|dec and there are tokens either before or after the operator
					- can't have both, can't have neither
			*/
			else if( SGS_ST_OP_UNARY( *mpp->token ) && ( mpp == *tree || 
				( ( *mpp->token == SGS_ST_OP_INC || *mpp->token == SGS_ST_OP_DEC ) &&
				( mpp != *tree ) != ( !!mpp->next ) ) ) )
			{
				int ret1;
				if( !mpp->next )
				{
					sgs_FTNode* pp = *tree;
					if( pp == mpp ) goto fail;

					while( pp->next != mpp )
						pp = pp->next;
					pp->next = NULL;
					mpp->child = *tree;
					*tree = mpp;

					mpp->type = SGS_SFT_OPER_P;
				}
				else
				{
					mpp->child = mpp->next;
					mpp->next = NULL;
				}
				ret1 = level_exp( F, &mpp->child );
				if( !ret1 )
				{
					return 0;
				}
				goto retsuccess;
			}
			/* problems */
			else goto fail;
		}
	}
	
	if( count <= 1 )
	{
retsuccess:
		switch( pfxfuncmode )
		{
		case PFXFUNC_THREAD:
			if( (*tree)->type != SGS_SFT_FCALL )
			{
				SFTC_PRINTERR( "expected function call after 'thread'" );
				goto fail_no_err;
			}
			(*tree)->type = SGS_SFT_THRCALL;
			break;
		case PFXFUNC_SUBTHREAD:
			if( (*tree)->type != SGS_SFT_FCALL )
			{
				SFTC_PRINTERR( "expected function call after 'subthread'" );
				goto fail_no_err;
			}
			(*tree)->type = SGS_SFT_STHCALL;
			break;
		case PFXFUNC_NEW:
			if( (*tree)->type != SGS_SFT_FCALL ||
				(*tree)->child->type == SGS_SFT_OPER )
			{
				SFTC_PRINTERR( "expected plain function call after 'new'" );
				goto fail_no_err;
			}
			(*tree)->type = SGS_SFT_NEWCALL;
			break;
		default:
			break;
		}
		
		return 1;
	}

	/* in case we failed unexpectedly, dump & debug */
	sgs_Msg( F->C, SGS_ERROR, "[line %d] Missing operators or separators", predictlinenum( *tree ) );
	F->C->state |= SGS_HAS_ERRORS;
#if SGS_DEBUG && SGS_DEBUG_DATA
	sgsFT_Dump( F->C, *tree );
#endif
	return 0;

fail:
	if( mpp == NULL )
		mpp = *tree;
	sgs_Msg( F->C, SGS_ERROR, "[line %d] Invalid expression", mpp ? sgsT_LineNum( mpp->token ) : 0 );
fail_no_err:
#if SGS_DEBUG && SGS_DEBUG_DATA
	sgsFT_Dump( F->C, *tree );
#endif
	return 0;
}


SFTRET parse_dict( SFTC )
{
	sgs_TokenList startok = SFTC_AT;
	sgs_FTNode* expr = NULL, *fexp = NULL, *cur;
	/* dictionary expression */
	SFTC_NEXT;
	while( !SFTC_IS( '}' ) )
	{
		/* in a dict literal, with the exception of values, keywords = identifers */
		int is_ident = SFTC_IS( SGS_ST_IDENT ) || SFTC_IS( SGS_ST_KEYWORD );
		int is_varkey = SFTC_IS( '[' );
		if( !is_ident && !is_varkey && !SFTC_IS( SGS_ST_STRING ) )
		{
			SFTC_PRINTERR( "expected key identifier, string or '[' in dictionary expression" );
			goto fail;
		}
		
		/* make key */
		if( is_varkey )
		{
			SFTC_NEXT;
			cur = parse_exp( F, "]", 1 );
			if( !cur )
				goto fail;
		}
		else
		{
			cur = make_node( SGS_SFT_ARGMT, SFTC_AT, NULL, NULL );
		}
		SFTC_NEXT;
		
		/* add key to node list */
		if( !fexp )
			expr = fexp = cur;
		else
		{
			expr->next = cur;
			expr = expr->next;
		}
		
		if( !SFTC_IS( SGS_ST_OP_SET ) )
		{
			if( is_ident )
			{
				if( SFTC_IS( ',' ) || SFTC_IS( '}' ) )
				{
					expr->next = make_node( SGS_SFT_IDENT, expr->token, NULL, NULL );
					expr = expr->next;
				}
				else
				{
					SFTC_PRINTERR( "Expected '=', ',' or '}' after dictionary key" );
					goto fail;
				}
			}
			else
			{
				SFTC_PRINTERR( "Expected '=' in dictionary expression "
					"/ missing closing bracket before '{'" );
				goto fail;
			}
		}
		else
		{
			SFTC_NEXT;
			
			expr->next = parse_exp( F, ",}", 2 );
			if( !expr->next )
				goto fail;
			else
				expr = expr->next;
		}
		
		if( SFTC_IS( ',' ) )
			SFTC_NEXT;
	}
	return make_node( SGS_SFT_DCTLIST, startok, NULL, fexp );
	
fail:
	if( fexp )
		SFTC_DESTROY( fexp );
	return NULL;
}


SFTRET parse_exp( SFTC, char* endtoklist, int etlsize )
{
	sgs_FTNode* node, *cur;
	char prev = 0;

	if( SFTC_IS( 0 ) )
	{
		SFTC_UNEXP;
		SFTC_SETERR;
		return NULL;
	}

	/* special cases */
	if( SFTC_ISKEY( "var" ) )
	{
		SFTC_NEXT;
		node = parse_arglist( F, endtoklist[ endtoklist[0] == ',' ] );
		if( node )
			node->type = SGS_SFT_VARLIST;
		return node;
	}
	if( SFTC_ISKEY( "global" ) )
	{
		SFTC_NEXT;
		node = parse_arglist( F, endtoklist[ endtoklist[0] == ',' ] );
		if( node )
			node->type = SGS_SFT_GVLIST;
		return node;
	}

	cur = node = make_node( 0, NULL, NULL, NULL );

	for(;;)
	{
		if( SFTC_IN( endtoklist, etlsize ) )
		{
			break;
		}
		else if( SFTC_IS( 0 ) )
		{
			SFTC_UNEXP;
			goto fail;
		}
		else if( SFTC_IS( SGS_ST_STRING ) )
		{
			cur = cur->next = make_node( SGS_SFT_CONST, SFTC_AT, NULL, NULL );
			if( sgsT_IsIdent( sgsT_Next( SFTC_AT ), "c" ) )
			{
				uint32_t val;
				SGS_AS_UINT32( val, SFTC_AT + 1 );
				if( val != 1 )
				{
					SFTC_PRINTERR( "expected 1 character in character literal" );
					goto fail;
				}
				SFTC_NEXT;
			}
		}
		else if( SFTC_IS( SGS_ST_NUMINT )
			  || SFTC_IS( SGS_ST_NUMREAL )
			  || SFTC_IS( SGS_ST_NUMPTR ) )
		{
			cur = cur->next = make_node( SGS_SFT_CONST, SFTC_AT, NULL, NULL );
		}
		else if( SFTC_IS( SGS_ST_IDENT ) )
		{
			if( SFTC_IS_ID( "map" ) && sgsT_Next( SFTC_AT ) && *sgsT_Next( SFTC_AT ) == '{' )
			{
				SFTC_NEXT;
				cur->next = parse_dict( F );
				if( !cur->next )
					goto fail;
				cur = cur->next;
				cur->type = SGS_SFT_MAPLIST;
			}
			else
				cur = cur->next = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
		}
		else if( SFTC_IS( SGS_ST_KEYWORD ) )
		{
			if( SFTC_ISKEY( "function" ) )
			{
				cur->next = parse_function( F, 1, NULL );
				if( !cur->next )
					goto fail;
				cur = cur->next;
				continue;
			}
			else
				cur = cur->next = make_node( SGS_SFT_KEYWORD, SFTC_AT, NULL, NULL );
		}
		else if( SGS_ST_ISOP( *SFTC_AT ) )
			cur = cur->next = make_node( SGS_SFT_OPER, SFTC_AT, NULL, NULL );
		else if( SGS_ST_ISSPEC( *SFTC_AT ) )
		{
			/* array accesor / argument list / subexpression */
			if( SFTC_IS( '(' ) || SFTC_IS( '[' ) )
			{
				int isidx = prev == SGS_ST_IDENT || prev == ')' || prev == ']';
				char cend = SFTC_IS( '(' ) ? ')' : ']';
				sgs_FTNode* exprlist = make_node( SFTC_IS( '(' ) ? SGS_SFT_EXPLIST : SGS_SFT_ARRLIST, SFTC_AT, NULL, NULL );
				sgs_FTNode* expr, * curexpr = NULL;
				char endcstr[ 3 ] = { ',', 0, 0 };
				endcstr[1] = cend;

				SFTC_NEXT;
				/* if this is an empty expression (for a function call), do not process it further */
				if( !SFTC_IS( cend ) )
				{
					for(;;)
					{
						/* if not index, extra ',' is allowed */
						if( !isidx && SFTC_IS( cend ) )
						{
							SFTC_NEXT;
							break;
						}

						expr = parse_exp( F, endcstr, 2 );
						if( !expr )
						{
							SFTC_DESTROY( exprlist );
							goto fail;
						}

						if( curexpr )
							curexpr->next = expr;
						else
							exprlist->child = expr;
						curexpr = expr;

						if( SFTC_IS( cend ) )
						{
							SFTC_NEXT;
							break;
						}

						SFTC_NEXT;
					}
				}
				else
					SFTC_NEXT;

				cur = cur->next = exprlist;
				continue;
			}
			/* dictionary */
			else if( SFTC_IS( '{' ) )
			{
				cur->next = parse_dict( F );
				if( !cur->next )
					goto fail;
				cur = cur->next;
			}
			else
			{
				sgs_Msg( F->C, SGS_ERROR, "[line %d] Unexpected token '%c' found!", SFTC_LINENUM, *SFTC_AT );
				F->C->state |= SGS_MUST_STOP;
			}
		}
		else
		{
			SFTC_PRINTERR( "INTERNAL ERROR in parse_exp: unknown token found!" );
			F->C->state |= SGS_MUST_STOP;
		}

		if( F->C->state & SGS_MUST_STOP )
		{
			SFTC_DESTROY( node );
			return NULL;
		}
		
		/* WP: char/uchar conversion */
		prev = (char) *SFTC_AT;
		SFTC_NEXT;
	}

	cur = node->next;
	{
		node->child = NULL;
		node->next = NULL;
		SFTC_DESTROY( node );
	}
	node = cur;
	if( !node )
	{
		SFTC_PRINTERR( "Empty expression found" );
		goto fail;
	}

	if( !level_exp( F, &node ) )
		goto fail;
	
	return node;

fail:
	SFTC_SETERR;
	if( node ) SFTC_DESTROY( node );
	return NULL;
}





SFTRET parse_explist( SFTC, char endtok )
{
	sgs_FTNode* explist = make_node( SGS_SFT_EXPLIST, SFTC_AT, NULL, NULL );
	sgs_FTNode* curexp = NULL, *node;
	char endtoklist[] = { ',', 0, 0 };
	endtoklist[1] = endtok;

	for(;;)
	{
		if( SFTC_IS( endtok ) )
		{
			break;
		}
		else if( SFTC_IS( 0 ) )
		{
			SFTC_UNEXP;
			goto fail;
		}
		else if( SFTC_IS( ',' ) || SFTC_AT == explist->token )
		{
			if( SFTC_AT != explist->token )
				SFTC_NEXT;
			node = parse_exp( F, endtoklist, 2 );
			if( !node )
				goto fail;
			if( curexp )
				curexp->next = node;
			else
				explist->child = node;
			curexp = node;
		}
		else
		{
			sgs_Msg( F->C, SGS_ERROR, "[line %d] Expected ',' or '%c'", SFTC_LINENUM, endtok );
			goto fail;
		}
	}

	return explist;

fail:
	SFTC_SETERR;
	SFTC_DESTROY( explist );
	return NULL;
}

SFTRET parse_if( SFTC )
{
	sgs_FTNode *node = NULL, *nexp = NULL, *nif = NULL, *nelse = NULL;
	sgs_TokenList begin = SFTC_AT;

	SFTC_NEXT;
	if( !SFTC_IS( '(' ) )
	{
		SFTC_PRINTERR( "Expected '(' after 'if'" );
		goto fail;
	}

	SFTC_NEXT;

	nexp = parse_exp( F, ")", 1 );
	if( !nexp ) goto fail;
	SFTC_NEXT;

	nif = parse_stmt( F );
	if( !nif ) goto fail;

	if( SFTC_ISKEY( "else" ) )
	{
		SFTC_NEXT;
		nelse = parse_stmt( F );
		if( !nelse ) goto fail;
	}

	nexp->next = nif;
	nif->next = nelse;
	node = make_node( SGS_SFT_IFELSE, begin, NULL, nexp );

	return node;

fail:
	if( nexp ) SFTC_DESTROY( nexp );
	if( nif ) SFTC_DESTROY( nif );
	if( nelse ) SFTC_DESTROY( nelse );
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_while( SFTC )
{
	sgs_FTNode *node, *nexp = NULL, *nwhile = NULL;
	sgs_TokenList begin = SFTC_AT;

	SFTC_NEXT;
	if( !SFTC_IS( '(' ) )
	{
		SFTC_PRINTERR( "Expected '(' after 'while'" );
		goto fail;
	}

	SFTC_NEXT;

	nexp = parse_exp( F, ")", 1 );
	if( !nexp ) goto fail;
	SFTC_NEXT;

	nwhile = parse_stmt( F );
	if( !nwhile ) goto fail;

	nexp->next = nwhile;
	node = make_node( SGS_SFT_WHILE, begin, NULL, nexp );

	return node;

fail:
	if( nexp ) SFTC_DESTROY( nexp );
	if( nwhile ) SFTC_DESTROY( nwhile );
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_dowhile( SFTC )
{
	sgs_FTNode *node, *nexp = NULL, *nwhile = NULL;
	sgs_TokenList begin = SFTC_AT;

	SFTC_NEXT;
	nwhile = parse_stmt( F );
	if( !nwhile ) goto fail;

	if( !SFTC_ISKEY( "while" ) )
	{
		SFTC_PRINTERR( "Expected 'while' after statement in do/while" );
		goto fail;
	}

	SFTC_NEXT;
	if( !SFTC_IS( '(' ) )
	{
		SFTC_PRINTERR( "Expected '(' after 'while'" );
		goto fail;
	}

	SFTC_NEXT;

	nexp = parse_exp( F, ")", 1 );
	if( !nexp ) goto fail;
	SFTC_NEXT;

	nexp->next = nwhile;
	node = make_node( SGS_SFT_DOWHILE, begin, NULL, nexp );

	return node;

fail:
	if( nexp ) SFTC_DESTROY( nexp );
	if( nwhile ) SFTC_DESTROY( nwhile );
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_for( SFTC )
{
	sgs_TokenList begin = SFTC_AT;

	SFTC_NEXT;
	
	if( SFTC_IS( '(' ) )
	{
		sgs_FTNode *ninit = NULL, *nexp = NULL, *nincr = NULL, *nwhile = NULL;
		
		SFTC_NEXT;

		ninit = parse_explist( F, ';' );
		if( !ninit ) goto fail;
		SFTC_NEXT;

		nexp = parse_explist( F, ';' );
		if( !nexp ) goto fail;
		SFTC_NEXT;

		nincr = parse_explist( F, ')' );
		if( !nincr ) goto fail;
		SFTC_NEXT;

		nwhile = parse_stmt( F );
		if( !nwhile ) goto fail;

		ninit->next = nexp;
		nexp->next = nincr;
		nincr->next = nwhile;
		return make_node( SGS_SFT_FOR, begin, NULL, ninit );
		
	fail:
		if( ninit ) SFTC_DESTROY( ninit );
		if( nexp ) SFTC_DESTROY( nexp );
		if( nincr ) SFTC_DESTROY( nincr );
		if( nwhile ) SFTC_DESTROY( nwhile );
	}
	else if( SFTC_IS( SGS_ST_IDENT ) )
	{
		int realnum = 0;
		sgs_FTNode *nname = NULL, *nrange = NULL, *nwhile = NULL;
		
		nname = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
		
		SFTC_NEXT;
		
		if( !SFTC_IS( ':' ) )
		{
			SFTC_PRINTERR( "Expected ':' after identifier in 'numeric for'" );
			goto fail_numfor;
		}
		SFTC_NEXT;
		if( SFTC_IS( SGS_ST_OP_MMBR ) )
		{
			realnum = 1;
			SFTC_NEXT;
		}
		
		if( !SFTC_IS( '(' ) )
		{
			SFTC_PRINTERR( "Expected '(' after ':'/':.' in 'numeric for'" );
			goto fail_numfor;
		}
		SFTC_NEXT;
		
		nrange = parse_explist( F, ')' );
		if( !nrange ) goto fail_numfor;
		SFTC_NEXT;
		
		if( nrange->child == NULL ||
			( nrange->child->next &&
			nrange->child->next->next &&
			nrange->child->next->next->next ) )
		{
			SFTC_PRINTERR( "Expected 1-3 values in 'numeric for'" );
			goto fail_numfor;
		}

		nwhile = parse_stmt( F );
		if( !nwhile ) goto fail_numfor;
		
		nname->next = nrange;
		nrange->next = nwhile;
		return make_node( realnum ? SGS_SFT_FORNUMR : SGS_SFT_FORNUMI, begin, NULL, nname );
		
	fail_numfor:
		if( nname ) SFTC_DESTROY( nname );
		if( nrange ) SFTC_DESTROY( nrange );
		if( nwhile ) SFTC_DESTROY( nwhile );
	}
	else
	{
		SFTC_PRINTERR( "Expected '(' or identifier after 'for'" );
	}
	
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_foreach( SFTC )
{
	/*
		(x: => null=>ident
		(x,x: => ident=>ident
		(x,: => ident=>null
	*/
	sgs_FTNode *nvar = NULL, *nkey = NULL, *nexp = NULL, *nwhile = NULL;
	sgs_TokenList begin = SFTC_AT;

	SFTC_NEXT;
	if( !SFTC_IS( '(' ) )
	{
		SFTC_PRINTERR( "Expected '(' after 'foreach'" );
		goto fail;
	}

	SFTC_NEXT;
	if( !SFTC_IS( SGS_ST_IDENT ) )
	{
		SFTC_PRINTERR( "Expected identifier after '(' in 'foreach'" );
		goto fail;
	}
	/* (x:e) */
	nkey = make_node( SGS_SFT_NULL, SFTC_AT, NULL, NULL );
	nvar = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
	SFTC_NEXT;

	if( !SFTC_IS( ':' ) && !SFTC_IS( ',' ) )
	{
		SFTC_PRINTERR( "Expected ':' or ',' after identifier in 'foreach'" );
		goto fail;
	}

	if( SFTC_IS( ',' ) )
	{
		SFTC_NEXT;
		if( !SFTC_IS( SGS_ST_IDENT ) && !SFTC_IS( ':' ) )
		{
			SFTC_PRINTERR( "Expected identifier or ':' after ',' in 'foreach'" );
			goto fail;
		}

		if( SFTC_IS( SGS_ST_IDENT ) )
		{
			/* (x,x:e) */
			nkey->type = SGS_SFT_IDENT;
			nvar->token = SFTC_AT;
			SFTC_NEXT;
		}
		else
		{
			/* (x,:e) */
			nkey->type = SGS_SFT_IDENT;
			nvar->type = SGS_SFT_NULL;
		}

		if( !SFTC_IS( ':' ) )
		{
			SFTC_PRINTERR( "Expected ':' after identifier #2 or ',' in 'foreach'" );
			goto fail;
		}
	}

	SFTC_NEXT;

	nexp = parse_explist( F, ')' );
	if( !nexp ) goto fail;

	SFTC_NEXT;
	nwhile = parse_stmt( F );
	if( !nwhile ) goto fail;

	nkey->next = nvar;
	nvar->next = nexp;
	nexp->next = nwhile;
	return make_node( SGS_SFT_FOREACH, begin, NULL, nkey );

fail:
	if( nvar ) SFTC_DESTROY( nvar );
	if( nkey ) SFTC_DESTROY( nkey );
	if( nexp ) SFTC_DESTROY( nexp );
	if( nwhile ) SFTC_DESTROY( nwhile );
	return NULL;
}

SFTRET parse_function( SFTC, int inexp, sgs_TokenList namepfx )
{
	int hasname = 0;
	sgs_FTNode *nname = NULL, *nargs = NULL, *nbody = NULL, *nclos = NULL;
	sgs_TokenList begin = SFTC_AT;
	
	SFTC_NEXT;
	if( !inexp && !namepfx )
	{
		if( !SFTC_IS( SGS_ST_IDENT ) )
		{
			SFTC_PRINTERR( "Expected identifier after 'function'" );
			goto fail;
		}
	}
	
	if( SFTC_IS( SGS_ST_IDENT ) )
	{
		hasname = 1;
		nname = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
		SFTC_NEXT;
		if( namepfx )
		{
			nname = make_node( SGS_SFT_CLSPFX, namepfx, NULL, nname );
		}
		else if( SFTC_IS( SGS_ST_OP_MMBR ) )
		{
			nname = make_node( SGS_SFT_OPER, SFTC_AT, NULL, nname );
			SFTC_NEXT;
			if( !SFTC_IS( SGS_ST_IDENT ) )
			{
				SFTC_PRINTERR( "Expected identifier after 'function', identifier and '.'" );
				goto fail;
			}
			else
			{
				nname->child->next = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
				SFTC_NEXT;
			}
		}
	}
	
	if( !SFTC_IS( '(' ) )
	{
		if( !hasname )
			SFTC_PRINTERR( "Expected '(' after 'function'" );
		else
			SFTC_PRINTERR( "Expected '(' after 'function' and its name" );
		goto fail;
	}
	
	SFTC_NEXT;
	
	nargs = parse_arglist( F, ')' );
	if( !nargs ) goto fail;
	SFTC_NEXT;
	
	if( SFTC_ISKEY( "use" ) )
	{
		/* closure */
		SFTC_NEXT;
		if( !SFTC_IS( '(' ) )
		{
			SFTC_PRINTERR( "Expected '(' after 'use' in 'function'" );
			goto fail;
		}
		SFTC_NEXT;
		nclos = parse_arglist( F, ')' );
		if( !nclos ) goto fail;
		nclos->type = SGS_SFT_USELIST;
		SFTC_NEXT;
	}
	else
		nclos = make_node( SGS_SFT_USELIST, SFTC_AT, NULL, NULL );
	
	if( !SFTC_IS( '{' ) && !SFTC_IS( SGS_ST_OP_SET ) )
	{
		SFTC_PRINTERR( "Expected '{', '=' or 'use'" );
		goto fail;
	}
	
	if( SFTC_IS( SGS_ST_OP_SET ) )
	{
		SFTC_NEXT;
		nbody = parse_explist( F, ';' );
		if( !nbody ) goto fail;
		nbody->type = SGS_SFT_RETURN;
		SFTC_NEXT;
	}
	else
	{
		nbody = parse_stmt( F );
		if( !nbody ) goto fail;
	}
	
	nargs->next = nclos;
	nclos->next = nbody;
	nbody->next = nname;
	return make_node( SGS_SFT_FUNC, begin, NULL, nargs );
	
fail:
	if( nname ) SFTC_DESTROY( nname );
	if( nargs ) SFTC_DESTROY( nargs );
	if( nclos ) SFTC_DESTROY( nclos );
	if( nbody ) SFTC_DESTROY( nbody );
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_decltree( SFTC )
{
	sgs_FTNode *node, *instgt, *tmpnn;
	int level = 1;
	
	node = make_node( SGS_SFT_DCLTREE, SFTC_AT, NULL, NULL );
	SFTC_NEXT;
	
	if( !SFTC_IS( SGS_ST_IDENT ) ) goto intro_parse_fail;
	node->child = instgt = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
	SFTC_NEXT;
	if( !SFTC_IS( ':' ) ) goto intro_parse_fail;
	SFTC_NEXT;
	if( !SFTC_IS( SGS_ST_IDENT ) ) goto intro_parse_fail;
	instgt = instgt->next = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
	SFTC_NEXT;
	if( !SFTC_IS( '{' ) ) goto intro_parse_fail;
	SFTC_NEXT;
	
	for(;;)
	{
		if( SFTC_IS( '}' ) )
		{
			level--;
			if( level == 0 )
			{
				SFTC_NEXT;
				return node;
			}
			else
			{
				instgt = instgt->next = make_node( SGS_SFT_DTEXIT, SFTC_AT, NULL, NULL );
				SFTC_NEXT;
				continue;
			}
		}
		if( !SFTC_IS( SGS_ST_IDENT ) )
		{
			SFTC_PRINTERR( "expected key identifier in 'decltree'" );
			goto fail;
		}
		
		/* append identifier */
		instgt = instgt->next = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
		
		SFTC_NEXT;
		
		if( SFTC_IS( SGS_ST_OP_SET ) ) /* = */
		{
			SFTC_NEXT;
			tmpnn = parse_exp( F, ";", 1 );
			if( !tmpnn )
				goto fail;
			
			/* attach expression */
			instgt->child = tmpnn;
		}
		else if( SFTC_IS( '{' ) )
		{
			/* append subnode block */
			instgt = instgt->next = make_node( SGS_SFT_DTENTER, SFTC_AT, NULL, NULL );
			level++;
		}
		else if( SFTC_IS( '(' ) )
		{
			/* argument list for previous node */
			SFTC_NEXT;
			tmpnn = parse_explist( F, ')' );
			if( !tmpnn )
				goto fail;
			
			/* attach expression list */
			tmpnn->type = SGS_SFT_DTNPRM;
			instgt->child = tmpnn;
			
			SFTC_NEXT;
			if( !SFTC_IS( '{' ) )
			{
				SFTC_PRINTERR( "expected '{' after subnode argument list in 'decltree'" );
				goto fail;
			}
			
			/* append subnode block */
			instgt = instgt->next = make_node( SGS_SFT_DTENTER, SFTC_AT, NULL, NULL );
			level++;
		}
		else
		{
			SFTC_PRINTERR( "expected '=', '(' or '{' after key identifier in 'decltree'" );
			goto fail;
		}
		
		SFTC_NEXT;
	}
	
intro_parse_fail:
	SFTC_PRINTERR( "Expected 'decltree <treename> : <classname> { ... }'" );
fail:
	SFTC_DESTROY( node );
	return NULL;
}

SFTRET parse_class( SFTC )
{
	sgs_FTNode *node, **nit;
		
	node = make_node( SGS_SFT_CLASS, SFTC_AT, NULL, NULL );
	nit = &node->child;
	
	if( !SFTC_IS( SGS_ST_IDENT ) || SFTC_IS_ID( "class" ) )
	{
		SFTC_PRINTERR( "Expected identifier after 'class'" );
		goto fail;
	}
	*nit = make_node( SGS_SFT_IDENT, SFTC_AT, NULL, NULL );
	nit = &(*nit)->next;
	SFTC_NEXT;
	
	if( SFTC_IS( ':' ) )
	{
		SFTC_NEXT;
		if( !SFTC_IS( SGS_ST_IDENT ) )
		{
			SFTC_PRINTERR( "Expected identifier after ':' in class" );
			goto fail;
		}
		*nit = make_node( SGS_SFT_CLSINH, SFTC_AT, NULL, NULL );
		nit = &(*nit)->next;
		SFTC_NEXT;
	}
	if( !SFTC_IS( '{' ) )
	{
		SFTC_PRINTERR( "Expected '{' after (inherited) class name" );
		goto fail;
	}
	SFTC_NEXT;
	
	while( !SFTC_IS( '}' ) )
	{
		sgs_FTNode* nn;
		if( SFTC_ISKEY( "global" ) )
		{
			SFTC_NEXT;
			nn = parse_arglist( F, ';' );
			if( !nn )
				goto fail;
			nn->type = SGS_SFT_CLSGLOB;
			*nit = nn;
			nit = &(*nit)->next;
			SFTC_NEXT;
		}
		else if( SFTC_ISKEY( "function" ) )
		{
			nn = parse_function( F, 0, node->child->token );
			if( !nn )
				goto fail;
			*nit = nn;
			nit = &(*nit)->next;
		}
		else
		{
			SFTC_PRINTERR( "Unexpected token in class" );
			goto fail;
		}
	}
	
	return node;
	
fail:
	if( node ) SFTC_DESTROY( node );
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_command( SFTC, int multi )
{
	sgs_FTNode *nargs = NULL;
	sgs_TokenList begin = SFTC_AT;

	SFTC_NEXT;
	
	nargs = parse_explist( F, ';' );
	if( !nargs ) goto fail;
	SFTC_NEXT;
	
	if( multi )
	{
		/* one argument to one function call */
		sgs_FTNode *r = NULL, *n = NULL, *p = nargs->child;
		
		if( !p )
		{
			if( nargs ) SFTC_DESTROY( nargs );
			return make_node( SGS_SFT_BLOCK, begin, NULL, NULL );
		}
		
		nargs->child = NULL;
		SFTC_DESTROY( nargs );
		
		while( p )
		{
			sgs_FTNode* nn = make_node( SGS_SFT_FCALL, begin, NULL,
				make_node( SGS_SFT_IDENT, begin, 
					make_node( SGS_SFT_EXPLIST, p->token, NULL, p ),
						NULL ) );
			sgs_FTNode* pp = p;
			p = p->next;
			pp->next = NULL;
			if( !r )
				r = n = nn;
			else
				n = n->next = nn;
		}
		
		return make_node( SGS_SFT_BLOCK, begin, NULL, r );
	}
	else
	{
		/* one function call */
		sgs_FTNode* nname = make_node( SGS_SFT_IDENT, begin, nargs, NULL );
		return make_node( SGS_SFT_FCALL, begin, NULL, nname );
	}
	
fail:
	if( nargs ) SFTC_DESTROY( nargs );
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_stmt( SFTC )
{
	sgs_FTNode* node;

	if( SFTC_IS(0) )
	{
		SFTC_UNEXP;
		goto fail;
	}

	if( SFTC_ISKEY( "if" ) ) { return parse_if( F ); }
	else if( SFTC_ISKEY( "else" ) )
	{
		SFTC_PRINTERR( "Found 'else' without matching 'if'" );
		goto fail;
	}
	else if( SFTC_ISKEY( "while" ) ) { return parse_while( F ); }
	else if( SFTC_ISKEY( "do" ) ) { return parse_dowhile( F ); }
	else if( SFTC_ISKEY( "for" ) ) { return parse_for( F ); }
	else if( SFTC_ISKEY( "foreach" ) ) { return parse_foreach( F ); }
	else if( SFTC_ISKEY( "break" ) )
	{
		sgs_TokenList orig = SFTC_AT;
		SFTC_NEXT;

		if( SFTC_IS( SGS_ST_NUMINT ) )
		{
			sgs_Int blev;
			SGS_AS_INTEGER( blev, SFTC_AT + 1 );
			if( blev < 1 || blev > 255 )
			{
				SFTC_PRINTERR( "Invalid break level (can be between 1 and 255)" );
				goto fail;
			}
			SFTC_NEXT;
		}
		
		if( !SFTC_IS( ';' ) )
		{
			SFTC_UNEXP;
			goto fail;
		}
		SFTC_NEXT;

		return make_node( SGS_SFT_BREAK, orig, NULL, NULL );
	}
	else if( SFTC_ISKEY( "continue" ) )
	{
		sgs_TokenList orig = SFTC_AT;
		SFTC_NEXT;

		if( SFTC_IS( SGS_ST_NUMINT ) )
		{
			sgs_Int blev;
			SGS_AS_INTEGER( blev, SFTC_AT + 1 );
			if( blev < 1 || blev > 255 )
			{
				SFTC_PRINTERR( "Invalid continue level (can be between 1 and 255)" );
				goto fail;
			}
			SFTC_NEXT;
		}
		
		if( !SFTC_IS( ';' ) )
		{
			SFTC_UNEXP;
			goto fail;
		}
		SFTC_NEXT;

		return make_node( SGS_SFT_CONT, orig, NULL, NULL );
	}
	else if( SFTC_ISKEY( "function" ) ) { return parse_function( F, 0, NULL ); }
	else if( SFTC_ISKEY( "return" ) )
	{
		SFTC_NEXT;
		node = parse_explist( F, ';' );
		if( !node )
			goto fail;

		node->type = SGS_SFT_RETURN;

		SFTC_NEXT;
		return node;
	}
	else if( SFTC_ISKEY( "var" ) || SFTC_ISKEY( "global" ) )
	{
		node = parse_exp( F, ";", 1 );
		if( !node )
			goto fail;
		if( !SFTC_IS( ';' ) )
		{
			SFTC_UNEXP;
			SFTC_DESTROY( node );
			goto fail;
		}

		SFTC_NEXT;
		return node;
	}
	else if( SFTC_ISKEY( "defer" ) )
	{
		sgs_TokenList orig = SFTC_AT;
		SFTC_NEXT;
		node = parse_stmt( F );
		if( !node )
			goto fail;
		return make_node( SGS_SFT_DEFER, orig, NULL, node );
	}
	else if( SFTC_ISKEY( "decltree" ) )
	{
		return parse_decltree( F );
	}
#define NOT_FCALL ( !sgsT_Next( F->at ) || '(' != *sgsT_Next( F->at ) )
	else if( SFTC_IS_ID( "class" ) && NOT_FCALL )
	{
		SFTC_NEXT;
		node = parse_class( F );
		if( !node )
			goto fail;
		SFTC_NEXT;
		return node;
	}
	/* SIMPLE COMMANDS */
	else if((
		SFTC_IS_ID( "print" ) ||
		SFTC_IS_ID( "println" ) ||
		SFTC_IS_ID( "yield" )
		) && NOT_FCALL )
	{
		return parse_command( F, 0 );
	}
	/* MULTIPLIED COMMANDS */
	else if( SFTC_IS_ID( "include" ) && NOT_FCALL )
	{
		return parse_command( F, 1 );
	}
	/* BLOCK OF STATEMENTS */
	else if( SFTC_IS( SGS_ST_CBRKL ) )
	{
		SFTC_NEXT;
		node = parse_stmtlist( F, '}' );
		if( !node ) goto fail;

		SFTC_NEXT;
		return node;
	}
	/* SEPARATED STATEMENTS */
	else
	{
		node = parse_explist( F, ';' );
		if( node )
		{
			SFTC_NEXT;
			return node;
		}
		else
			goto fail;
	}

fail:
	SFTC_SETERR;
	return NULL;
}

SFTRET parse_stmtlist( SFTC, char end )
{
	sgs_FTNode* stmtlist = make_node( SGS_SFT_BLOCK, NULL, NULL, NULL );
	sgs_FTNode* curstmt = NULL;

	for(;;)
	{
		if( SFTC_IS( end ) )
		{
			break;
		}
		else if( SFTC_IS( 0 ) )
		{
			SFTC_UNEXP;
			SFTC_SETERR;
			goto fail;
		}
		else
		{
			sgs_FTNode* stmt = parse_stmt( F );
			if( curstmt )
				curstmt->next = stmt;
			else
				stmtlist->child = stmt;
			curstmt = stmt;
		}

		if( F->C->state & SGS_MUST_STOP )
			goto fail;
	}

	return stmtlist;

fail:
	SFTC_DESTROY( stmtlist );
	SFTC_SETERR;
	return NULL;
}

sgs_FTNode* sgsFT_Compile( SGS_CTX, sgs_TokenList tlist )
{
	sgs_FTNode* ret;
	FTComp F;
	{
		F.C = C;
		F.at = tlist;
		F.heap = F.heapend = _make_heap( C );
	}
	ret = parse_stmtlist( &F, 0 );
	if( !ret )
	{
		sgsFT_Destroy( C, F.heap );
		return NULL;
	}
	F.heap->child = ret;
	return F.heap;
}


