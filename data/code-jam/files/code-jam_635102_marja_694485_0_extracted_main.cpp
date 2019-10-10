#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

bool is_symm_x(const vector<int>& nums, int added)
{
    //cerr << "is symm" << endl;
    //copy(nums.begin(), nums.end(), ostream_iterator<int>(cerr, " "));
    //cerr << " + " << added << endl;
    int n = nums.size() - 1;
    for (int i = added, j = n; i < j; ++i, --j) {
        if (nums[i] != nums[j]) {
            //cerr << "not" << endl;
            return false;
        }
    }
    //cerr << "is" << endl;
    return true;
}

bool is_symm_y(const vector<int>& nums, int added)
{
    //cerr << "is symm_y" << endl;
    //copy(nums.begin(), nums.end(), ostream_iterator<int>(cerr, " "));
    //cerr << " + " << added << endl;
    int n = nums.size() - 1;
    for (int i = 0, j = n - added; i < j; ++i, --j) {
        if (nums[i] != nums[j]) {
            //cerr << "not" << endl;
            return false;
        }
    }
    //cerr << "is" << endl;
    return true;
}


int main(void)
{
    int cases = 0;
    cin >> cases;
    for (int c = 0; c < cases; ++c) {
        int k = 0;
        cin >> k;
        vector<vector<int> > diamond_rows(2 * k - 1);
        vector<vector<int> > diamond_cols(k);
        //cerr << "k is " << k << endl;
        for (int row = 1; row <= k; ++row) {
            diamond_rows[row-1] = vector<int>(row);
            for (int n = 0; n < row; ++n) {
                cin >> diamond_rows[row-1][n];
                diamond_cols[n].push_back(diamond_rows[row-1][n]);
            }
        }
        for (int row = k + 1; row <= 2 * k - 1; ++row) {
            diamond_rows[row-1] = vector<int>(2 * k -row);
            for (int n = 0; n < 2*k - row; ++n) {
                cin >> diamond_rows[row-1][n];
                diamond_cols[n].push_back(diamond_rows[row-1][n]);
            }
        }
        /*cerr << "rows" << endl;
        for (int row = 0; row < 2* k - 1; ++row) {
            copy(diamond_rows[row].begin(), diamond_rows[row].end(), ostream_iterator<int>(cerr, " "));
            cerr << endl;
        }

        cerr << "cols" << endl;
        for (int col = 0; col < k ; ++col) {
            copy(diamond_cols[col].begin(), diamond_cols[col].end(), ostream_iterator<int>(cerr, " "));
            cerr << endl;
        }
        */

        int needed = 0;
        for (int row = 0; row < 2 * k - 1; ++row) {
            while (!is_symm_x(diamond_rows[row], needed))
                ++needed;            
        }
        //cerr << "for x needed " << needed;
        for (int col = 0; col < k; ++col) {
            while (!is_symm_y(diamond_cols[col], needed))
                ++needed;
        }

        //cerr << needed << endl;
        int cost = (k + needed) * (k + needed) - k * k; 
        cout << "Case #" << (c + 1) << ": " << cost << endl;
    }
    return 0;
}
