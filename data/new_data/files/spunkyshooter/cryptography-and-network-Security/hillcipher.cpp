/*
Author: Manoj Kumar R
Date: 11-Sept-2019

# Hillcipher
1. This is polygraphic cipher. 
2. Key is a n X n square matrix.
3. Break the PlainText into chunks of length n.
4. Then apply encryption for each chunk

Encipher Algo
1. C.T = (P.T X K )mod 26 , where C.T and P.T are Row vectors/matrices

Decipher Algo:
1. First cal determinant of the key
2. Find the modular inverse of the determinant. i.e (det|K|)^-1 mod 26
3. Find the inverse of the key. (with the help of 2)
4. P.T = C.T X K^-1 mod 26

NOTE:
Traditionally, P.T and C.T are row vectors. In some textbooks they take column
vectors, then the P.T should be multiplied afterwards. i.e: C.T = K X P.T mod 26
Similarly, P.T = K^-1 X C.T mod 26
Also Remember, Both are not same. matrix multiplication is not commutative

#TODO:
Decryption:
    finding determinant and inverse in lengthy. Will do after some time.
*/
#include<iostream>
#include<vector>
using namespace std;
#define DB(x) cout<<#x<<": "<<x<<'\n'

class Hillcipher {
    const string AlphabetSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<vector<int>> key; // N X N, square matrix
    int N;
    vector<int> plainTextNum;
    string PlainText;

    public:
    Hillcipher(vector<vector<int>> key){
        this->key = key;
        this->N = (int)key.size();
    }

    string encryption(string plainText){
        this->PlainText = plainText;
        string cipherText = "";
        int padding = N - ( (int)plainText.size() % N);
        //plainText is padded with 0's .
        //Remember we are going to extract the numerical from ascii itself. i.e A = 65
        //Therefore 'A' - 65 = 0 (index of A)
        if(padding != N){
            for(int i = 0;i < padding; i++)
                this->PlainText += '0';
        }
        for(int i = 0 ;i< this->PlainText.size(); i += N){
            matrixMultiplication(i); //index of plaintext
        }
        for(int index: plainTextNum ){
            cipherText += AlphabetSet[index];
        }
        return cipherText;
    }

    string decryption(const string cipherText){
    
        return "Don't Know";
    }

    private:
    //Receives the index of the plaintext from where the chunk should be considered
    //Does CT = Key X P.T mod 26
    //And pushes the result to the plainTextNum array.
    void matrixMultiplication(int index){
        //multipies N X N keymatrix with N X 1 PlainText matrix
        //Doen't confuse here since i haven't represent the PlainText as Col matrix.
        for(int i = 0;i < this->N ; i++){
            int sum = 0;
            int k = index;
            for(int j = 0; j < this->N ; j++){
                int numericalOfChar = this->PlainText[k++];
                numericalOfChar -= (numericalOfChar >=65) ? 65 : 48; //'A' = 65, '0' = 48
                sum += ((key[i][j] % 26) * (numericalOfChar % 26)) % 26;
            }
            plainTextNum.push_back(sum%26);
        }

    }


};


int main(){
    
    vector<vector<int>> key = {{5,17},{4,17}};  //{{2,4,5},{9,2,1},{3,17,7}};

    string plainText; //P.T
    cout<<"Enter plainText: ";
    cin>>plainText;
    Hillcipher HC(key);
    string cipherText = HC.encryption(plainText);
    cout<<"Ciphertext: "<<cipherText<<'\n';
    // string decryptedText = HC.decryption(cipherText);
    // cout<<"decryptedText: "<<decryptedText<<'\n';

   
}
