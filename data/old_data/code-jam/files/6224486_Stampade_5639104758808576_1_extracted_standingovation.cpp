//
//  standingovation.cpp
//  Google Code Jam 2015
//
//  Created by sameep sheth on 4/11/15.
//  Copyright (c) 2015 SameepsCoolUtilities. All rights reserved.
//

#include "commonfiles.h"

int friendsneeded(string &x);

int friendsinaudience(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "supply file plz"<<endl;
        return 0;
    }
    
    ifstream input(argv[2]);
    ofstream output("/Users/sameepsheth/Downloads/output1");
    if (!input) {
        cout<<"can't open file"<<argv[2]<<endl;
        return 0;
    }
    
    int testcases;
    int maxshyness;
    string shynessstring;
    input>>testcases;
    for (int i = 0; i < testcases; i++) {
        input>>maxshyness;
        input>>shynessstring;
        output<<"Case #"<<i+1<<": "<<friendsneeded(shynessstring)<<endl;
    }
    return 0;
}

int friendsneeded(string &x)
{
    int count = x[0] - '0';
    int cur;
    int friends = 0;
    int extrafriends = 0;
    for(int i = 1; i < x.length(); i++) {
        cur = x[i]-'0';
        if (cur > 0 && count < i) {
            extrafriends = i - count;
            friends += extrafriends;
            count += extrafriends;
        }
        count += cur;
    }
    return friends;
}