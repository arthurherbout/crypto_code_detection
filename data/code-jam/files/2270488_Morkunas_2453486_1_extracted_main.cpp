#include <iostream>
#include <fstream>
using namespace std;


int main()
{

    ifstream input("A-large.in");
    ofstream out ("A-large.out");
    int n;
    char a[4][4];
    input >> n ;
    int p=0;
    while (p!=n)
    {

        for (int i =0; i < 4; i++)
        {
            for (int y = 0 ; y < 4; y++)
            {
                input >> a[i][y];


            }

        }


        int f=0;


        for (int i =0; i < 4; i++)
        {
            for (int y = 0 ; y < 4; y++)
            {
                if (a[i][y]== '.')
                {
                    f++;
                }

            }

        }



        if ((a[0][0]=='X') && (a[0][1]=='X') &&(a[0][2]=='X') && (a[0][3]=='X')) // 1 eilute XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[1][0]=='X') && (a[1][1]=='X') &&(a[1][2]=='X') && (a[1][3]=='X'))// 2 eilute XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[2][0]=='X') && (a[2][1]=='X') &&(a[2][2]=='X') && (a[2][3]=='X'))// 3 eilute XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[3][0]=='X') && (a[3][1]=='X') &&(a[3][2]=='X') && (a[3][3]=='X')) // 4 eilute XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][0]=='X')&&(a[1][0]=='X')&&(a[2][0]=='X')&&(a[3][0]=='X')) // 1 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][1]=='X')&&(a[1][1]=='X')&&(a[2][1]=='X')&&(a[3][1]=='X'))// 2 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][2]=='X')&&(a[1][2]=='X')&&(a[2][2]=='X')&&(a[3][2]=='X'))// 3 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='X')&&(a[1][3]=='X')&&(a[2][3]=='X')&&(a[3][3]=='X'))// 4 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][0]=='X') && (a[0][1]=='X') &&(a[0][2]=='X') && (a[0][3]=='T')) // 1 eilute XXXT
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[1][0]=='X') && (a[1][1]=='X') &&(a[1][2]=='X') && (a[1][3]=='T'))// 2 eilute XXXT
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[2][0]=='X') && (a[2][1]=='X') &&(a[2][2]=='X') && (a[2][3]=='T'))// 3 eilute XXXT
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[3][0]=='X') && (a[3][1]=='X') &&(a[3][2]=='X') && (a[3][3]=='T')) // 4 eilute XXXT
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][0]=='X') && (a[0][1]=='X') &&(a[0][2]=='T') && (a[0][3]=='X')) // 1 eilute XXTX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[1][0]=='X') && (a[1][1]=='X') &&(a[1][2]=='T') && (a[1][3]=='X'))// 2 eilute XXTX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[2][0]=='X') && (a[2][1]=='X') &&(a[2][2]=='T') && (a[2][3]=='X'))// 3 eilute XXTX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[3][0]=='X') && (a[3][1]=='X') &&(a[3][2]=='T') && (a[3][3]=='X')) // 4 eilute XXTX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][0]=='X') && (a[0][1]=='T') &&(a[0][2]=='X') && (a[0][3]=='X')) // 1 eilute XTXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[1][0]=='X') && (a[1][1]=='T') &&(a[1][2]=='X') && (a[1][3]=='X'))// 2 eilute XTXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[2][0]=='X') && (a[2][1]=='T') &&(a[2][2]=='X') && (a[2][3]=='X'))// 3 eilute XTXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[3][0]=='X') && (a[3][1]=='T') &&(a[3][2]=='X') && (a[3][3]=='X')) // 4 eilute XTXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }



        else if ((a[0][0]=='T') && (a[0][1]=='X') &&(a[0][2]=='X') && (a[0][3]=='X')) // 1 eilute TXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[1][0]=='T') && (a[1][1]=='X') &&(a[1][2]=='X') && (a[1][3]=='X'))// 2 eilute TXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[2][0]=='T') && (a[2][1]=='X') &&(a[2][2]=='X') && (a[2][3]=='X'))// 3 eilute TXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[3][0]=='T') && (a[3][1]=='X') &&(a[3][2]=='X') && (a[3][3]=='X')) // 4 eilute TXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }



        else if ((a[0][0]=='T')&&(a[1][0]=='X')&&(a[2][0]=='X')&&(a[3][0]=='X')) // 1 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][1]=='T')&&(a[1][1]=='X')&&(a[2][1]=='X')&&(a[3][1]=='X'))// 2 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][2]=='T')&&(a[1][2]=='X')&&(a[2][2]=='X')&&(a[3][2]=='X'))// 3 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='T')&&(a[1][3]=='X')&&(a[2][3]=='X')&&(a[3][3]=='X'))// 4 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][0]=='X')&&(a[1][0]=='T')&&(a[2][0]=='X')&&(a[3][0]=='X')) // 1 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][1]=='X')&&(a[1][1]=='T')&&(a[2][1]=='X')&&(a[3][1]=='X'))// 2 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][2]=='X')&&(a[1][2]=='T')&&(a[2][2]=='X')&&(a[3][2]=='X'))// 3 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='X')&&(a[1][3]=='T')&&(a[2][3]=='X')&&(a[3][3]=='X'))// 4 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][0]=='X')&&(a[1][0]=='X')&&(a[2][0]=='T')&&(a[3][0]=='X')) // 1 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][1]=='X')&&(a[1][1]=='X')&&(a[2][1]=='T')&&(a[3][1]=='X'))// 2 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][2]=='X')&&(a[1][2]=='X')&&(a[2][2]=='T')&&(a[3][2]=='X'))// 3 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='X')&&(a[1][3]=='X')&&(a[2][3]=='T')&&(a[3][3]=='X'))// 4 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }




        else if ((a[0][0]=='X')&&(a[1][0]=='X')&&(a[2][0]=='X')&&(a[3][0]=='T')) // 1 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][1]=='X')&&(a[1][1]=='X')&&(a[2][1]=='X')&&(a[3][1]=='T'))// 2 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][2]=='X')&&(a[1][2]=='X')&&(a[2][2]=='X')&&(a[3][2]=='T'))// 3 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='X')&&(a[1][3]=='X')&&(a[2][3]=='X')&&(a[3][3]=='T'))// 4 stulpelis XXXX
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }



        //----------------------------------------------- O raidytes

        else if ((a[0][0]=='O') && (a[0][1]=='O') &&(a[0][2]=='O') && (a[0][3]=='O')) // 1 eilute OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[1][0]=='O') && (a[1][1]=='O') &&(a[1][2]=='O') && (a[1][3]=='O'))// 2 eilute OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[2][0]=='O') && (a[2][1]=='O') &&(a[2][2]=='O') && (a[2][3]=='O'))// 3 eilute OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[3][0]=='O') && (a[3][1]=='O') &&(a[3][2]=='O') && (a[3][3]=='O')) // 4 eilute OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }


        else if ((a[0][0]=='O')&&(a[1][0]=='O')&&(a[2][0]=='O')&&(a[3][0]=='O')) // 1 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][1]=='O')&&(a[1][1]=='O')&&(a[2][1]=='O')&&(a[3][1]=='O'))// 2 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][2]=='O')&&(a[1][2]=='O')&&(a[2][2]=='O')&&(a[3][2]=='O'))// 3 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='O')&&(a[1][3]=='O')&&(a[2][3]=='O')&&(a[3][3]=='O'))// 4 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }


        else if ((a[0][0]=='O') && (a[0][1]=='O') &&(a[0][2]=='O') && (a[0][3]=='T')) // 1 eilute OOOT
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[1][0]=='O') && (a[1][1]=='O') &&(a[1][2]=='O') && (a[1][3]=='T'))// 2 eilute OOOT
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[2][0]=='O') && (a[2][1]=='O') &&(a[2][2]=='O') && (a[2][3]=='T'))// 3 eilute OOOT
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[3][0]=='O') && (a[3][1]=='O') &&(a[3][2]=='O') && (a[3][3]=='T')) // 4 eilute OOOT
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }


        else if ((a[0][0]=='O') && (a[0][1]=='O') &&(a[0][2]=='T') && (a[0][3]=='O')) // 1 eilute OOTO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[1][0]=='O') && (a[1][1]=='O') &&(a[1][2]=='T') && (a[1][3]=='O'))// 2 eilute OOTO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[2][0]=='O') && (a[2][1]=='O') &&(a[2][2]=='T') && (a[2][3]=='O'))// 3 eilute OOTO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[3][0]=='O') && (a[3][1]=='O') &&(a[3][2]=='T') && (a[3][3]=='O')) // 4 eilute OOTO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }


        else if ((a[0][0]=='O') && (a[0][1]=='T') &&(a[0][2]=='O') && (a[0][3]=='O')) // 1 eilute OTOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[1][0]=='O') && (a[1][1]=='T') &&(a[1][2]=='O') && (a[1][3]=='O'))// 2 eilute OTOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[2][0]=='O') && (a[2][1]=='T') &&(a[2][2]=='O') && (a[2][3]=='O'))// 3 eilute OTOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[3][0]=='O') && (a[3][1]=='T') &&(a[3][2]=='O') && (a[3][3]=='O')) // 4 eilute OTOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }



        else if ((a[0][0]=='T') && (a[0][1]=='O') &&(a[0][2]=='O') && (a[0][3]=='O')) // 1 eilute TOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[1][0]=='T') && (a[1][1]=='O') &&(a[1][2]=='O') && (a[1][3]=='O'))// 2 eilute TOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[2][0]=='T') && (a[2][1]=='O') &&(a[2][2]=='O') && (a[2][3]=='O'))// 3 eilute TOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[3][0]=='T') && (a[3][1]=='O') &&(a[3][2]=='O') && (a[3][3]=='O')) // 4 eilute TOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }



        else if ((a[0][0]=='T')&&(a[1][0]=='O')&&(a[2][0]=='O')&&(a[3][0]=='O')) // 1 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][1]=='T')&&(a[1][1]=='O')&&(a[2][1]=='O')&&(a[3][1]=='O'))// 2 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][2]=='T')&&(a[1][2]=='O')&&(a[2][2]=='O')&&(a[3][2]=='O'))// 3 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='T')&&(a[1][3]=='O')&&(a[2][3]=='O')&&(a[3][3]=='O'))// 4 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }


        else if ((a[0][0]=='O')&&(a[1][0]=='T')&&(a[2][0]=='O')&&(a[3][0]=='O')) // 1 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][1]=='O')&&(a[1][1]=='T')&&(a[2][1]=='O')&&(a[3][1]=='O'))// 2 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][2]=='O')&&(a[1][2]=='T')&&(a[2][2]=='O')&&(a[3][2]=='O'))// 3 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='O')&&(a[1][3]=='T')&&(a[2][3]=='O')&&(a[3][3]=='O'))// 4 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }


        else if ((a[0][0]=='O')&&(a[1][0]=='O')&&(a[2][0]=='T')&&(a[3][0]=='O')) // 1 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][1]=='O')&&(a[1][1]=='O')&&(a[2][1]=='T')&&(a[3][1]=='O'))// 2 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][2]=='O')&&(a[1][2]=='O')&&(a[2][2]=='T')&&(a[3][2]=='O'))// 3 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='O')&&(a[1][3]=='O')&&(a[2][3]=='T')&&(a[3][3]=='O'))// 4 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }




        else if ((a[0][0]=='O')&&(a[1][0]=='O')&&(a[2][0]=='O')&&(a[3][0]=='T')) // 1 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][1]=='O')&&(a[1][1]=='O')&&(a[2][1]=='O')&&(a[3][1]=='T'))// 2 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][2]=='O')&&(a[1][2]=='O')&&(a[2][2]=='O')&&(a[3][2]=='T'))// 3 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='O')&&(a[1][3]=='O')&&(a[2][3]=='O')&&(a[3][3]=='T'))// 4 stulpelis OOOO
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }




        //======================== Istrizaines

        else if ((a[0][0]=='X')&&(a[1][1]=='X') && (a[2][2]=='X') && (a[3][3]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][0]=='T')&&(a[1][1]=='X') && (a[2][2]=='X') && (a[3][3]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][0]=='X')&&(a[1][1]=='T') && (a[2][2]=='X') && (a[3][3]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][0]=='X')&&(a[1][1]=='X') && (a[2][2]=='T') && (a[3][3]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][0]=='X')&&(a[1][1]=='X') && (a[2][2]=='X') && (a[3][3]=='T'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][3]=='X')&&(a[1][2]=='X') && (a[2][1]=='X') && (a[3][0]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='T')&&(a[1][2]=='X') && (a[2][1]=='X') && (a[3][0]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='X')&&(a[1][2]=='T') && (a[2][1]=='X') && (a[3][0]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='X')&&(a[1][2]=='X') && (a[2][1]=='T') && (a[3][0]=='X'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }
        else if ((a[0][3]=='X')&&(a[1][2]=='X') && (a[2][1]=='X') && (a[3][0]=='T'))
        {
            out<< "Case #" << p+1 << ": " << "X won" << endl;
        }


        else if ((a[0][0]=='O')&&(a[1][1]=='O') && (a[2][2]=='O') && (a[3][3]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][0]=='T')&&(a[1][1]=='O') && (a[2][2]=='O') && (a[3][3]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][0]=='O')&&(a[1][1]=='T') && (a[2][2]=='O') && (a[3][3]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][0]=='O')&&(a[1][1]=='O') && (a[2][2]=='T') && (a[3][3]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][0]=='O')&&(a[1][1]=='O') && (a[2][2]=='O') && (a[3][3]=='T'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }


        else if ((a[0][3]=='O')&&(a[1][2]=='O') && (a[2][1]=='O') && (a[3][0]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='T')&&(a[1][2]=='O') && (a[2][1]=='O') && (a[3][0]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='O')&&(a[1][2]=='T') && (a[2][1]=='O') && (a[3][0]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='O')&&(a[1][2]=='O') && (a[2][1]=='T') && (a[3][0]=='O'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
        else if ((a[0][3]=='O')&&(a[1][2]=='O') && (a[2][1]=='O') && (a[3][0]=='T'))
        {
            out<< "Case #" << p+1 << ": " << "O won" << endl;
        }
                else if (f>0)
                {
                    out<< "Case #" << p+1 << ": "<< "Game has not completed" << endl;

                }
                else
                {
                    out<< "Case #" << p+1 << ": "<< "Draw" << endl;

                }





f=0;
        p++;
    }

    // (a[0][0]=='X')&&

    input.close();
    out.close();


    return 0;
}
