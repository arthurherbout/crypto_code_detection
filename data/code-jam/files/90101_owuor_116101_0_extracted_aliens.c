#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#define DEBUG
#define ISCHAR 1
#define ISSTRING  2
typedef struct{
  int type;
  char *cp;
} string_t;

#define BUFSIZE 4096
#define MAXWORDLEN 15
#define MAXDICT 5000


int L, D, N;
string_t strvals[MAXWORDLEN];
int words_count;
char cur_word[MAXWORDLEN];
char dict[MAXDICT][MAXWORDLEN];
int machine_depth;


int check_words(int level, char *word);

int comp_dict_words(const void *wd1, const void *wd2)
{
  return strcmp((char*)wd1, (char *)wd2);
}

int comp_dict_words_limited(const void *wd1, const void *wd2)
{
  return strncmp((char*)wd1, (char *)wd2, machine_depth);
}

main(int argc, char *argv[])
{
  FILE *fp;
  char  lbuf[BUFSIZE];
  char *fname = (char *)argv[1];
  int   x, str, n;
  char *retp, *cp1;

  if (argc < 2 || !(fp=fopen(fname, "r"))){
    printf ("Usage: alien <input file>\n");
    return 0;
  }
  /* Load Test Cases */
  retp = fgets ((char*)lbuf, BUFSIZE, fp);
  if (retp){
    sscanf(lbuf, "%d %d %d", &L, &D, &N);
  }
  for (x=0; x<D; x++){
    retp = fgets (lbuf, BUFSIZE, fp);
    if (retp){
      cp1 = strtok(lbuf, " \n");
      strcpy(dict[x], cp1);
    }
  }
  //sort data
  qsort(dict, D, MAXWORDLEN, comp_dict_words);
  //Load and process Test Cases
  for (n=0; n<N; n++){
    int i, buflen, curchar;
    char *cp, cstr[MAXWORDLEN];

    //Read data into string type variable
    memset(lbuf, 0, BUFSIZE);
    retp = fgets(lbuf, BUFSIZE, fp);
    buflen = strlen(lbuf);
    for (i=0, cp=lbuf, curchar=0; i<buflen; i++){
      if (*cp == '('){                //Handle the bracket
	*cp = 0;
	strvals[curchar].cp = cp+1;
	strvals[curchar].type = ISSTRING;
	//close the bracket
	cp = strchr(cp+1, ')');
	*cp = 0;
      }
      else if ( *cp >= 'a' && *cp <='z'){  //normal char
	strvals[curchar].cp = cp;
	strvals[curchar].type = ISCHAR;
      }
      cp++;
      curchar++;
    }
    //now count
    words_count = 0;
    memset(cur_word, 0, L);
    check_words(0, cur_word);
    printf("Case #%d:  %d\n", n+1, words_count);
  }

  return 0;
}

int check_words(int level, char *word)
{
  int i;
  char lword[MAXWORDLEN];

  machine_depth = level;
  if (level>=3 && !bsearch(word, dict, D, MAXWORDLEN, comp_dict_words_limited)){
    //printf ("Pruned\n");
    return;
  }
  if (level < L){
    //move to new recursive level
    if (strvals[level].type == ISCHAR){
      strncpy(lword, word, level);
      lword[level] = *strvals[level].cp;
      check_words(level+1, lword);
    }
    else{  //enumerate possibilities
      for (i=0; i<strlen(strvals[level].cp); i++){
	strncpy(lword, word, level);
	lword[level] = *(strvals[level].cp+i);
	check_words(level+1, lword);
      }
    }
  }
  else { //search in dictionary
    word[level] = 0;
    //    printf("Searched for word: %s\n", word);
    if (bsearch(word, dict, D, MAXWORDLEN, comp_dict_words)){
      words_count++;
    }
  }
}
