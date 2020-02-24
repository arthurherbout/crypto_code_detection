#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

unsigned long patSum(unsigned long a, unsigned long b) {
	return a + b - ((a & b) << 1);
}

struct TableEntry {

	TableEntry() : patSumL(0), patSumR(0), sumL(0), sumR(0) { }
	TableEntry(unsigned long _patSumR, unsigned long _sumR) : patSumL(0), patSumR(_patSumR), sumL(0), sumR(_sumR) { }

	TableEntry moveToLeft(const std::vector<unsigned long> &candies, unsigned long candy) const {
		TableEntry ret = *this;

		ret.patSumL = patSum(ret.patSumL, candies[candy]);
		ret.patSumR = patSum(ret.patSumR, candies[candy]);

		ret.sumL += candies[candy];
		ret.sumR -= candies[candy];

#ifdef DEBUG
		std::cout << "Moving candy " << candies[candy] << " changes " << 
			patSumL << ", " << patSumR << " -> " << patSumDiff() << " :: " << 
			sumL << ", " << sumR << " -> " << sum() << " to " << 
			ret.patSumL << ", " << ret.patSumR << " -> " << ret.patSumDiff() << " :: " << 
			ret.sumL << ", " << ret.sumR << " -> " << ret.sum() << std::endl;

#endif

		return ret;
	}

	unsigned long patSumDiff() const {
		return std::abs((int) patSumL - (int) patSumR);
	}

	unsigned long sum() const {
		return std::max(sumL, sumR);
	}

	unsigned long patSumL;
	unsigned long patSumR;

	unsigned long sumL;
	unsigned long sumR;
};

int main(int arc, char **argv) {
	unsigned long numTestCases;
	std::cin >> numTestCases;

	for(unsigned long testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		
		unsigned long numCandies;
		std::cin >> numCandies;

		std::vector<unsigned long> candies;
		unsigned long sumR = 0;
		unsigned long patSumR = 0;
		for(unsigned long i = 0; i < numCandies; i++) {
			unsigned long candy;
			std::cin >> candy;
			candies.push_back(candy);
			sumR += candy;
			patSumR = patSum(patSumR, candy);
		}

		TableEntry base(patSumR, sumR);
		std::map<unsigned long, TableEntry> col;

		for(unsigned long colNum = 0; colNum < numCandies; colNum++) {
			std::map<unsigned long, TableEntry> nextCol = col;

			// Can use the candy alone
			TableEntry l = base.moveToLeft(candies, colNum);
			if(l.sumL > 0 && l.sumR > 0 && nextCol[l.patSumDiff()].sum() < l.sum()) {
				nextCol[l.patSumDiff()] = l;
			}

			// Can add the candy to previous chains
			for(std::map<unsigned long, TableEntry>::const_iterator colIt = col.begin();
			    colIt != col.end(); ++colIt) {
				
				TableEntry next = colIt->second.moveToLeft(candies, colNum);
				if(next.sumL > 0 && next.sumR > 0 && nextCol[next.patSumDiff()].sum() < next.sum()) {
					nextCol[next.patSumDiff()] = next;
				}
			}

			col = nextCol;
		}

		std::cout << "Case #" << testCaseNum + 1 << ": ";
		if(col.find(0) == col.end()) {
			std::cout << "NO" << std::endl;
		} else {
			std::cout << col[0].sum() << std::endl;
		}
	}

	return 0;
}
