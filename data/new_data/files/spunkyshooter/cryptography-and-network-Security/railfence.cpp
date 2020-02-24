#include<iostream>
#include<vector>
using namespace std;
#define DB(x) cout<<#x<<" : "<<x<<'\n'
// TODO: decryption for improved railfence
class TraditionalRailfence{
    int depth;
    
    public:
    TraditionalRailfence(int depth){
        this->depth = depth;
    }
    string encryption(string plainText){
        //Note: if plainText length is less than depth, then there is no encryption
       
        //There is no need of 2d matrix to fill. 
        //Note: This is a sparse matrix, hence using 2D matrix consumes more unwanted memory.
        //Instead, we can use the below logic
        //MEETMETODAY with depth = 3 ,fills as shown below
        /*
        M....M.....D
        .E.T..E..O..A
        ..E....T......Y

        can also represented as:
        MM D
        ETEOA
        E T Y
        hence one loop is for going down, other is for going up.
        Extra care should be taken for handling the edge cases like length.
        */

        vector<string> rails(depth,""); //intializing the rails
        int d = 0;
        while(d < plainText.size()) {
            int index = 0;
            for(int i = d;i < plainText.size() && i < (this->depth + d); i++){
                rails[index++] += plainText[i];
            }
            d += this->depth;
            index -= 2; 
            //2 times because: index has been increased due to (index++)
            //and when we start to fill from down to up, we should leave the bottom one (see example above)
            for(int i = d ;(index > 0 && d < plainText.size()) && i < (this->depth + d); i++,d++ ){
                rails[index--] += plainText[i];
            }
        }
        string cipherText = "";
        
        for(string x: rails){
           cipherText += x;
        }
        

        return cipherText;
    }

    string decryption(string ciphertext){
        //For decryption we need to know the length of the cipher text and depth of the rails
        //Since we know the length of the rails, we can generate the indexes. (same as used in encryption algo)
        vector<vector<int>> railsIndex(this->depth); //intializing the rails
        int d = 0;
        int length = (int)ciphertext.size();
        while(d < length) {
            int index = 0;
            for(int i = d;i < length && i < (this->depth + d); i++){
                railsIndex[index++].push_back(i);
            }
            d += this->depth;
            index -= 2; 
            //2 times because: index has been increased due to (index++)
            //and when we start to fill from down to up, we should leave the bottom one (see example above)
            for(int i = d ;(index > 0 && d < length) && i < (this->depth + d); i++,d++ ){
                 railsIndex[index--].push_back(i);
            }
        }
        /*
        ex: plaintext: MEETMETODAY
        railIndexVector
        0 6
        1 5 7
        2 4 8 10
        3 9

        => 0,6,1,5,7,2,4,8,10,3,9 : This is the way cipher text is present
        */
        //By Now we have indexes in the railsIndex vectors.
        //That particular index is mapped to cipher text.
        //For that we take ith element from ciphertext and see what is it index from railIndex vector and place it in the plaintext
        //hence we need plainText of length same as cipher text 
        string plainText = ciphertext ; //any dummy text of length same as cipher text works
        //because that will be substituted with the correct order letters.
        int i = 0;
        for(vector<int> A: railsIndex){
           for(int a:A){
               
               plainText[a] = ciphertext[i++];
           }
           
        }
        
        return plainText;
    }
};

//In order to make it complex: write msg row by row and read column by column
//The order of the column becomes the key.
//Do padding if spaces left in the tail
class ImprovedRailFence{
    //Remember: elements in the key vector will be in range [1..keysize] ,
    public:
    string encryption(string plainText,vector<int> key){
        int rows = plainText.size() / key.size();
        int columns = key.size();
        if(plainText.size() % key.size() != 0) rows++;
        //2D matrix exactly of needed rows and columns and filled with 'X' used as padding.
        //these 'X's will be substituted
        vector<vector<char>> matrix(rows,vector<char>(columns, 'X'));
         //index = -1, since initially 0 % columns == 0
        //storing row wise

        for(int i = 0, k =0;i<rows && k < plainText.size();i++){
            for(int j = 0;j <columns && k < plainText.size();j++){
                    matrix[i][j] = plainText[k++];
            }
        }
        for(vector<char> C:matrix){
            for(char c:C){
                cout<<c<<' ';
            }
            cout<<'\n';
        }
        string cipherText = "";
        for(int i = 1;i <= columns;i++){ //starting from 1 because key have values from [1..key.size()]
            //find the index
            int index = -1; 
            //Asumption: indexes in the key doesn't violate
            for(int j = 0; j <columns; j++)
            {
                if(key[j] == i){
                    index = j;
                    break;
                }
            }//If key is long. This isn't efficient, In that case map the idexes and sort, then use it
            //reading column wise
            for(int j = 0 ;j < rows; j++){
                cipherText += matrix[j][index];
            }

        }
        return cipherText;
    }
    string decryption(string cipherText,vector<int> key){
        int rows = cipherText.size() / key.size(); //its completly divisible
        int columns = key.size();
        vector<vector<char>> matrixUnordered(rows,vector<char>(columns));
        
        //storing column wise
        for(int i = 0 ,k = 0;i<columns;i++){
            for(int j =0 ;j <rows;j++){
                matrixUnordered[j][i] = cipherText[k++];
            }
        }
        cout<<"before reordering\n";
        for(vector<char> C:matrixUnordered){
            for(char c:C){
                cout<<c<<' ';
            }
            cout<<'\n';
        }
        //elements should be allinged w.r.t key.
        //But in matrixUnordered, they are allinged actually [1,2..N]
        //Now we have to reaarange them w.r.t to the key.
        vector<vector<char>> matrixOrderedWithkey(rows,vector<char>(columns));
        int k = 0;
        for(int keyIndex: key){ //pick the index from the key array
            for(int i = 0;i<rows ;i++){
                //pick element from matrixUnordered array with that particular index
                //and store in the matrixOrderedWithkey 2D array
                matrixOrderedWithkey[i][k] = matrixUnordered[i][keyIndex-1]; 
            }
            k++;
        }
        cout<<"After reordering\n";
         for(vector<char> C:matrixOrderedWithkey){
            for(char c:C){
                cout<<c<<' ';
            }
            cout<<'\n';
        }
        
        string plainText = "";
        for(int i = 0;i < rows;i++){ 
            //reading row wise
            for(int j = 0 ;j < columns; j++){
                plainText += matrixOrderedWithkey[i][j];
            }

        }
        //Removing the padding present at the tail.
        for(auto it = plainText.rbegin() ;it != plainText.rend(); it++){
            if((*it) == 'X') plainText.pop_back();
            else break;
        }
        return plainText;
    }
};
int main() {
    TraditionalRailfence TRF(4);
    string plainText; //P.T
    cout<<"Enter plainText: ";
    cin>>plainText;
    string cipherText =  TRF.encryption(plainText);

    cout<<"Ciphertext: "<<cipherText<<'\n';
    string decryptedText = TRF.decryption(cipherText);
    cout<<"decryptedText: "<<decryptedText<<'\n';

    ImprovedRailFence IRF;
    string plainText2; //P.T
    cout<<"Enter plainText For improved RailFence cipher: ";
    cin>>plainText2;
    string cipherText2 =  IRF.encryption(plainText2,{2,4,3,1});

    cout<<"Ciphertext: "<<cipherText2<<'\n';
    string decryptedText2 = IRF.decryption(cipherText2,{2,4,3,1});
    cout<<"decryptedText: "<<decryptedText2<<'\n';
  

}