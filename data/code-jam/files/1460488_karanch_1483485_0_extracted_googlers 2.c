#include<stdio.h>
#include<string.h>

int main()
{
     int t,i,caseno=1;
     char input[102];
     char map[26] = {'y','h','e','s','o','c','v','x','d','u','i','g','l','b','k','r','z','t','n','w','j','p','f','m','a','q'};
     
     gets(input);
     t = atoi(input);
     while(t-->0)
     {
                 gets(input);
                 //for(i=0;i<strlen(input);i++)
//                 {
//                                             if(input[i]!=' ')
//                                                    map[input[i]-'a'] = output[i];             
//                 }
                 printf("Case #%d: ",caseno++);
                 for(i=0;i<strlen(input);i++)
                 {
                                             if(input[i]!=' ')
                                             {
                                                            if(input[i]<=90 && input[i] >=65)
                                                            {
                                                                           input[i] += 'a' - 'A';                              
                                                            }
                                                            printf("%c",map[input[i]-'a']);
                                             }
                                             else
                                                            printf(" ");
                 }

                // for(i=0;i<26;i++)
//                 {
//                             printf("\n%c->%c",('a'+i),map[i]);    
//                 }
                 printf("\r\n");
                 
                 
                 free(input);
     }
     return 0;
}
     
