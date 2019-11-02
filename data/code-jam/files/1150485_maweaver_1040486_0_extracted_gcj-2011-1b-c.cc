#include <algorithm>
#include <iostream>
#include <vector>

int posMod(int num, int m) {
	while(num < 0) {
		num += m;
	}

	return num % m;
}

struct Node {
	int color;
	std::vector<int> ns;
};

struct NeighborComp {

	NeighborComp(int _cur, int _n) : cur(_cur), n(_n) { }

	int cur;
	int n;

	bool operator()(const int &a, const int &b) const {
		int aDist = posMod(a - cur, n);
		int bDist = posMod(b - cur, n);

		return aDist < bDist;
	}

};

bool valid(std::vector<Node> &nodes, const std::vector<std::vector<int> > &rooms, int numColors) {
	for(std::vector<std::vector<int> >::const_iterator roomsIt = rooms.begin();
	    roomsIt != rooms.end(); ++roomsIt) {
		
		std::vector<bool> found(numColors, false);

		for(std::vector<int>::const_iterator roomIt = roomsIt->begin();
		    roomIt != roomsIt->end(); ++roomIt) {
			
			found[nodes[*roomIt].color] = true;
		}

		for(int i = 0; i < numColors; i++) {
			if(!found[i]) {
				return false;
			}
		}
	}
	return true;
}

bool search(std::vector<Node> &nodes, const std::vector<std::vector<int> > &rooms, int numColors, int pos) {
	if(pos < (int) nodes.size()) {
		for(int i = 0; i < numColors; i++) {
			nodes[pos].color = i;
			if(search(nodes, rooms, numColors, pos + 1)) {
				return true;
			}
		}
		return false;
	} else {
		return valid(nodes, rooms, numColors);
	}
}

int main(int arc, char **argv) {
	
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
#ifdef DEBUG
		std::cout << "----------" << std::endl;
#endif

		int numVerts, numWalls;
		std::cin >> numVerts >> numWalls;

		std::vector<Node> nodes(numVerts);
		for(int i = 0; i < numVerts; i++) {
			nodes[i].ns.push_back(posMod(i + 1, numVerts));
			nodes[i].ns.push_back(posMod(i - 1, numVerts));
		}

		std::vector<std::pair<int, int> > walls(numWalls);
		for(int i = 0; i < numWalls; i++) {
			int u;
			std::cin >> u;
			walls[i].first = u;
		}

		for(int i = 0; i < numWalls; i++) {
			int v;
			std::cin >> v;
			walls[i].second = v;
		}

		for(std::vector<std::pair<int, int> >::const_iterator wallsIt = walls.begin();
		    wallsIt != walls.end(); ++wallsIt) {
			
			nodes[wallsIt->first - 1].ns.push_back(wallsIt->second - 1);
			nodes[wallsIt->second - 1].ns.push_back(wallsIt->first - 1);
		}

		for(int i = 0; i < numVerts; i++) {
			std::sort(nodes[i].ns.begin(), nodes[i].ns.end(), NeighborComp(i, numVerts));
		}

#ifdef DEBUG
		for(int i = 0; i < numVerts; i++) {
			std::cout << i << " " << i << std::endl;
		}
		std::cout << "#" << std::endl;
#endif
		for(int i = 0; i < numVerts; i++) {
			for(std::vector<int>::const_iterator nsIt = nodes[i].ns.begin();
			    nsIt != nodes[i].ns.end(); ++nsIt) {
#ifdef DEBUG
				std::cout << i << " " << *nsIt << " ";
#endif
			}
#ifdef DEBUG
			std::cout << std::endl;
#endif
		}

		std::vector<std::vector<int> > rooms;

		for(int i = 0; i < numVerts; i++) {
			for(int j = 0; j < (int) nodes[i].ns.size(); j++) {
				int cur = nodes[i].ns[j];
				int prev = i;

				if(cur == posMod(i - 1, numVerts)) {
					continue;
				}

				std::vector<int> room;
				room.push_back(i);
#ifdef DEBUG
				std::cout << prev << " -> " << cur <<  ": ";
#endif
				while(cur != i) {
#ifdef DEBUG
					std::cout << cur << " ";
#endif
					room.push_back(cur);
					for(int k = 0; k < (int) nodes[cur].ns.size(); k++) {
						if(nodes[cur].ns[k] == prev) {
							prev = cur;
							cur = nodes[prev].ns[posMod(k - 1, nodes[prev].ns.size())];
							break;
						}
					}
				}
#ifdef DEBUG
				std::cout << std::endl;
#endif
				std::sort(room.begin(), room.end());
				rooms.push_back(room);
			}
		}

		std::sort(rooms.begin(), rooms.end());

		std::vector<std::vector<int> > realRooms;
		for(int i = 0; i < (int) rooms.size(); i++) {
			if(i == 0 || rooms[i] != rooms[i - 1]) {
				realRooms.push_back(rooms[i]);
			}
		}

		rooms = realRooms;

#ifdef DEBUG
		for(std::vector<std::vector<int> >::const_iterator roomsIt = rooms.begin();
		    roomsIt != rooms.end(); ++roomsIt) {
			std::cout << "[ ";
			for(std::vector<int>::const_iterator roomIt = roomsIt->begin();
			    roomIt != roomsIt->end(); ++roomIt) {
				std::cout << *roomIt << " ";
			}
			std::cout << "]" << std::endl;
		}
#endif
		std::vector<Node> best;
		int numColors = 1;
		while(search(nodes, rooms, numColors, 0)) {
			best = nodes;
			numColors++;
		}

		std::cout << "Case #" << testCaseNum + 1 << ": " << numColors - 1 << std::endl;
		for(int i = 0; i < numVerts; i++) {
			std::cout << best[i].color + 1;
			if(i != numVerts - 1) {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}


	return 0;
}
