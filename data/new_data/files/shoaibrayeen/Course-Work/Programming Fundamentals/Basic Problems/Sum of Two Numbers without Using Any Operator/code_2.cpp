//
//  code_2.cpp
//  Quiz
//
//  Created by Mohd Shoaib Rayeen on 15/04/19.
//  Copyright © 2019 Shoaib Rayeen. All rights reserved.
//


#include <iostream>
using namespace std;

int add(int x, int y) {
    return printf("%*c%*c", x, '\r', y, '\r');
}

int main() {
    int a, b;
    cout << "\nEnter 1st Number\t:\t";
    cin >> a;
    cout << "\nEnter 2nd Number\t:\t";
    cin >> b;
    printf("\nSum\t:\t%d\n" ,  add(a,b) );
    return 0;
}
