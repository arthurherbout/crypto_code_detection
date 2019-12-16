#include <iostream>
#include <fstream>
#include <vector>


using namespace std;



int main()
{

    ifstream sisf;
    sisf.open("C.in");
    ofstream valf;
    valf.open("C.out");


    int C, N, K, B, T;

    sisf >> C;

    for(int u = 1; u<=C; u++)
    {
        int pos[55], vel[55];
        bool chi[55];

        sisf >> N >> K >> B >> T;
        int temp = N;
        int lift = 0;
        int sol = 0;

        for(int i = 1; i<=N; i++)
            sisf >> pos[i];
        for(int i = 1; i<=N; i++)
            sisf >> vel[i];

        for(int i = N; i>=1; i--)
        {
            if(pos[i]+vel[i]*T>=B)
                chi[i]=1;
            else chi[i]=0;
        }
        while(1)
        {

            if(chi[temp])
            {

                K=K-1;
                sol = sol + lift;
            }
            else lift++;

            if(K==0)break;

            temp--;
            if(temp<1)
            {
                sol=-1;
                break;
            }

        }
        if(sol==-1)
            valf << "Case #" << u << ": IMPOSSIBLE\n";
        else valf << "Case #" << u << ": "<< sol <<"\n";
    }

    return 0;
}
