#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
        char string[128] = {0};
        int cnt = 1;
        char refstring[1000];
        int i, counter;
        for (i = 0; i < 1000; i++)
                refstring[i] = i;


        char *delim = " \n\t";

        int testcasenumber = 0;
//      printf("Enter the number\n");
        scanf("%d", &testcasenumber);
//      printf("test case number : %d\n", testcasenumber);
        getchar();
        for (counter = 0; counter < testcasenumber; counter++)
        {
                if (fgets(string, 128, stdin))
                {
                        string[strlen(string)] = 0;
//                      printf("line %d, string is %s\n", cnt, string);
//                      cnt++;


                        char * shyest = NULL;
                        char * num = NULL;
                        int scnt = 0;
                        shyest = strtok(string, delim);
                        if (shyest);
//                              printf("The shyest level is %d\n", atoi(shyest));

                        num  = strtok(NULL, delim);
                        int totcnt = 0;
                        int reqcnt =0;
                        int presentcnt = 0;
                        if (num)
                        {
                                for (i = 1; i < strlen(num); i++)
                                {
                                        //printf("Shyness %d : people %d\n", scnt, (num[i] - '0'));
                                        //scnt++;
                                        reqcnt = i;
                                        presentcnt += (num[i-1] - '0');
                                        while (presentcnt < reqcnt)
                                        {
                                                totcnt++;
                                                presentcnt++;
                                        }

                                }

                                printf("case #%d: %d\n", cnt, totcnt);
                                cnt++;


                        }
                }
        }



        return 0;
}
