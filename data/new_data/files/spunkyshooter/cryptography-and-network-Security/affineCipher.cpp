/*
Author: Manoj Kumar R
Date: 10-Sept-2019

#Affine cipher
1.Has Two keys a and b (integers)
    - let a is multiplicative key.  a belongs to [1,m]
    - b is additive key.  b belongs to [1,m]
    - let m is alphabets_set_size
    - then a should not have any common factor with m. i.e gcd(a,m) = 1 
        - other wise, Encryption and Decryption won't be one-one, which is the basic condition in cryptography
            - And inverse mod m doesn't exist  
    - (remember size of alphabets changes, if comma, fullstop,space is considered).
    - for simplicity take english alphabets (hence 26).

2. Algorithm
    - C = E(P*a + b)% m
    - P = ((C-b)*a^-1)% m



Some Imp Notes:
    - mapping is one -to-one
    - Code can be still over engineered like storing just alphaMappedToKey and that needs to find the key by value while decrypting.
        - So keeping in mind that reader's time , algorithm is coded
    - This cipher is weak.
*/
#include<iostream>
#include<map>
using namespace std;

//our AplhabetSet consists lowercase, uppercase and space 
//size = 53 ,primenumber . Hence multiplicative_key can have 52 values. (except number 53)
const string AlphabetSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ";
map<char,int> alphaToIndex; 

void mapAlphabetsToIndex(string AlphabetSet){
    for(int i = 0; i<(int)AlphabetSet.size() ; i++){
        alphaToIndex[AlphabetSet[i]] = i;
    }
    //if we use just array, every time when we need integer for an alphabet, we need to search the element in array.
    // at most m operation should be done. i.e O(m) ex: 53 operations
    //If we store in map with indexed mapped. then atmost we just need to do just log(m) operations ex: log(53) base 2 = 6.
    //And this scales up nicely when alphabet size increases.
}

int gcd(int a,int b,int &x, int &y)
{
    if(a == 0){
        x = 0; y = 1;
        return b;
    }
    int x1,y1;
    int d = gcd(b%a,a,x1,y1);
    x = y1 - (b/a)*x1;
    y = x1;
    return d;
}
int invModM(int a,int m){
    int d,x,y;
    //linear diophantine equation: ax + by = c.
    //let b = m and c = 1, then ax + my = 1, now take mod on both sides
    // ax = 1 mod m (congruent)
    //then using extended euclidean algorithm, find the solution for linear diophantine equation, ax + my = 1
    int g = gcd(a,m,x,y);
    //assuming that a and m are coprime. 
    //(hence not performing any checks here) such that whether g is 1 or not
    x =(x%m + m)%m;
     // x can be -ve number. hence we should make it positive since a^-1 shouldn't be negative
    return x;
}
//Encrypts Plain text  to Cipher text
//@params
//plainText = Plain text
//a = multiplicative key
//b = additive key
//m = alphabetset size ex: (ABC..Z) = 26 , (ABC..Zabc..z) = 52 , (ABC..Z_) = 27(along with space)
string encryption(string plainText,int a,int b,int m){

    string cipherText = "";
    //substituting, fo easier understanding i am storing in different variable cipherText
    for (int i=0;i<(int)plainText.size(); i++){
        int index = (((alphaToIndex[ plainText[i] ] % m )* (a % m) % m) + b % m ) % m ;
        if(index < 0) index += m;
        cipherText += AlphabetSet[index];
    }
    return cipherText;
}

//decrypts cipher text to plain text.
//@params
//plainText = Plain text
//a = multiplicative key
//b = additive key
//m = alphabetset size 
string decryption(string cipherText,int a,int b,int m){

    string plainText = "";
    int aInverseModm = invModM(a,m);
    // cout<<aInverseModm<<'\n';
    for (int i=0;i<(int)cipherText.size(); i++){
        char c = cipherText[i];
        int index = ( (((alphaToIndex[c] % m) - b%m)%m )* aInverseModm ) % m ;
        if(index < 0) index += m; //handling the -ve case, we can do this in above line also .add m and take mod m
        plainText += AlphabetSet[index];
    }
    return plainText;
}



int main(){
   
    
    mapAlphabetsToIndex(AlphabetSet);
    int multiplicativeKey = 7,additiveKey = 8,alphabetSetSize = 53;
    string plainText; //P.T
    cout<<"Enter plainText: ";
    cin>>plainText;
    string cipherText = encryption(plainText,multiplicativeKey,additiveKey,alphabetSetSize);

    cout<<"Ciphertext: "<<cipherText<<'\n';
    string decryptedText = decryption(cipherText,multiplicativeKey,additiveKey,alphabetSetSize);
    cout<<"decryptedText: "<<decryptedText<<'\n';
    
}