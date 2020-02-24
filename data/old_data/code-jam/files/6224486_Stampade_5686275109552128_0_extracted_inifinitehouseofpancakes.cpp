//
//  inifinitehouseofpancakes.cpp
//  Google Code Jam 2015
//
//  Created by sameep sheth on 4/11/15.
//  Copyright (c) 2015 SameepsCoolUtilities. All rights reserved.
//

#include "commonfiles.h"


int optimize_pancake_allocation(vector<int> d,int depth, int prev_min);

int infinitehouseofpancakes(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "supply file plz"<<endl;
        return 0;
    }
    
    ifstream input(argv[2]);
    ofstream output("/Users/sameepsheth/Downloads/output2");
    if (!input) {
        cout<<"can't open file"<<argv[2]<<endl;
        return 0;
    }
    
    int testcases;
    int total_diners;
    int current_diner_pnacakes;
    vector<int> diners;

    input>>testcases;

    for (int i = 0; i < testcases; i++) {
        input>>total_diners;
        
        for (int j = 0; j < total_diners; j++) {
            input>>current_diner_pnacakes;
            diners.push_back(current_diner_pnacakes);
        }
        output<<"Case #"<<i+1<<": "<<optimize_pancake_allocation(diners, 0, 99999999)<<endl;
        diners.clear();
    }
    return 0;
}

void findmax(vector<int>& d, int &max, int &index) {
    max = -1;
    index = -1;
    for (int i = 0 ; i < d.size(); i++) {
        if (d[i] > max) {
            max = d[i];
            index = i;
        }
    }
}

int min_of_two(int a, int b)
{
    if ( a < b)
        return a;
    else
        return b;
}

int optimize_pancake_allocation(vector<int> d,int depth, int prev_min)
{
    int max = -1;
    int index = -1;
    findmax(d, max, index);
    int new_min = min_of_two((max+depth), prev_min);
    
    if (max == 1 || max == 2 || max == 3){
        return min_of_two( (depth + max), prev_min);
    }
    
    int mid = max / 2;
    
    int current_min = prev_min;
    d.push_back(1);
    int last = (int)(d.size() - 1);
    for (int j = 2; j < mid + 1; j++) {
        d[index] = j;
        d[last] = max - j;
        current_min = min_of_two(current_min, optimize_pancake_allocation(d, depth+1, new_min));
    }
    return current_min;
}