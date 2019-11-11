#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[])
{
    ifstream input("B-small-attempt0.in");
    ofstream output("B-small-practice.ou.txt");

    
    int casi;
    input >> casi;
    
    for(int i = 0; i < casi; i++)
    {
        int combinazioni = 0;
        int a, b, k;
        input >> a;
        input >> b;
        input >> k;
        
        for(int y = 0; y < k; y++)
        {
            for(int j = 0; j < a; j++)
            {
                for(int z = 0; z < b; z++)
                {
                    if((j & z) == y)
                    {
                        combinazioni++;
                    }
                }
            }
        }
        cout << i << endl;
        output << "Case #" << i + 1<< ": " <<combinazioni << endl;
    }
}
