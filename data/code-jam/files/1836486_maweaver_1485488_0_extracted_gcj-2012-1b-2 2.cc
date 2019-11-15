#include <iostream>
#include <queue>
#include <vector>

int minWaitTime(int x1, int y1, int x2, int y2,
                int r, int c,
                int waterHeight,
                const std::vector<std::vector<int> > &ceilingHeights,
                const std::vector<std::vector<int> > &floorHeights) {
	if(x2 <= -1 || x2 >= c || y2 <= -1 || y2 >= r) {
		return -1;
	}

	if(!(floorHeights[y1][x1] <= ceilingHeights[y2][x2] - 50)) {
		return -1;
	}

	if(!(floorHeights[y2][x2] <= ceilingHeights[y2][x2] - 50)) {
		return -1;
	}

	if(!(floorHeights[y2][x2] <= ceilingHeights[y1][x1] - 50)) {
		return -1;
	}

	int reqWaterHeight = ceilingHeights[y2][x2] - 50;
	int diff = waterHeight - reqWaterHeight;
	return std::max(diff, 0);
}

bool canMoveSimple(int x1, int y1, int x2, int y2,
                   int r, int c,
                   int waterHeight,
                   const std::vector<std::vector<int> > &ceilingHeights,
                   const std::vector<std::vector<int> > &floorHeights) {
	
	if(x2 <= -1 || x2 >= c || y2 <= -1 || y2 >= r) {
		return false;
	}

	if(!(waterHeight <= ceilingHeights[y2][x2] - 50)) {
		return false;
	}

	if(!(floorHeights[y1][x1] <= ceilingHeights[y2][x2] - 50)) {
		return false;
	}

	if(!(floorHeights[y2][x2] <= ceilingHeights[y2][x2] - 50)) {
		return false;
	}

	if(!(floorHeights[y2][x2] <= ceilingHeights[y1][x1] - 50)) {
		return false;
	}

	return true;
}

int main(int argc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int waterHeight, r, c;
		std::cin >> waterHeight >> r >> c;

		std::vector<std::vector<int> > ceilingHeights(r, std::vector<int>(c, 0));
		std::vector<std::vector<int> > floorHeights(r, std::vector<int>(c, 0));

		for(int i = 0; i < r; i++) {
			for(int j = 0; j < c; j++) {
				std::cin >> ceilingHeights[i][j];
			}
		}

		for(int i = 0; i < r; i++) {
			for(int j = 0; j < c; j++) {
				std::cin >> floorHeights[i][j];
			}
		}

		std::vector<std::vector<int> > dp(r, std::vector<int>(c, -1));

		dp[0][0] = 0;

		std::deque<std::pair<int, int> > queue;
		queue.push_back(std::make_pair(0, 0));
		while(!queue.empty()) {
			std::pair<int, int> cur = queue.front();
			queue.pop_front();

			for(int dy = -1; dy <= 1; dy++) {
				for(int dx = -1; dx <= 1; dx++) {
					if((dy == 0 || dx == 0) &&
					   canMoveSimple(cur.first, cur.second, 
					                 cur.first + dx, cur.second + dy,
					                 r, c,
					                 waterHeight, ceilingHeights, floorHeights) &&
					   dp[cur.second + dy][cur.first + dx] == -1) {
						dp[cur.second + dy][cur.first + dx] = 0;
						queue.push_back(std::make_pair(cur.first + dx, cur.second + dy));
					}
				}
			}
		}

		/*
			std::cout << "--------------------" << std::endl;
			for(int y = 0; y < r; y++) {
				for(int x = 0; x < c; x++) {
					std::cout << dp[y][x] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << "--------------------" << std::endl;
*/
		while(dp[r - 1][c - 1] == -1) {
			std::vector<std::vector<int> > nextDp(r, std::vector<int>(c, -1));

			for(int y = 0; y < r; y++) {
				for(int x = 0; x < c; x++) {
					if(dp[y][x] == -1) {
						continue;
					}
					for(int dy = -1; dy <= 1; dy++) {
						for(int dx = -1; dx <= 1; dx++) {
							if((dy == 0 || dx == 0) && (dx != 0 || dy != 0)) {
								int nx = x + dx;
								int ny = y + dy;

								int enterTime = minWaitTime(x, y, nx, ny, r, c, waterHeight, ceilingHeights, floorHeights);
								if(enterTime != -1) {
									//std::cout << "To go from (" << x << ", " << y << ") @ " << dp[y][x] << " to (" << nx << ", " << ny << "), need to wait for time t = " << enterTime << std::endl;

									int newWaterHeight = waterHeight - std::max(enterTime, dp[y][x]);
									int time = std::max(enterTime, dp[y][x]) + (newWaterHeight - floorHeights[y][x] >= 20 ? 10 : 100);
									if(nextDp[ny][nx] == -1 || time < nextDp[ny][nx]) {
										nextDp[ny][nx] = time;
									}
								}
							}
						}
					}
				}
			}

			/*
			std::cout << "--------------------" << std::endl;
			for(int y = 0; y < r; y++) {
				for(int x = 0; x < c; x++) {
					std::cout << nextDp[y][x] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << "--------------------" << std::endl;
			*/
			dp = nextDp;
		}
		
		std::cout << "Case #" << testCaseNum + 1 << ": " << (dp[r - 1][c - 1] / 10) << "." << (dp[r - 1][c - 1] % 10) << std::endl;

	}

	return 0;
}
