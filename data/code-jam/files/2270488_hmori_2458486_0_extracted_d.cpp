#include <iostream>
#include <set>
#include <map>
#include <vector>
using namespace std;

class Chest{
public:
  multiset<int> keys;
  int type;
};

class Node{
public:
  multiset<int> currentKeys;
  set<int> openedChests;
  vector<int> history;
  set<int> historySet;
  Node(multiset<int>& keys):currentKeys(keys){}
  Node(multiset<int>& keys, set<int> _openedChests):currentKeys(keys),openedChests(_openedChests){}
  Node(Node* node):currentKeys(node->currentKeys),openedChests(node->openedChests),history(node->history),historySet(node->historySet){}
};


void emitHistory(vector<int> his){
  for(int i = 0; i < (int)his.size(); i++){
    cout << his[i]+1;
    if(i != his.size() -1){
      cout << ' ';
    }
  }
}

set<set<int> > tabuSet;
bool BFS(multiset<int>& initialKeys, map<int, Chest*>& chests, Node* tmpNode){
  vector<Node*> stack;
  // open leaves from the node
  for(int c = 0; c < (int)chests.size(); c++){
    // c is not opend && c can be opened with currentKeys
    Node* newNode;
    if(tmpNode->openedChests.find(c) == tmpNode->openedChests.end() // c is not opened
       && tmpNode->currentKeys.find(chests[c]->type) != tmpNode->currentKeys.end()){ // c can be opened
      newNode = new Node(tmpNode);
      // expand node
      multiset<int>::iterator it = newNode->currentKeys.find(chests[c]->type);
      newNode->currentKeys.erase(it);// key consumption
      newNode->openedChests.insert(c); // openedChest insertion
      for(multiset<int>::iterator it = chests[c]->keys.begin(); it != chests[c]->keys.end(); ++it){
	newNode->currentKeys.insert(*it); // key obtain
      }
      newNode->history.push_back(c); // history insertion
      newNode->historySet.insert(c); // history insertion
      // check tabulist
      if(tabuSet.find(newNode->historySet) != tabuSet.end()){
	break;
      }
      // check solved
      if(newNode->history.size() == chests.size()){
	emitHistory(newNode->history);
	cout << endl;
	return true;
      }else if(BFS(initialKeys, chests,newNode)){
	return true;
      }
    } 
  }
  tabuSet.insert(tmpNode->historySet);
  delete tmpNode;
  return false;
}


void solve(multiset<int>& initialKeys, map<int, Chest*>& chests, int t){
  // DFS accoding to chestNO order
  // node (state) = {currentKeys (multiset<int>), openedChests (set<int>)}
  
  // check # of keys and chests
  multiset<int> totalKeys(initialKeys);
  for(int c = 0; c < (int)chests.size(); c++){
    for(multiset<int>::iterator it = chests[c]->keys.begin(); it != chests[c]->keys.end(); ++it){
      totalKeys.insert(*it); // key obtain
    }
  }
  bool solvable = true;
  // check sufficiency
  for(int c = 0; c < (int)chests.size(); c++){
    if(totalKeys.find(chests[c]->type) == totalKeys.end()){
      solvable = false;
      break;
    }
    multiset<int>::iterator it = totalKeys.find(chests[c]->type);
    totalKeys.erase(it);// key consumption
  }
  

  cout << "Case #" << t+1 << ": ";
  if(!solvable){
    cout << "IMPOSSIBLE" << endl;
  }
  else if(!BFS(initialKeys, chests, new Node(initialKeys))){
    cout << "IMPOSSIBLE" << endl;
  }
}


int main(){
  int T;
  cin >> T;
  for(int t = 0; t < T; t++){
    tabuSet.clear();
    int K, N;
    cin >> K >> N;
    multiset<int> initialKeys;
    for(int k = 0; k < K; k++){
      int key;
      cin >> key;
      initialKeys.insert(key);
    }
    map<int, Chest*> chests;
    for(int n = 0; n < N; n++){
      Chest* chest = new Chest();
      cin >> chest->type;
      int numKeys;
      cin >> numKeys;
      for(int k = 0; k < numKeys; k++){
	int tmpKey;
	cin >> tmpKey;
	chest->keys.insert(tmpKey);
      }
      chests[n]=chest;
    }


    solve(initialKeys, chests, t);



  }
}
