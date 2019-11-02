/**
 * @author Leo "Ekinox" Gaspard <ekinox1995@gmail.com>
 */

#include <iostream>
#include <list>

/// @return The xor-sum of @param V.
int XorSum(std::list<int> const & V) {
    int R = 0;
    for (std::list<int>::const_iterator it = V.begin() ; it != V.end() ; ++it) {
        R ^= *it;
    }
    return R;
}

/// @return The sum of @param Bag except the minimum.
int Best(std::list<int> const & Bag) {
    int Min = 0;
    int Sum = 0;
    for (std::list<int>::const_iterator it = Bag.begin() ; it != Bag.end() ; ++it) {
        if (*it < Min || Min == 0) {
            Sum += Min;
            Min = *it;
        } else {
            Sum += *it;
        }
    }
    return Sum;
}

int main(int, char**) {
    int T;
    std::cin >> T; std::cin.ignore();

    for (int I = 1 ; I <= T ; ++I) {
        int N;
        std::cin >> N; std::cin.ignore();

        std::list<int> Bag;
        while (N > 0) {
            --N;
            int C;
            std::cin >> C;
            Bag.push_back(C);
        }

        /// Patrick's sum is the xor sum.
        int PatrickSum = XorSum(Bag);

        /// If the xor-sum isn't 0, then it is not possible to equilibrate.
        /// Example : ^(V) = Xor-sum of V ; Sean = Sean's stack ; Patrick = Patrick's stack ; Bag = Candies in the bag
        ///    ^(Patrick) = ^(Sean)
        /// => ^(Patrick) ^ ^(Sean) = 0
        /// => ^(Bag) = 0
        if (PatrickSum != 0) {
            std::cout << "Case #" << I << ": NO" << std::endl;
        } else {
            /// And, at last, Patrick's sum has some interesting characteristics.
            /// With Candy any candy initially in Patrick :
            ///    ^(Sean) = ^(Patrick)
            /// => ^(Sean) = ^(Patrick without Candy) ^ Candy
            /// => ^(Sean) ^ Candy = ^(Patrick without Candy)
            /// => ^(Sean with Candy) = ^(Patrick without Candy
            /// So, if a distribution does exist, then Sean could "steal" any
            /// candy Patrick could have, without changing the equality
            /// relationship between their two stacks.
            /// However, no stack might be empty.
            /// So will Sean offer the less-valued candy to Patrick.
            std::cout << "Case #" << I << ": " << Best(Bag) << std::endl;
        }
    }

    return 0;
}

