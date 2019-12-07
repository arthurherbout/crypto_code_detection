/*
Author: Manoj Kumar R
Date: 11-Sept-2019

#AutoKey System
- Is polyalphabetic cipher
- Improvement over Vigenere cipher.
- Instead of key being repeated, here plaintext will be repeated after key
- Therefore the resulting key will be K = k + repeated PT
- Key legth(m) is small than plaintext length(n). i.e m < n
*/
#include<iostream>
#include<vector>
using namespace std;
#define DB(x) cout<<#x<<": "<<x<<'\n'

class AutoKeySystem {
    const string AlphabetSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string key; 
    
    public:
    string encryption(string plainText,string key){ 
      //Remember key is changed for every message. Since P.T is repeated.
      // hence, we need new key every time. 
      //Assumption: m < n
       int m = (int)key.size() , n = (int)plainText.size(); 
        string cipherText = "";
        for(int i = 0; i < m ; i++){
             cipherText += AlphabetSet[( (plainText[i] - 65) + (key[i] - 65) ) %26];
        }
        int k = 0;
       for(int i = m;i<n; i++){
           cipherText += AlphabetSet[( (plainText[i] - 65) + (plainText[k % n] - 65) ) %26];
           k++;
       }
        return cipherText;
    }

    string decryption(const string cipherText,string fullKey){ 
        //full key = key + repeated plain text. 
        //Doubt: Key already has PT in it,How secure is this? Does this makes sense?
        //Instead of fullKey, If we receive key and painText as parameter. This is foolish.
        
        //Assumption: fullkey length = cipherText length
        int n = (int)cipherText.size();
        string plainText = "";
        int k; //temp variable; numerical index of alphabets. A = 0, B = 1..
        for(int i = 0; i < n ; i++){
            k = ( (cipherText[i] - 65) - (fullKey[i] - 65) ) %26;
            if(k < 0) k += 26;
            plainText += AlphabetSet[k];
        }
        return plainText;
    }

};


int main(){
    
    //Assumption: message is in capital letters
    string plainText,key = "DE"; //P.T
    cout<<"Enter plainText: ";
    cin>>plainText;
    AutoKeySystem HC; //pass the key

    string cipherText = HC.encryption(plainText,key);
    cout<<"Ciphertext: "<<cipherText<<'\n';
    string fullkey = key;

    //Actually this will be provided. Just for simplicity. we are generating on own using previous plaintext
    int index = 0, PTlength = (int)plainText.size();
    for(int i= (int)key.size(); i<cipherText.size() ; i++){
        fullkey += plainText[index % PTlength];
        index ++;
    }
    string decryptedText = HC.decryption(cipherText, fullkey);
    cout<<"decryptedText: "<<decryptedText<<'\n';

   
}
