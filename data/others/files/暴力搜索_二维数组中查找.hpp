/*
��ά�����в��� https://www.nowcoder.com/practice/abc3fe2ce8e146608e868a70efebf62e?tpId=13&tqId=11154&rp=1&ru=/ta/coding-interviews&qru=/ta/coding-interviews/question-ranking

��Ŀ��Դ����ָOffer

��Ŀ����
��һ����ά�����У�ÿһ�ж����մ����ҵ�����˳������ÿһ�ж����մ��ϵ��µ�����˳������
�����һ������������������һ����ά�����һ���������ж��������Ƿ��и�������

ʱ�����ƣ�1�� �ռ����ƣ�32768K �ȶ�ָ����629898
����֪ʶ�㣺 ����
*/

#pragma once

#include "../all.h"

class Solution {
public:
    bool Find(int target, vector<vector<int> > array) {
        int i = array.size() - 1;
        int j = 0;

        while ((i >= 0) && j < array[0].size())
        {
            if (array[i][j] > target)
                i--;
            else if (array[i][j] < target)
                j++;
            else
                return true;
        }
        
        return false;
    }
};

int solve() {
    int target = 5;
    vector<vector<int> > array{ { 1,2,3,4 },{ 2,3,4,5 } };

    bool flag = Solution().Find(target, array);
    cout << flag << endl;
    return 0;
}