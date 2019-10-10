#define o mems++
#define oo mems+= 2
#define ooo mems+= 3 \

#define show_basics 1
#define show_choices 2
#define show_details 4 \

#define vars_per_vchunk 341 \

#define cells_per_chunk 511 \

#define hack_in(q,t) (tmp_var*) (t|(ullng) q)  \

#define hack_out(q) (((ullng) q) &0x3) 
#define hack_clean(q) ((tmp_var*) ((ullng) q&-4) )  \

/*2:*/
#line 71 "./sat0w.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*5:*/
#line 171 "./sat0w.w"

typedef union{
char ch8[8];
uint u2[2];
long long lng;
}octa;
typedef struct tmp_var_struct{
octa name;
uint serial;
int stamp;
struct tmp_var_struct*next;
}tmp_var;

typedef struct vchunk_struct{
struct vchunk_struct*prev;
tmp_var var[vars_per_vchunk];
}vchunk;

/*:5*//*6:*/
#line 198 "./sat0w.w"

typedef struct chunk_struct{
struct chunk_struct*prev;
tmp_var*cell[cells_per_chunk];
}chunk;

/*:6*//*23:*/
#line 466 "./sat0w.w"

typedef struct{
uint litno;
}cell;

/*:23*//*24:*/
#line 481 "./sat0w.w"

typedef struct{
uint start;
uint wlink;
}clause;

/*:24*//*25:*/
#line 490 "./sat0w.w"

typedef struct{
octa name;
}variable;

/*:25*/
#line 78 "./sat0w.w"
;
/*3:*/
#line 100 "./sat0w.w"

int random_seed= 0;
int verbose= show_basics;
int show_choices_max= 1000000;
int hbits= 8;
int buf_size= 1024;
ullng imems,mems;
ullng bytes;
ullng nodes;
ullng thresh= 0;
ullng delta= 0;
ullng timeout= 0x1fffffffffffffff;

/*:3*//*7:*/
#line 204 "./sat0w.w"

char*buf;
tmp_var**hash;
uint hash_bits[93][8];
vchunk*cur_vchunk;
tmp_var*cur_tmp_var;
tmp_var*bad_tmp_var;
chunk*cur_chunk;
tmp_var**cur_cell;
tmp_var**bad_cell;
ullng vars;
ullng clauses;
ullng nullclauses;
ullng cells;

/*:7*//*26:*/
#line 495 "./sat0w.w"

cell*mem;
clause*cmem;
uint nonspec;
variable*vmem;
char*move;

/*:26*/
#line 79 "./sat0w.w"
;
/*27:*/
#line 510 "./sat0w.w"

void print_clause(uint c){
register uint k,l;
printf("%d:",c);
for(k= cmem[c].start;k<cmem[c-1].start;k++){
l= mem[k].litno;
printf(" %s%.8s",l&1?"~":"",vmem[l>>1].name.ch8);
}
printf("\n");
}

/*:27*//*28:*/
#line 524 "./sat0w.w"

void print_clauses_watching(int l){
register uint p;
for(p= cmem[l].wlink;p;p= cmem[p].wlink)
print_clause(p);
}

/*:28*//*29:*/
#line 533 "./sat0w.w"

void print_state(int l){
register int k;
fprintf(stderr," after %lld mems:",mems);
for(k= 1;k<=l;k++)fprintf(stderr,"%c",move[k]+'0');
fprintf(stderr,"\n");
fflush(stderr);
}

/*:29*/
#line 80 "./sat0w.w"
;
main(int argc,char*argv[]){
register uint c,h,i,j,k,l,p,q,r,level,parity;
/*4:*/
#line 135 "./sat0w.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,"%d",&verbose)-1);break;
case'c':k|= (sscanf(argv[j]+1,"%d",&show_choices_max)-1);break;
case'h':k|= (sscanf(argv[j]+1,"%d",&hbits)-1);break;
case'b':k|= (sscanf(argv[j]+1,"%d",&buf_size)-1);break;
case's':k|= (sscanf(argv[j]+1,"%d",&random_seed)-1);break;
case'd':k|= (sscanf(argv[j]+1,"%lld",&delta)-1);thresh= delta;break;
case'T':k|= (sscanf(argv[j]+1,"%lld",&timeout)-1);break;
default:k= 1;
}
if(k||hbits<0||hbits> 30||buf_size<=0){
fprintf(stderr,
"Usage: %s [v<n>] [c<n>] [h<n>] [b<n>] [s<n>] [d<n>] [T<n>] < foo.sat\n",argv[0]);
exit(-1);
}

/*:4*/
#line 83 "./sat0w.w"
;
/*8:*/
#line 219 "./sat0w.w"

gb_init_rand(random_seed);
buf= (char*)malloc(buf_size*sizeof(char));
if(!buf){
fprintf(stderr,"Couldn't allocate the input buffer (buf_size=%d)!\n",
buf_size);
exit(-2);
}
hash= (tmp_var**)malloc(sizeof(tmp_var)<<hbits);
if(!hash){
fprintf(stderr,"Couldn't allocate %d hash list heads (hbits=%d)!\n",
1<<hbits,hbits);
exit(-3);
}
for(h= 0;h<1<<hbits;h++)hash[h]= NULL;

/*:8*//*14:*/
#line 357 "./sat0w.w"

for(j= 92;j;j--)for(k= 0;k<8;k++)
hash_bits[j][k]= gb_next_rand();

/*:14*/
#line 84 "./sat0w.w"
;
/*9:*/
#line 248 "./sat0w.w"

while(1){
if(!fgets(buf,buf_size,stdin))break;
clauses++;
if(buf[strlen(buf)-1]!='\n'){
fprintf(stderr,
"The clause on line %lld (%.20s...) is too long for me;\n",clauses,buf);
fprintf(stderr," my buf_size is only %d!\n",buf_size);
fprintf(stderr,"Please use the command-line option b<newsize>.\n");
exit(-4);
}
/*10:*/
#line 285 "./sat0w.w"

for(j= k= 0;;){
while(buf[j]==' ')j++;
if(buf[j]=='\n')break;
if(buf[j]<' '||buf[j]> '~'){
fprintf(stderr,"Illegal character (code #%x) in the clause on line %lld!\n",
buf[j],clauses);
exit(-5);
}
if(buf[j]=='~')i= 1,j++;
else i= 0;
/*11:*/
#line 310 "./sat0w.w"

{
register tmp_var*p;
if(cur_tmp_var==bad_tmp_var)/*12:*/
#line 328 "./sat0w.w"

{
register vchunk*new_vchunk;
new_vchunk= (vchunk*)malloc(sizeof(vchunk));
if(!new_vchunk){
fprintf(stderr,"Can't allocate a new vchunk!\n");
exit(-6);
}
new_vchunk->prev= cur_vchunk,cur_vchunk= new_vchunk;
cur_tmp_var= &new_vchunk->var[0];
bad_tmp_var= &new_vchunk->var[vars_per_vchunk];
}

/*:12*/
#line 313 "./sat0w.w"
;
/*15:*/
#line 361 "./sat0w.w"

cur_tmp_var->name.lng= 0;
for(h= l= 0;buf[j+l]> ' '&&buf[j+l]<='~';l++){
if(l> 7){
fprintf(stderr,
"Variable name %.9s... in the clause on line %lld is too long!\n",
buf+j,clauses);
exit(-8);
}
h^= hash_bits[buf[j+l]-'!'][l];
cur_tmp_var->name.ch8[l]= buf[j+l];
}
if(l==0)goto empty_clause;
j+= l;
h&= (1<<hbits)-1;

/*:15*/
#line 315 "./sat0w.w"
;
/*16:*/
#line 377 "./sat0w.w"

for(p= hash[h];p;p= p->next)
if(p->name.lng==cur_tmp_var->name.lng)break;
if(!p){
p= cur_tmp_var++;
p->next= hash[h],hash[h]= p;
p->serial= vars++;
p->stamp= 0;
}

/*:16*/
#line 316 "./sat0w.w"
;
if(p->stamp==clauses||p->stamp==-clauses)/*17:*/
#line 391 "./sat0w.w"

{
if((p->stamp> 0)==(i> 0))goto empty_clause;
}

/*:17*/
#line 317 "./sat0w.w"

else{
p->stamp= (i?-clauses:clauses);
if(cur_cell==bad_cell)/*13:*/
#line 341 "./sat0w.w"

{
register chunk*new_chunk;
new_chunk= (chunk*)malloc(sizeof(chunk));
if(!new_chunk){
fprintf(stderr,"Can't allocate a new chunk!\n");
exit(-7);
}
new_chunk->prev= cur_chunk,cur_chunk= new_chunk;
cur_cell= &new_chunk->cell[0];
bad_cell= &new_chunk->cell[cells_per_chunk];
}

/*:13*/
#line 320 "./sat0w.w"
;
*cur_cell= p;
if(i==1)*cur_cell= hack_in(*cur_cell,1);
if(k==0)*cur_cell= hack_in(*cur_cell,2);
cur_cell++,k++;
}
}

/*:11*/
#line 296 "./sat0w.w"
;
}
if(k==0){
fprintf(stderr,"(Empty line %lld is being ignored)\n",clauses);
nullclauses++;
}
goto clause_done;
empty_clause:/*18:*/
#line 401 "./sat0w.w"

while(k){
/*19:*/
#line 410 "./sat0w.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:19*/
#line 403 "./sat0w.w"
;
k--;
}
if((buf[0]!='~')||(buf[1]!=' '))
fprintf(stderr,"(The clause on line %lld is always satisfied)\n",clauses);
nullclauses++;

/*:18*/
#line 303 "./sat0w.w"
;
clause_done:cells+= k;

/*:10*/
#line 259 "./sat0w.w"
;
}
if((vars>>hbits)>=10){
fprintf(stderr,"There are %lld variables but only %d hash tables;\n",
vars,1<<hbits);
while((vars>>hbits)>=10)hbits++;
fprintf(stderr," maybe you should use command-line option h%d?\n",hbits);
}
clauses-= nullclauses;
if(clauses==0){
fprintf(stderr,"No clauses were input!\n");
exit(-77);
}
if(vars>=0x80000000){
fprintf(stderr,"Whoa, the input had %llu variables!\n",vars);
exit(-664);
}
if(clauses>=0x80000000){
fprintf(stderr,"Whoa, the input had %llu clauses!\n",clauses);
exit(-665);
}
if(cells>=0x100000000){
fprintf(stderr,"Whoa, the input had %llu occurrences of literals!\n",cells);
exit(-666);
}

/*:9*/
#line 85 "./sat0w.w"
;
if(verbose&show_basics)
/*21:*/
#line 428 "./sat0w.w"

fprintf(stderr,
"(%lld variables, %lld clauses, %llu literals successfully read)\n",
vars,clauses,cells);

/*:21*/
#line 87 "./sat0w.w"
;
/*30:*/
#line 547 "./sat0w.w"

/*31:*/
#line 558 "./sat0w.w"

free(buf);free(hash);
mem= (cell*)malloc(cells*sizeof(cell));
if(!mem){
fprintf(stderr,"Oops, I can't allocate the big mem array!\n");
exit(-10);
}
bytes= cells*sizeof(cell);
nonspec= vars+vars+2;
if(nonspec+clauses>=0x100000000){
fprintf(stderr,"Whoa, nonspec+clauses is too big for me!\n");
exit(-667);
}
cmem= (clause*)malloc((nonspec+clauses)*sizeof(clause));
if(!cmem){
fprintf(stderr,"Oops, I can't allocate the cmem array!\n");
exit(-11);
}
bytes+= (nonspec+clauses)*sizeof(clause);
vmem= (variable*)malloc((vars+1)*sizeof(variable));
if(!vmem){
fprintf(stderr,"Oops, I can't allocate the vmem array!\n");
exit(-12);
}
bytes+= (vars+1)*sizeof(variable);
move= (char*)malloc((vars+1)*sizeof(char));
if(!move){
fprintf(stderr,"Oops, I can't allocate the move array!\n");
exit(-13);
}

/*:31*/
#line 548 "./sat0w.w"
;
/*32:*/
#line 589 "./sat0w.w"

for(j= 0;j<nonspec;j++)o,cmem[j].start= cmem[j].wlink= 0;
for(c= nonspec+clauses-1,k= 0;c>=nonspec;c--){
j= 0;
/*33:*/
#line 603 "./sat0w.w"

for(i= 0;i<2;j++){
/*19:*/
#line 410 "./sat0w.w"

if(cur_cell> &cur_chunk->cell[0])cur_cell--;
else{
register chunk*old_chunk= cur_chunk;
cur_chunk= old_chunk->prev;free(old_chunk);
bad_cell= &cur_chunk->cell[cells_per_chunk];
cur_cell= bad_cell-1;
}

/*:19*/
#line 605 "./sat0w.w"
;
i= hack_out(*cur_cell);
p= hack_clean(*cur_cell)->serial;
p+= p+(i&1)+2;
if(j==0)
ooo,cmem[c].start= k,cmem[c].wlink= cmem[p].wlink,cmem[p].wlink= c,j= 1;
o,mem[k++].litno= p;
}

/*:33*/
#line 593 "./sat0w.w"
;
}
o,cmem[c].start= k;

/*:32*/
#line 550 "./sat0w.w"
;
/*34:*/
#line 614 "./sat0w.w"

for(c= vars;c;c--){
/*20:*/
#line 419 "./sat0w.w"

if(cur_tmp_var> &cur_vchunk->var[0])cur_tmp_var--;
else{
register vchunk*old_vchunk= cur_vchunk;
cur_vchunk= old_vchunk->prev;free(old_vchunk);
bad_tmp_var= &cur_vchunk->var[vars_per_vchunk];
cur_tmp_var= bad_tmp_var-1;
}

/*:20*/
#line 616 "./sat0w.w"
;
o,vmem[c].name.lng= cur_tmp_var->name.lng;
}

/*:34*/
#line 551 "./sat0w.w"
;
/*35:*/
#line 623 "./sat0w.w"

if(cur_cell!=&cur_chunk->cell[0]||
cur_chunk->prev!=NULL||
cur_tmp_var!=&cur_vchunk->var[0]||
cur_vchunk->prev!=NULL){
fprintf(stderr,"This can't happen (consistency check failure)!\n");
exit(-14);
}
free(cur_chunk);free(cur_vchunk);

/*:35*/
#line 552 "./sat0w.w"
;

/*:30*/
#line 88 "./sat0w.w"
;
imems= mems,mems= 0;
/*36:*/
#line 639 "./sat0w.w"

level= 1;
newlevel:if(level> vars)goto satisfied;
oo,move[level]= (cmem[level+level+1].wlink!=0||cmem[level+level].wlink==0);
if((verbose&show_choices)&&level<=show_choices_max){
fprintf(stderr,"Level %d, trying %s%.8s",
level,move[level]?"~":"",vmem[level].name.ch8);
if(verbose&show_details)
fprintf(stderr," (%lld mems)",
mems);
fprintf(stderr,"\n");
}
nodes++;
if(delta&&(mems>=thresh))thresh+= delta,print_state(level);
if(mems> timeout){
fprintf(stderr,"TIMEOUT!\n");
goto done;
}
tryit:parity= move[level]&1;
/*37:*/
#line 680 "./sat0w.w"

for(o,c= cmem[level+level+1-parity].wlink;c;c= q){
oo,i= cmem[c].start,q= cmem[c].wlink,j= cmem[c-1].start;
for(p= i+1;p<j;p++){
o,k= mem[p].litno;
if(k>=level+level||(o,((move[k>>1]^k)&1)==0))break;
}
if(p==j){
if(verbose&show_details)
fprintf(stderr,"(Clause %d contradicted)\n",
c);
o,cmem[level+level+1-parity].wlink= c;
goto try_again;
}
oo,mem[i].litno= k,mem[p].litno= level+level+1-parity;
ooo,cmem[c].wlink= cmem[k].wlink,cmem[k].wlink= c;
if(verbose&show_details)
fprintf(stderr,"(Clause %d now watches %s%.8s)\n",
c,k&1?"~":"",vmem[k>>1].name.ch8);
}
o,cmem[level+level+1-parity].wlink= 0;

/*:37*/
#line 659 "./sat0w.w"
;
level++;goto newlevel;
try_again:if(o,move[level]<2){
o,move[level]= 3-move[level];
if((verbose&show_choices)&&level<=show_choices_max){
fprintf(stderr,"Level %d, trying again",level);
if(verbose&show_details)fprintf(stderr," (%lld mems)\n",
mems);
else fprintf(stderr,"\n");
}
goto tryit;
}
if(level> 1)/*38:*/
#line 702 "./sat0w.w"

{
level--;
goto try_again;
}

/*:38*/
#line 671 "./sat0w.w"
;
if(1){
printf("~\n");
if(verbose&show_basics)fprintf(stderr,"UNSAT\n");
}else{
satisfied:if(verbose&show_basics)fprintf(stderr,"!SAT!\n");
/*39:*/
#line 708 "./sat0w.w"

for(k= 1;k<level;k++)
printf(" %s%.8s",move[k]&1?"~":"",vmem[k].name.ch8);
printf("\n");

/*:39*/
#line 677 "./sat0w.w"
;
}

/*:36*/
#line 90 "./sat0w.w"
;
done:if(verbose&show_basics)
fprintf(stderr,"Altogether %llu+%llu mems, %llu bytes, %llu nodes.\n",
imems,mems,bytes,nodes);
}

/*:2*/
