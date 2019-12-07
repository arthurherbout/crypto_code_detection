/*
Author: Manoj Kumar R
Date: 10-Sept-2019

#Monoalphabetic cipher
1.key is a permutation of alphabet set (hence String)
    - size of key is same as alphabets size.
    - (remember size of alphabets changes if comma, fullstop,space is considered).
    - for simplicity take english alphabets (hence 26).
2.Every ith aplhabet in alphabet set (ACBCD...) is mapped to the ith key alphabet.

Algorithm:
1. Take key
2. Map to the alphabet set.(store in array/vector)
3. For every charecter, see where it is mapped and substitute.

Some Imp Notes:
    - mapping is one -to-one
    - Code can be still over engineered like storing just alphaMappedToKey and that needs to find the key by value while decrypting.
        - So keeping in mind that reader's time , algorithm is coded

*/
#include<iostream>
#include<map>
using namespace std;

/*
    also supports uppercase,lowercase,extra alphabets such as , . _
    we can take simple array and store just mapped char also, but every time we need to find the index
    This can be global. 
        - because at some time he may get cipher text and that needs to be decrypted
*/
map<char,char> alphaMappedToKey, keyMappedToAplha;


//maps the alphabetset to key
//call below function whenever key or alphabetset is changed.
void mapAplhabetsAndKey(string aplhabets,string key){
     //suppose key is changed in between communication, then previous mapping should be discarded
     alphaMappedToKey = map<char,char>(); 

    //mapping the alphabets to key
    for(int i= 0; i< (int)aplhabets.size(); i++){
        alphaMappedToKey[aplhabets[i]] = key[i]; 
        //example: {'A': 'S'}
    }
    //mapping the key to alphabets.
    for(int i=0;i< (int)key.size();i++){
        keyMappedToAplha[key[i]] = aplhabets[i];
    }
}

//Plain text is encryted to Cipher text
//@params are self explainatory
string encryption(string plainText){

    string cipherText = "";
    //substituting, fo easier understanding i am storing in different variable cipherText
    for (int i=0;i<(int)plainText.size(); i++){
        cipherText += alphaMappedToKey[plainText[i]];
    }
    return cipherText;
}

//decrypts the cipher text to plain text.
string decryption(string cipherText){

    string plainText = "";
    for (int i=0;i<(int)cipherText.size(); i++){
        plainText += keyMappedToAplha[cipherText[i]];
    }
    return plainText;
}

int main(){
    string alphabetset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string key = "VTUYXWZSBIEDCFGHAJKLMNOPQR";
    
    mapAplhabetsAndKey(alphabetset,key);

    string plainText; //P.T
    cout<<"Enter plainText: ";
    cin>>plainText;
    string cipherText = encryption(plainText);

    cout<<"Ciphertext: "<<cipherText<<'\n';
    string decryptedText = decryption(cipherText);
    cout<<"decryptedText: "<<decryptedText<<'\n';
    
}