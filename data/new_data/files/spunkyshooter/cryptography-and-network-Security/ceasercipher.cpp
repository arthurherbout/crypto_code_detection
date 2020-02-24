/*
Author: Manoj kumar R
Date: 10-Sept-2019
Some notes:

- charecter is represented in ascii, i.e A=65,a = 97
- 'a' + 2 = 'c'(i.e 99)
*/
#include<iostream>
using namespace std;

string encryption(string plainText,int key){
    string cipherText;
    if(key < 0) {
        //when key is negative,c++ computes as -(abs(key)%26)
        //in python, you can do simply key%26. doesn't matter -ve or +ve
        key = key%26 + 26;  
    }
    else{
        key %= 26; 
        //this can be ommitted here since this will be done along with the formula.
        //But this handles the overflow error. 
        //ex: key = max large number supported, then in the below formula addition results in overflow error
    }
    for(int i=0;i<(int)plainText.size();i++){
        if(plainText[i]>=97)  //when alphabet is lowercase
            cipherText += (plainText[i] - 97 + key)%26 + 97;
        else //when alphabet is uppercase
            cipherText += (plainText[i] - 65 + key)%26 + 65;
    }
    return cipherText;
}

string decryption(string cipherText,int key){
    string plainText;
    if(key < 0) {
        //when key is negative,c++ computes as -(abs(key)%26)
        //in python, you can do simply key%26. doesn't matter -ve or +ve
        key = key%26 + 26;  
    }
    else{
        key %= 26; 
        //this can be ommitted here since this will be done along with the formula.
        //But this handles the overflow error. 
        //ex: key = max large number supported, then in the below formula addition results in overflow error
    }
    for(int i=0;i<(int)cipherText.size();i++){
        if(cipherText[i]>=97)  //when alphabet is lowercase
            {
                int temp = (cipherText[i] - 97 - key)%26;//this can result in -ve ans
                
                //handles the negative case.remember the plain text is string and ascii code starts from 97
                //therefore if temp is -ve, then we have to add 26. 
                if(temp < 0) temp += 26; 
                plainText += temp + 97;
            }
            
        else //when alphabet is uppercase
        {
                int temp = (cipherText[i] - 65 - key)%26;
                if(temp < 0) temp += 26; 
                plainText += temp + 65;
        }
    }
    return plainText;
}

int main(){
    int key = -1; //by default key = 3. also works for anykey ex: -123, 78...
    string plainText = "aAbBcC"; //P.T
    cout<<"Enter Plaintext: ";
    cin>>plainText;
    string cipherText = encryption(plainText,key);
    cout<<"CipherText: "<<cipherText<<'\n'; //plaintext is ch
     cout<<"DecryptedText: "<<decryption(cipherText,key)<<'\n';
}