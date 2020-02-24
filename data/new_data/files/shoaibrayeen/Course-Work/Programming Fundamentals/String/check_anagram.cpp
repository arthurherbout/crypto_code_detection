#include <stdio.h>
#include <string.h>

void quickSort(char *arr, int si, int ei);
bool areAnagram(char *str1, char *str2)
{
    int n1 = strlen(str1);
    int n2 = strlen(str2);
    if (n1 != n2)
        return false;
    quickSort(str1, 0, n1 - 1);
    quickSort(str2, 0, n2 - 1);
        for (int i = 0; i < n1;  i++)
        if (str1[i] != str2[i])
            return false;
    
    return true;
}

void exchange(char *a, char *b)
{
    char temp;
    temp = *a;
    *a   = *b;
    *b   = temp;
}

int partition(char A[], int si, int ei)
{
    char x = A[ei];
    int i = (si - 1);
    int j;
    
    for (j = si; j <= ei - 1; j++)
    {
        if(A[j] <= x)
        {
            i++;
            exchange(&A[i], &A[j]);
        }
    }
    exchange (&A[i + 1], &A[ei]);
    return (i + 1);
}

void quickSort(char A[], int si, int ei)
{
    int pi;
    if(si < ei)
    {
        pi = partition(A, si, ei);
        quickSort(A, si, pi - 1);
        quickSort(A, pi + 1, ei);
    }
}
int main()
{
    char str1[] = "test";
    char str2[] = "ttes";
    if (areAnagram(str1, str2))
        printf("The two strings are anagram of each other\n");
    else
        printf("The two strings are not anagram of each other\n");
    
    return 0;
}
