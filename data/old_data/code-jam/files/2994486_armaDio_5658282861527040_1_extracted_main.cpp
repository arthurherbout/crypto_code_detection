//
//  main.cpp
//  New Lottery Game
//
//  Created by Dylan Stenico on 03/05/14.
//  Copyright (c) 2014 Dylan Stenico. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[])
{
    ifstream input("/Users/dylanstenico/Documents/School/InformationTechnology/2014/GoogleCodeJam/New Lottery Game/B-large.in.txt");
    ofstream output("/Users/dylanstenico/Documents/School/InformationTechnology/2014/GoogleCodeJam/New Lottery Game/B-large-practice.ou.txt");

    
    int cases;
    input >> cases;
    
    for(int i = 0; i < cases; i++)
    {
        long combinations = 0;
        unsigned int a, b, k;
        input >> a;
        input >> b;
        input >> k;
        
        if((a > k && b <= k) || (b > k && a <= k))
        {
            combinations = a < b? a * k : b * k;
        }
        else
        {
            
            combinations = a * k + b * k - k * k;

        }
        
        
        combinations = a*b;
        
        cout << i << endl;
        output << "Case #" << i + 1<< ": " <<combinations << endl;
    }
}

/*
for(unsigned int y = 0; y < k; y++)
{
    for(unsigned int j = 0; j < a; j++)
    {
        if((j & y) >= y)
        {
            for(unsigned int z = 0; z < b; z++)
            {
                if((j & z) == y)
                {
                    combinations++;
                }
            }
        }
    }
}
 
 
 for(unsigned int j = 0; j < a; j++)
 {
 for(unsigned int z = 0; z < b; z++)
 {
 if((j & z) <=k)
 {
 combinations++;
 }
 }
 }
*/