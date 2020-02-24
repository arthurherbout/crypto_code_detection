//
//  main.cpp
//  cppsample
//
//  Created by 刘颖珊 on 2017/4/6.
//  Copyright © 2017年 刘颖珊. All rights reserved.
//

#include <iostream>

using namespace std;


long long tidy_number(long long max_number)
{
    long long x = max_number;
    long long exp = 1;
    long long result = max_number;
    long long negative = 0;
    while (x > 0)
    {
        long long last_number = x % 10 + negative;
        long long second_last_number = x % 100 / 10;
        exp *= 10;
        if (second_last_number > last_number)
        {
            negative = -1;
            result = max_number / exp * exp - 1;
        }
        else
        {
            negative = 0;
        }
        x /= 10;
    }
    return result;
}

void problemB()
{
    freopen("b_large_in.txt","r",stdin);
    freopen("b_large_out.txt","w",stdout);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
    {
        long long x;
        cin >> x;
        cout << "Case #" << i + 1 << ": " << tidy_number(x) << endl;
    }
}

void calculate_distance(long long n, long long k)
{
//    if (k == 0)
//    {
//        cout << n << " " << n << endl;
//        return;
//    }
//    if (k == 1)
//    {
//        cout << n / 2 << " " << (n - 1) / 2 << endl;
//        return;
//    }
    
    long long big_count_weight = 1, small_count_weight = 0;
    long long big = n;
    long long exp = 1;
    while (exp * 2 - 1 < k)
    {
        long long new_big_weight, new_small_weight;
        if (big % 2 == 0)
        {
            new_big_weight = big_count_weight;
            new_small_weight = big_count_weight + small_count_weight + small_count_weight;
        }
        else
        {
            new_big_weight = big_count_weight + big_count_weight + small_count_weight;
            new_small_weight = small_count_weight;
        }
        big_count_weight = new_big_weight;
        small_count_weight = new_small_weight;
        big = big / 2;
        exp = exp * 2;
    }
    
    long long last = k - (exp - 1);
    if (last <= big_count_weight)
    {
        cout << big / 2 << " " << (big - 1) / 2 << endl;
    }
    else
    {
        cout << (big - 1) / 2 << " " << (big - 2) / 2 << endl;
    }
}

void problemC()
{
    freopen("C-large-in.txt","r",stdin);
    freopen("C-large-out.txt","w",stdout);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
    {
        long long n, k;
        cin >> n >> k;
        cout << "Case #" << i + 1 << ": ";
        calculate_distance(n, k);
    }
}

int main(int argc, const char * argv[])
{
//    problemB();
    problemC();
    return 0;
}

