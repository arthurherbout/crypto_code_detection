#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main()
{
    istream& in = cin;
    // ifstream in("a_test.in");

    int T;
    in >> T;
    for (int testCase = 0; testCase != T; ++testCase) {

        int N;
        in >> N;
        vector<int> a;
        vector<int> b;
        vector<bool> doneOneStar;
        vector<bool> doneTwoStar;
        a.resize(N);
        b.resize(N);
        doneOneStar.resize(N);
        doneTwoStar.resize(N);
        
        for (int n = 0; n != N; ++n) {
            in >> a[n] >> b[n];
            doneOneStar[n] = doneTwoStar[n] = false;
        }

        int numStars = 0;
        int numLevels = 0;
        int numTwoStarsDone = 0;
        while (numTwoStarsDone < N) {

            // do a 2-star if we can.  firstly try to find one we haven't done in 1-star mode
            bool foundTwoStar = false;
            for (int i = 0; i != N; ++i) {
                if (!doneTwoStar[i] && !doneOneStar[i] && numStars >= b[i]) {
                    // can do this.
                    ++numTwoStarsDone;
                    doneTwoStar[i] = true;
                    ++numLevels;
                    numStars += 2;
                    foundTwoStar = true;
                    break;
                }
            }

            if (!foundTwoStar) {
                // try again, look at ones we've already completed in 1-star mode.
                for (int i = 0; i != N; ++i) {
                    if (!doneTwoStar[i] && numStars >= b[i]) {
                        // can do this.
                        ++numTwoStarsDone;
                        doneTwoStar[i] = true;
                        ++numLevels;
                        numStars ++;
                        foundTwoStar = true;
                        break;
                    }
                }
            }

            if (!foundTwoStar) {
                // didn't find a 2-star.  just do a 1-star level.
                // pick the one that's the hardest 2-star.  (thereby leaving more
                // easy 2-star opportunities open)
                int bestOneStar = -1;
                bool foundOneStar = false;
                for (int i = 0; i != N; ++i) {
                    if (!doneOneStar[i] && numStars >= a[i]) {
                        // can do this.
                        if (!foundOneStar || b[i] > b[bestOneStar]) {
                            foundOneStar = true;
                            bestOneStar = i;
                        }
                    }
                }

                if (foundOneStar) {
                    doneOneStar[bestOneStar] = true;
                    ++numLevels;
                    numStars++;
                } else {
                    // couldn't do anything.  stop now.
                    break;
                }
            }
        }

        if (numTwoStarsDone < N) {
            cout << "Case #" << (testCase + 1) << ": Too Bad" << endl;
        } else {
            cout << "Case #" << (testCase + 1) << ": " << numLevels << endl;
        }
    }
    return 0;
}
