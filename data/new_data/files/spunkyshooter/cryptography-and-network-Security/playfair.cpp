/*
Author: Manoj Kumar R
Date: 10-Sept-2019

# Playfair cipher
1. key is string.
2. Alphabet set is just( ABCD..Z ) size = 26
3. construct 5 X 5 matrix. (club i and j).
4. Asumption: letters in Diagram are not same. 
Encipher
 1. Break the words into diagrams (chunks of 2 letters)
 2. If any diagram has same letters, add 'X' 
    - ex: BALLOON => BA LL .. (same letters LL) => BA LX LO ON
    - ex: MOOC => MO OC
    - ex: MAGIC => MA GI CX
 3.If both letter is in same column, take the beneath element.(cylically)
 4.If both letter is in same row, take the right element.(cylically)
 5.If letter are in different row and column, take corner letter in the same row
Decipher
 1. Break the words into diagrams (chunks of 2 letters)
 2.If both letter is in same column, take the above element.(cylically)
 3.If both letter is in same row, take the left element.(cylically)
 4.If letter are in different row and column, take corner letter's in the same row
 
 Some Additional Notes:
    - unlike traditional playfair cipher, there is another method.
        -If we leave two same letter in the diagram,they will be encoded by moving down and right one square.
        -whereas in traditional play cipher, atomatically X is added
#TODO:
    - Add support to include numbers, punctuations.
        - Simple. they don't encode to anything. they remain as it is. BA2M => LK2JC (say)
*/
#include<iostream>
#include<vector>
#include<bitset>
using namespace std;
#define DB(x) cout<<#x<<": "<<x<<'\n'
vector<vector<char>> keyMatrix(5,vector<char>(5)); //2D matrix which stores the char
vector<pair<int,int>> aMTRAC(26); // alphabetsMappedToRowsAndColumns

void constructMatrix(const string AlphabetSet,const string key){
    bitset<26> filled; //can use array or map. but bitset is space efficient.
    //BY default not filled,false. A = 65
    int r = 0,c = 0; //tracking the indexes,
    //filling the key.
    for(char letter : key){
        bool ok = filled[letter - 65];
        if(!ok) {
            filled[letter - 65] = 1; //true, filled
            aMTRAC[letter - 65] = {r,c};
            keyMatrix[r][c] = letter;
            if(letter == 'I' || letter == 'J'){
                //since we don't know whether the letter would be I or J, hence once again assinging
                filled['I' - 65] = 1;
                filled['J' - 65] = 1;
                aMTRAC['I' - 65] = {r,c};
                aMTRAC['J' - 65] = {r,c};
                keyMatrix[r][c] = 'I';
            }
            //so when the letter is't filled, it fills it
            //And we need to increase the index
            //since we now that for every r , c runs from 0 to 4
            if(c == 4){
                r++;
                c = 0;
            }else{
                c++;
            }
        }   
    }
    //fill the charecters which ever isn't filled
    //there is not need to mark the charaters as filled, since we are iterating.
    for(char x : AlphabetSet){
        if(filled[x - 65]) continue; //incase 
        aMTRAC[x - 65] = {r,c};
        keyMatrix[r][c] = x;
        if(x == 'I'){ //when key doesn't have i or j, then it isn't filled
            filled['J' - 65] = 1; //hence for next interation J is considered as filled
            aMTRAC['J' - 65] = {r,c};
        }

        if(c == 4){
                r++;
                c = 0;
        }
        else c++;    
    }
   
    //For printing the matrix
    for(auto A:keyMatrix){
        for(char c:A){
            cout<<c<<' ';
        }
        cout<<'\n';
    }
   
    

}

vector<string> wordsIntoDiagrams(const string plainText){
    //Breaking into 2 letter words. (Diagrams)
    string temp;
    vector<string> diagrams;
    for(char letter:plainText){
       temp += letter;

       if((int)temp.size() == 2){
           if(temp[0] == temp[1]){
          
            //    temp[0] + "X" this won't work. this returns "back_aux". This is due to char + string (concatination)
               string s = string(1,letter) + "X"; //char is converted to string first and then concatinated
               diagrams.push_back(s);
               temp = letter; //temp is reassinged with only one letter. ex: temp = LL , then becomes temp = L
           }
           else{
                diagrams.push_back(temp);
                temp = "";
           }
          
       }
    }
    if(temp != "") //i.e temp has single letter. note: temp with 2 letters would be already processed before itself
    {
        diagrams.push_back(temp + "X");
    }
    return diagrams;
}

string encryption(string plainText){
     // we can use this, since we won't be having any repeated letters in a diagram.
    vector<string> diagrams = wordsIntoDiagrams(plainText); 
   
    string cipherText = "";
    for(string diagram: diagrams){
        string cipherTextDiagram = "";
        pair<int,int> indexes1 = aMTRAC[diagram[0] - 65];
        pair<int,int> indexes2 = aMTRAC[diagram[1] - 65];
   
        int row1 = indexes1.first, col1 = indexes1.second,
            row2 = indexes2.first, col2 = indexes2.second;
        if(row1 == row2){
            col1 = (col1 + 1)%5;
            col2 = (col2 + 1)%5;
            cipherTextDiagram = string(1,keyMatrix[row1][col1]) + string(1,keyMatrix[row2][col2]); 
        }
        else if(col1 == col2){
            row1 = (row1 + 1)%5;
            row2 = (row2 + 1)%5;
            cipherTextDiagram = string(1,keyMatrix[row1][col1]) + string(1,keyMatrix[row2][col2]);
        }
        else
        {
            cipherTextDiagram = string(1,keyMatrix[row1][col2]) + string(1,keyMatrix[row2][col1]);
        }
        cipherText += cipherTextDiagram;
        
    }
    return cipherText;

}

string decryption(const string cipherText){
     // we can use this, since we won't be having any repeated letters in a diagram.
    // vector<string> diagrams = wordsIntoDiagrams(cipherText); but not needed. 
    string plainText = "";
     for(int i=0;i<cipherText.size();i = i+2){
        string plainTextDiagram = "";
        pair<int,int> indexes1 = aMTRAC[cipherText[i] - 65];
        pair<int,int> indexes2 = aMTRAC[cipherText[i+1] - 65];
        int row1 = indexes1.first, col1 = indexes1.second,
            row2 = indexes2.first, col2 = indexes2.second;
        if(row1 == row2){
            col1 = ((col1 - 1)%5 + 5)%5;
            col2 = ((col2 - 1)%5 + 5)%5;
            plainTextDiagram = string(1,keyMatrix[row1][col1]) + string(1,keyMatrix[row2][col2]); 
        }
        else if(col1 == col2){
            row1 = ((row1 - 1)%5 + 5)%5;
            row2 = ((row2 - 1)%5 + 5)%5;
            plainTextDiagram = string(1,keyMatrix[row1][col1]) + string(1,keyMatrix[row2][col2]);
        }
        else
        {
            plainTextDiagram = string(1,keyMatrix[row1][col2]) + string(1,keyMatrix[row2][col1]);
        }
        plainText += plainTextDiagram;
        
    }
    return plainText;
}
string FormatedDecryptedText(const string decryptedText){
    /*
    if X is b/w two same letters, then remove x. ex: BALXLOON
    if X is at last, then remove x. ex: MAGICX
    */
    string PT = "";
    for(int i = 0; i< decryptedText.size(); i++){
        char c = decryptedText[i];
        if(c == 'X' && i == decryptedText.size() -1) continue; //hence we don't 
        if(i > 0  && (c == 'X' && (decryptedText[i-1]  == decryptedText[i+1]) )){
            continue;
        }

        PT += c; //in all other cases. 
    }
    return PT;
}
int main(){
    const string AlphabetSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string key = "PLAYFAIR";
    string plainText; //P.T
    cout<<"Enter plainText: ";
    cin>>plainText;
    constructMatrix(AlphabetSet,key);

    string cipherText = encryption(plainText);
    cout<<"Ciphertext: "<<cipherText<<'\n';
    string decryptedText = decryption(cipherText);
    cout<<"decryptedText: "<<decryptedText<<'\n';
    string formatedDT = FormatedDecryptedText(decryptedText);
    cout<<"FormatedDecryptedText: "<<formatedDT<<'\n';
}
