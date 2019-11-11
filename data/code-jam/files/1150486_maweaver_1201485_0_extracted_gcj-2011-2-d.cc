#include <algorithm>
#include <iostream>
#include <limits>
#include <vector> 

template<class T, class C>
class BinHeap {
public:
	BinHeap(const C &comp) :
		_size(0), _vec(std::vector<T*>()), _comp(comp)
	{ }
	
	int size() const {
		return _size;
	}
	
	bool isEmpty() const {
		return _size == 0;
	}
	
	void push(T& value) {
		if(_vec.size() < _size + 1) {
			_vec.resize((_size + 1) * 2);
		}
		_vec[_size] = &value;
		value.heapIdx = _size;
		
		_size++;
		
		siftUp(_size - 1);
	}
	
	T& top() {
		return *_vec[0];
	}
	
	T& pop() {
		T *val = _vec[0];
		
		_vec[0] = _vec[_size - 1];
		_vec[0]->heapIdx = 0;
		_size--;
		
		siftDown(0);
		
		return *val;
	}
	
	void siftUp(int pos) {
		while(pos > 0) {
			int parPos = (pos - 1) / 2;
			
			if(_comp(*_vec[pos], *_vec[parPos])) {
				T *t = _vec[pos];
				_vec[pos] = _vec[parPos];
				_vec[parPos] = t;
				_vec[pos]->heapIdx = pos;
				_vec[parPos]->heapIdx = parPos;
				
				pos = parPos;
			} else {
				break;
			}
		}
	}
	
	void siftDown(int pos) {
		int smallest = pos;
		int c1 = pos * 2 + 1;
		int c2 = pos * 2 + 2;
		
		if(c1 < _size && _comp(*_vec[c1], *_vec[smallest])) {
			smallest = c1;
		}
		
		if(c2 < _size && _comp(*_vec[c2], *_vec[smallest])) {
			smallest = c2;
		}
		
		if(smallest != pos) {
			T *t = _vec[smallest];
			
			_vec[smallest] = _vec[pos];
			_vec[smallest]->heapIdx = smallest;
			
			_vec[pos] = t;
			_vec[pos]->heapIdx = pos;
			
			siftDown(smallest);
		}
	}
	
	int _size;
	std::vector<T*> _vec;
	
	const C &_comp;
};


struct Node {

	Node() : dist(std::numeric_limits<int>::max()), heapIdx(-1), goal(false), neighbors() { }

	int dist;
	int heapIdx;
	bool goal;
	std::vector<int> neighbors;

};

struct NodeDistComp {
	bool operator()(const Node &a, const Node &b) const {
		return a.dist < b.dist;
	}
};

struct NodeNumThreatenedComp {
	NodeNumThreatenedComp(const std::vector<Node> &_nodes, const std::vector<bool> &_threatened, const std::vector<bool> &_visited) :
		nodes(_nodes), threatened(_threatened), visited(_visited) { }

	const std::vector<Node> &nodes;
	const std::vector<bool> &threatened;
	const std::vector<bool> &visited;

	bool operator()(const int &a, const int &b) {
		int numAThreatens = 0;
		for(std::vector<int>::const_iterator neighborsIt = nodes[a].neighbors.begin();
		    neighborsIt != nodes[a].neighbors.end(); ++neighborsIt) {
			
			if(!visited[*neighborsIt] && !threatened[*neighborsIt]) {
				numAThreatens++;
			}
		}

		int numBThreatens = 0;
		for(std::vector<int>::const_iterator neighborsIt = nodes[b].neighbors.begin();
		    neighborsIt != nodes[b].neighbors.end(); ++neighborsIt) {
			
			if(!visited[*neighborsIt] && !threatened[*neighborsIt]) {
				numBThreatens++;
			}
		}

		return numAThreatens > numBThreatens;
	}
};

int search(const std::vector<Node> &nodes, const std::vector<bool> &threatened, const std::vector<bool> &visited, int curNode, int distLeft) {

	if(distLeft == 0) {
		if(nodes[curNode].goal) {
			int numThreatened = 0;
			for(int i = 0; i < (int) threatened.size(); i++) {
				if(threatened[i] && !visited[i]) {
					numThreatened++;
				}
			}
			return numThreatened;
		} else {
			return -1;
		}
	}

	std::vector<int> next;
	for(std::vector<int>::const_iterator neighborsIt = nodes[curNode].neighbors.begin();
	    neighborsIt != nodes[curNode].neighbors.end(); ++neighborsIt) {
		
		if(!visited[*neighborsIt]) {
			next.push_back(*neighborsIt);
		}
	}

	std::sort(next.begin(), next.end(), NodeNumThreatenedComp(nodes, threatened, visited));

	int best = -1;
	for(std::vector<int>::const_iterator nextIt = next.begin();
	    nextIt != next.end(); ++nextIt) {
		
		std::vector<bool> nextThreatened = threatened;
		for(std::vector<int>::const_iterator neighborsIt = nodes[*nextIt].neighbors.begin();
		    neighborsIt != nodes[*nextIt].neighbors.end(); ++neighborsIt) {
			
			nextThreatened[*neighborsIt] = true;
		}

		std::vector<bool> nextVisited = visited;
		nextVisited[*nextIt] = true;

		best = std::max(best, search(nodes, nextThreatened, nextVisited, *nextIt, distLeft - 1));
	}

	return best;
}

int main(int arc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int numPlanets, numWormholes;
		std::cin >> numPlanets >> numWormholes;

		std::vector<Node> nodes(numPlanets, Node());

		for(int i = 0; i < numWormholes; i++) {
			int p1, p2;
			std::cin >> p1;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), ',');
			std::cin >> p2;
			nodes[p1].neighbors.push_back(p2);
			nodes[p2].neighbors.push_back(p1);
		}

		std::vector<int> goals;

		for(std::vector<Node>::iterator nodesIt = nodes.begin();
		    nodesIt != nodes.end(); ++nodesIt) {
			
			for(std::vector<int>::const_iterator neighborsIt = nodesIt->neighbors.begin();
			    neighborsIt != nodesIt->neighbors.end(); ++neighborsIt) {

				if(*neighborsIt == 1) {
					nodesIt->goal = true;
					goals.push_back(*neighborsIt);
				}
				
			}

		}

		nodes[0].dist = 0;
		BinHeap<Node, NodeDistComp> queue = BinHeap<Node, NodeDistComp>(NodeDistComp());
		queue.push(nodes[0]);

		int bestDist = 0;
		while(!queue.isEmpty()) {
			Node &cur = queue.pop();
			if(cur.goal) {
				bestDist = cur.dist;
				break;
			}

			for(std::vector<int>::const_iterator neighborsIt = cur.neighbors.begin();
			    neighborsIt != cur.neighbors.end(); ++neighborsIt) {
				
				if(cur.dist + 1 < nodes[*neighborsIt].dist) {
					nodes[*neighborsIt].dist = cur.dist + 1;
					if(nodes[*neighborsIt].heapIdx == -1) {
						queue.push(nodes[*neighborsIt]);
					} else {
						queue.siftUp(nodes[*neighborsIt].heapIdx);
					}
				}
			}
		}

		std::vector<bool> threatened(numPlanets, false);
		for(std::vector<int>::const_iterator neighborsIt = nodes[0].neighbors.begin();
		    neighborsIt != nodes[0].neighbors.end(); ++neighborsIt) {
			
			threatened[*neighborsIt] = true;
		}
		std::vector<bool> visited(numPlanets, false);
		visited[0] = true;

		int numThreatened = search(nodes, threatened, visited, 0, bestDist);


		std::cout << "Case #" << testCaseNum + 1 << ": " << bestDist << " " << numThreatened << std::endl;
	}

	
	return 0;
}
