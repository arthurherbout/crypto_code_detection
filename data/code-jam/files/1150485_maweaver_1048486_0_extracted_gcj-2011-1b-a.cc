#include <iomanip>
#include <iostream>
#include <vector>

int main(int arc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int n;
		std::cin >> n;

#ifdef DEBUG
		std::cout << "--------------------" << std::endl;
#endif

		std::vector<std::string> results;
		for(int i = 0; i < n; i++) {
			std::string row;
			std::cin >> row;

			results.push_back(row);
		}

		std::vector<std::vector<bool> > opponents(n, std::vector<bool>(n, false));
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				if(results[i][j] != '.') {
					opponents[i][j] = true;
				}
			}
		}

		std::vector<double> wps(n, 0);

		for(int i = 0; i < n; i++) {
			double numWins = 0;
			double numGames = 0;

			for(int j = 0; j < n; j++) {
				if(results[i][j] != '.') {
					numGames++;
				}

				if(results[i][j] == '1') {
					numWins++;
				}
			}

			wps[i] = numWins / numGames;
		}

		std::vector<std::vector<double> > wpExcept(n, std::vector<double>(n, 0));
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				if(opponents[i][j]) {
					// Calc wpExcept[i][j] = WP(i) excluding games played against j
					double numWins = 0;
					double numGames = 0;

					for(int k = 0; k < n; k++) {
						if(k != j && results[i][k] != '.') {
							numGames++;
						}
						if(k != j && results[i][k] == '1') {
							numWins++;
						}
					}
					wpExcept[i][j] = numWins / numGames;
				}
			}
		}

		std::vector<double> owps(n, 0);
		for(int i = 0; i < n; i++) {
			double owp = 0;
			double numOwpTeams = 0;
			for(int j = 0; j < n; j++) {
				if(j != i && opponents[i][j]) {
					owp += wpExcept[j][i];
					numOwpTeams++;
				}
			}
			owp /= (double) numOwpTeams;
			owps[i] = owp;
		}

		std::vector<double> oowps(n, 0);
		for(int i = 0; i < n; i++) {
			double oowp = 0;
			double numOowpTeams = 0;
			for(int j = 0; j < n; j++) {
				if(j != i && opponents[i][j]) {
					oowp += owps[j];
					numOowpTeams++;
				}
			}
			oowp /= numOowpTeams;
			oowps[i] = oowp;
		}

		std::cout << "Case #" << testCaseNum + 1 << ":" << std::endl;
		for(int i = 0; i < n; i++) {
			double wp = wps[i];
			double owp = owps[i];
			double oowp = oowps[i];

#ifdef DEBUG
			std::cout << (char) ('A' + i) << ": " << "wp = " << wp << ", owp = " << owp << ", oowp = "<< oowp << std::endl;
#endif

			std::cout << std::setprecision(12) << 0.25 * wp + 0.50 * owp + 0.25 * oowp << std::endl;
		}
	}

	return 0;
}
