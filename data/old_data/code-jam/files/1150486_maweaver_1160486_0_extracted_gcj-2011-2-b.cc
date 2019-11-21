#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

int main(int arc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int height, width, mass;
		std::cin >> height >> width >> mass;

#ifdef DEBUG
		std::cout << width << "x" << height << std::endl;
#endif
		std::vector<std::vector<int> > masses;
		for(int i = 0; i < height; i++) {
			std::string row;
			std::cin >> row;
#ifdef DEBUG
			std::cout << row << std::endl;
#endif
			std::vector<int> rowMasses(width, 0);
			for(int j = 0; j < width; j++) {
				rowMasses[j] = mass + (row[j] - '0');
			}
			masses.push_back(rowMasses);
		}

		int curBestRadius = 2;
		bool found = false;
		for(int centerX = 3; centerX <= 2 * width - 3; centerX++) {
			for(int centerY = 3; centerY <= 2 * height - 3; centerY++) {
				
				if((centerX % 2 == 0 && centerY % 2 == 1) ||
				   (centerX % 2 == 1 && centerY % 2 == 0)) {
					continue;
				}

				double centerXLoc = (double) centerX / 2.0;
				double centerYLoc = (double) centerY / 2.0;

#ifdef DEBUG
				std::cout << "Trying to center around (" << centerXLoc << ", " << centerYLoc << ")" << std::endl;
#endif
				int maxRadius = std::min(2 * width - centerX, 2 * height - centerY);
				maxRadius = std::min(maxRadius, centerX);
				maxRadius = std::min(maxRadius, centerY);

#ifdef DEBUG
				//std::cout << "Maximum radius: " << maxRadius << std::endl;
#endif
				std::vector<std::vector<std::pair<double, double> > > vecsToCenter;

				for(int y = 0; y < height; y++) {
					std::vector<std::pair<double, double> > row;
					for(int x = 0; x < width; x++) {
						double xLoc = (double) x + 0.5;
						double yLoc = (double) y + 0.5;

						row.push_back(std::make_pair((centerXLoc - xLoc) * (double) masses[y][x], (centerYLoc - yLoc) * (double) masses[y][x]));
					}
					vecsToCenter.push_back(row);
				}

				for(int radius = curBestRadius + 1; radius <= maxRadius; radius++) {
					if((centerX % 2 == 1 && radius % 2 == 0) ||
					   (centerX % 2 == 0 && radius % 2 == 1)) {
						continue;
					}

					std::pair<double, double> curVec = std::make_pair(0, 0);

					int minX = centerX / 2 - radius / 2;
					int maxX = centerX / 2 + radius / 2;
					int minY = centerY / 2 - radius / 2;
					int maxY = centerY / 2 + radius / 2;

					if(radius % 2 == 1) {
						maxX++;
						maxY++;
					}
#ifdef DEBUG
					std::cout << "  " << radius << ": from (" << minX << ", " << minY << ") to (" << maxX << ", " << maxY << ")" << std::endl;
#endif
					for(int x = minX; x < maxX; x++) {
						for(int y = minY; y < maxY; y++) {
							if((x == minX && y == minY) ||
							   (x == maxX - 1 && y == minY) ||
							   (x == minX && y == maxY - 1) ||
							   (x == maxX - 1 && y == maxY - 1)) {

								continue;
							}
							
#ifdef DEBUG
							//std::cout << "(" << x << ", " << y << "): (" << curVec.first << ", " << curVec.second << ") + (" << vecsToCenter[y][x].first << ", " << vecsToCenter[y][x].second << ") = (" << curVec.first + vecsToCenter[y][x].first << ", " << curVec.second + vecsToCenter[y][x].second << ")" << std::endl;
#endif
							curVec.first += vecsToCenter[y][x].first;
							curVec.second += vecsToCenter[y][x].second;

						}
					}

#ifdef DEBUG
					//std::cout << "Sum: (" << curVec.first << ", " << curVec.second << ")" << std::endl;
#endif
					if(std::abs(curVec.first) < .000000001 && std::abs(curVec.second) < .0000000001) {
#ifdef DEBUG
						std::cout << "Success!" << std::endl;
#endif
						curBestRadius = std::max(curBestRadius, radius);
						found = true;
					}
				}
			}
		}

		std::cout << "Case #" << testCaseNum + 1 << ": ";
		if(!found) {
			std::cout << "IMPOSSIBLE" << std::endl;
		} else {
			std::cout << curBestRadius << std::endl;
		}
	}

	return 0;
}
