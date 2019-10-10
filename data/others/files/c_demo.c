#include <stdio.h>
#include <string.h>

#define BLOCK "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" \
              "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" \
              "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" \

void good();
void evil();
              
int main(char** argv, int argc) {
    char *first = BLOCK "+";
    char *second = BLOCK "-";
    
    if (memcmp(first, second, 192) == 0) {
        good();
    }
    else {
        evil();
    }
    
    return 0;
}

void good() {
   puts("good"); 
}

void evil() {
   puts("evil"); 
}
