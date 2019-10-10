// C Runtime system for the APL compiler C backend
//
// Copyright (c) 2015, Martin Elsman
// MIT License

// ------------------
// Some abbreviations
// ------------------

#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define ori(x,y) (int)(((unsigned int)(x))|((unsigned int)(y)))
#define andi(x,y) (int)(((unsigned int)(x))&((unsigned int)(y)))
#define xori(x,y) (int)(((unsigned int)(x))^((unsigned int)(y)))
#define shli(x,y) (int)(((unsigned int)(x))<<((unsigned int)(y)))
#define shri(x,y) (int)(((unsigned int)(x))>>((unsigned int)(y)))
#define shari(x,y) (int)(((int)(x))>>((unsigned int)(y)))
#define i2d(x) ((double)x)
#define d2i(x) ((int)x)
#define b2i(x) ((x)?1:0)
#define ln(x)  (log(x))
#define true 1
#define false 0
#define bool int

int maxi(int a, int b) { return (a > b) ? a : b; }
int mini(int a, int b) { return (a < b) ? a : b; }

double maxd(double a, double b) { return (a > b) ? a : b; }
double mind(double a, double b) { return (a < b) ? a : b; }


// ------------------------------
// Printing of scalars and arrays
// ------------------------------

static void prInt(int i) { printf("%d", i); }

static void prBool(int b) { prInt(b); }

// [countChar(c,s)] returns the number of occurences of c in s.
static ssize_t countChar(ssize_t c, char *s) {
  char *p; 
  ssize_t count;

  count = 0;
  for( p=s; *p != '\0'; p++ ) 
    {
      if( *p == c ) count++;
    }
  return count;
}

static void formatD(char* buf, double arg)
{
  sprintf(buf, "%.12g", arg);
  if( countChar('.', buf) == 0 && countChar('E', buf) == 0 ) 
    {
      strcat(buf, ".0");
    }
}

static void prDouble(double arg)
{
  char buf[64];
  formatD(buf, arg);
  printf("%s", buf);
}

// Print result of program evaluation

static void prScalarDouble(double arg)
{
  printf("[]("); prDouble(arg); printf(")");
}

// ---------------------------
// Some mathematical functions
// ---------------------------

static int resi(int y, int x)    // notice the swapped arguments
{
  int tmp;
  if ( y == 0 ) { return x; }
  tmp = x % y;
  if (tmp == 0 || (x>0 && y>0) || (x<0 && y<0)) {
    return tmp;
  } else {
    return tmp+y;
  }
}

static int floori(double x) {
  return (int)floor(x);
}

static int ceili(double x) {
  return (int)ceil(x);
}

// ------------------------------
// Roll function
// ------------------------------

static double roll (int x) {
  int i = rand();
  double r = ((double)i)/((double)RAND_MAX);
  if (x == 0) {
    return r;
  }
  int y = (int)(x * r);
  return (double)y;
}


// -----------------
// Now function
// -----------------

struct timeval tv_init;

static void initialize() {
  gettimeofday(&tv_init, NULL);
  return;
}

// return time since process start in milliseconds
static int now (int x) {
  struct timeval tv_check;
  gettimeofday(&tv_check, NULL);
  long int usec = tv_check.tv_usec - tv_init.tv_usec;
  long int sec = tv_check.tv_sec - tv_init.tv_sec;
  long int msec = usec / 1000;
  return (int)(sec*1000+msec);
}

// -----------------
// Halting execution
// -----------------

void halt(char *s) {
  printf("Execution halted: %s\n",s);
  exit(1);
}


/* Buffer size in number of bytes */
#define INIT_BUFFER_SIZE (4*1024)

/* Copy values to a new buffer, double the size, free the old buffer. */
int double_buffersize(int* bufferSize, void** buffer_ptr) {
  int newBufferSize = 2*(*bufferSize);
  //printf("Doubling the buffer, new buffer size: %d bytes\n", newBufferSize);
  int* buffer = *buffer_ptr;
  int* newBuffer = malloc (newBufferSize);
  if (newBuffer != NULL) {
    memcpy(newBuffer, buffer, *bufferSize);
    free(buffer);
    *buffer_ptr = newBuffer;
    *bufferSize = newBufferSize;
    return 1;
  } else {
    return 0;
  }
}

int* read_csv_ints(FILE* handle, int* valuesRead) {
  int bufferSize = INIT_BUFFER_SIZE;
  //printf("Alloc initial buffer, size: %d bytes\n", bufferSize);
  int* buffer_ptr = (int*) malloc (bufferSize);
  if (buffer_ptr == NULL) {
    return 0;
  }

  int i = 0;
	char line[1024];
  //printf("Reading first line\n");
  while (fgets(line, 1024, handle)) {
    const char* tok;
    tok = strtok(line, " ,\n");
    while (tok != NULL) {
      buffer_ptr[i] = atoi(tok);
      i++;
      if (sizeof(int)*i >= bufferSize) {
        //printf("Read %d integers, resize buffer\n", i);        
        if(!double_buffersize(&bufferSize, (void**)&buffer_ptr)) {
          return NULL;
        }
      }
      tok = strtok (NULL, " ,\n");
    }
  }
  *valuesRead = i;
  return buffer_ptr;
}

/* Arg, complete COPY-PASTE of above, just changed to read doubles */
double* read_csv_doubles(FILE* handle, int* valuesRead) {
  int bufferSize = INIT_BUFFER_SIZE;
  // printf("Alloc initial buffer, size: %d bytes\n", bufferSize);
  double* buffer_ptr = (double*) malloc (bufferSize);
  if (buffer_ptr == NULL) {
    return NULL;
  }

  int i = 0;
	char line[1024];
  //printf("Reading first line\n");
  while (fgets(line, 1024, handle)) {
    const char* tok;
    tok = strtok(line, " ,\n");
    while (tok != NULL) {
      buffer_ptr[i] = atof(tok);
      i++;
      if (sizeof(double)*i >= bufferSize) {
        //printf("Read %d integers, resize buffer\n", i);        
        if(!double_buffersize(&bufferSize, (void**)&buffer_ptr)) {
          return NULL;
        }
      }
      tok = strtok (NULL, " ,\n");
    }
  }
  *valuesRead = i;
  return buffer_ptr;
}

int* readIntVecFile(int* valuesRead, char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error reading %s: %d (%s)\n", filename, errno, strerror(errno));
    exit(0);
  }

  int* res = read_csv_ints(file, valuesRead);
  if (res == NULL) {
    printf("readIntVecFile: Error reading file '%s'\n", filename);
    fclose(file);
    exit(0);
  }
  fclose(file);
  return res;
}

double* readDoubleVecFile(int* valuesRead, char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error reading %s: %d (%s)\n", filename, errno, strerror(errno));
    exit(0);
  }

  double* res = read_csv_doubles(file, valuesRead);
  if (res == NULL) {
    printf("readDoubleVecFile: Error reading file '%s'\n", filename);
    fclose(file);
    exit(0);
  }
  fclose(file);
  return res;
}
