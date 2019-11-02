#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int palindrome(char* val)
{
    int i;
    for(i=0;i<strlen(val)/2;i++)
    {
        if(val[i]!=val[strlen(val)-i-1])
            return 0;
    }
    return 1;
}

int main()
{
    FILE* entree = NULL;
    FILE* sortie = NULL;
    int nb_case;
    int A,B;
    int k;
    int i,j,res=0;
    char test[16];
    char* teste;

    entree = fopen("C-small-attempt2.in", "r");
    sortie = fopen("output.txt", "w");

    if (entree != NULL&&sortie != NULL)
    {
        fscanf(entree,"%d",&nb_case);
        //nb_case=1;
        for(k=0;k<nb_case;k++)
        {
            fscanf(entree,"%d %d",&A,&B);
            printf("\n\n%d %d\n",A,B);
            res=0;
            for(i=0;i<B+1;i++)
            {
                j=i*i;
                sprintf(teste,"%d",j);
                //puts(teste);
                sprintf(test,"%d",i);
                if ((palindrome(test)==1)&&(palindrome(teste)==1)&&(j<=B)&&(j>=A))
                    //printf("\nres=%s",test);
                    res++;
                //test=NULL;
               // printf("est palind=%d\n\n",palindrome(test));
            }
            fprintf(sortie, "Case #%d: %d",k+1,res);
            if(k<nb_case-1)
                fprintf(sortie, "\n");
        }
    }
    fclose(entree);
    fclose(sortie);
    return 0;
}
