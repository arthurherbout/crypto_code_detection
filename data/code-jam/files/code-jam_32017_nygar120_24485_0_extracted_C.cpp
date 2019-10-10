#include <iostream>
#include <gmpxx.h>
#define PREC 1000

using namespace std;

int tab[1000001];
int ns;
int cs;

void ajoute(unsigned long int a)
{
if (cs == -1)
{
    int f;

    if (tab[a] == -1)
        {tab[a] = a;}

    cs = tab[a];

} else {

if (tab[a] == cs)
    return;

ns--;

if (tab[a] == -1)
    {tab[a] = cs; return;}

int f = tab[a];
for (int i=0; i<1000000; i++)
    if(tab[i] == f)
        tab[i] = cs;

}
}

int main()
{
    int C;
    mpz_t x, y, A, B, P;

    mpz_init(x);
    mpz_init(y);
    mpz_init(A);
    mpz_init(B);
    mpz_init(P);

    cin >> C;
    for (int i=1;i<=C;i++)
    {
        cin >> A;
        cin >> B;
        cin >> P;

        for (int j=0; j<1000000; j++)
            tab[j] = -1;

        mpz_sub(y, B, A);
        ns = mpz_get_ui(y);
        ns++;

        if (mpz_probab_prime_p(P, 10) == 0)
            mpz_nextprime(P, P);

        while (mpz_cmp(P,B)<0)
        {
//            cout << "avec " << P << endl;
//            system("pause");
            cs = -1;
            mpz_set(x, A);
//            cout << "-go- " << A << ":" << B << ":" << x <<endl;

            while (mpz_cmp(x,B)<=0)
            {
            //cout << "---- " << x << endl;
                if (mpz_divisible_p(x, P) != 0)
                {
                    mpz_sub(y, x, A);
                    ajoute(mpz_get_ui(y));
                   // cout << x << "divisible par " << P << " : " << ns << endl;
                }
                mpz_add_ui(x, x, 1);
            }
            mpz_nextprime(P, P);
        }

        cout << "Case #" << i <<": " << ns << endl;
    }


	return 0;
}
