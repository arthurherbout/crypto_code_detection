#include <fstream>
#include <cstring>
using namespace std;
typedef long long lld;
char name[1000002];
bool TFO[100002];
bool Vow(char a)
{
    return (a=='a' || a=='e' || a=='i' || a=='o' || a=='u');
}
int main ()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    lld i,j,ii,jj,ind;
    lld tests,t,len,k,last=0;
    lld vowel,consonant;
    lld nbeg,nend,ans;

    for (i=0;i<100002;i++)
    {
        TFO[i]=false;
    }

    scanf("%I64d",&tests);
    for (t=1;t<=tests;t++)
    {
        scanf("%s",name);
        len=strlen(name);
        scanf("%I64d",&k);

        vowel=0;
        consonant=0;
        last=0;
        nbeg=0;
        nend=k-1;
        ans=0;

        for (j=0;j<k;j++)
        {
            if (Vow(name[j]))
            {
                vowel++;
            }
            else
            {
                consonant++;
            }
        }

        while (nend<len)
        {
            if (vowel>0)
            {
                nend++;
                nbeg=nend-k+1;

                if (Vow(name[nend]))
                {
                    vowel++;
                }
                else
                {
                    consonant++;
                }
                if (Vow(name[nbeg-1]))
                {
                    vowel--;
                }
                else
                {
                    consonant--;
                }
                continue;
            }
            ans+=(len-nend)*(nbeg-last+1);

            nend++;
            nbeg=nend-k+1;
            last=nbeg;

            if (Vow(name[nend]))
            {
                vowel++;
            }
            else
            {
                consonant++;
            }
            if (Vow(name[nbeg-1]))
            {
                vowel--;
            }
            else
            {
                consonant--;
            }
        }
        printf("Case #%I64d: %I64d\n",t,ans);
    }
}
