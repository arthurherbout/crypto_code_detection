#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int N, M;

bool check_row_col(vector<vector<char> > &board, int r, int c){
    int r_non_plus = 0, c_non_plus = 0;
    for(int i = 0; i < N; i ++){
        if(board[i][c] != '.' && board[i][c] != '+')
            c_non_plus ++;
        if(board[r][i] != '.' && board[r][i] != '+')
            r_non_plus ++;
    }
    return c_non_plus <= 1 && r_non_plus <= 1;
}

bool check_dia(vector<vector<char> > &board, int r, int c){
    int non_x_1 = 0, non_x_2 = 0;
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){

            if(i + j == r + c){
                if(board[i][j] != '.' && board[i][j] != 'x'){
                    non_x_1 ++;
                }
            }
            if(i - j == r - c){
                if(board[i][j] != '.' && board[i][j] != 'x')
                    non_x_2 ++;
            }
        }
    }
    return (non_x_1 <= 1) && (non_x_2 <= 1);
}

int count_points(vector<vector<char> > &board){
    int res = 0;
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            char c = board[i][j];
            if(c == 'x' || c == '+')
                res ++;
            else if (c == 'o')
                res += 2;
        }
    }
    return res;
}
void print_board(vector<vector<char> > &board){
    for(int i = 0; i < N; i ++){
            for(int j = 0; j < N; j ++){
                cout << board[i][j] << " ";
            }
            cout << endl;
    }
}

void fast_solution(int N, vector<vector<char> > &board){
    vector<string> modifications;
    vector<char> arr = {'+', 'x', 'o'};
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            char m, c = board[i][j];
            bool plus_valid =false, x_valid = false, o_valid = false;
            for(int k = 0; k < 3; k ++){
                board[i][j] = arr[k];
                bool valid = check_row_col(board, i, j) && check_dia(board, i, j);
                if(valid){
                    if(k == 0)  plus_valid = true;
                    else if (k == 1)    x_valid = true;
                    else if (k == 2)    o_valid = true;
                }
            }
            board[i][j] = c;
            if(o_valid)  m = 'o';
            else if (plus_valid)   m = '+';
            else if (x_valid)   m = 'x';
            else    continue;
            
            if(c == m || (c != '.' && m != 'o') ) continue;
            string tmp = string(1, m) + " " + to_string(i + 1) + " " + to_string(j + 1);
            modifications.push_back(tmp);
            board[i][j] = m;
        }
    }
    
    cout << to_string(count_points(board))+ " " + to_string(modifications.size()) << endl;
    for(string str : modifications)
        cout << str << endl;
}

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int t, count = 1;
    cin >> t;
    while(t--){
        cin >> N >> M;
        vector<vector<char> > board(N, vector<char> (N, '.'));
        for(int i = 0; i < M; i ++){
            char c;
            int row, col;
            cin >> c;
            cin >> row >> col;
            board[row - 1][col - 1] = c;
        }
        cout << "Case #" << count << ": ";
        fast_solution(N, board);
        count ++;
    }

    return 0;
}