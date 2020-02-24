# include <stdio.h>
# include <iostream>
# include <string>
# include <vector>

using namespace std;

#define INF 1 << 29

vector<string> dic;

typedef struct node {
  vector<int> adj;
  bool vis;
  int bowl;
};

vector<node> ing;
vector<int> order;

int find_include (string s){
  for (int i=0; i<dic.size(); i++)
    if (dic[i] == s) return i;
  dic.push_back(s);
  return dic.size()-1;
}

int visita (){

  vector<bool>bowls;

  for (int k=0; k<order.size(); k++){

    int i = order[k];
    ing[i].vis = true;

    //Verifica se algum filho nao foi visitado
    for (int j=0; j<ing[i].adj.size(); j++){
      node dst = ing[ing[i].adj[j]];
      if (dst.vis == false) return INF;
    }
    
     //Verifica se existe algum bowl vazio
     for (int j=0; j<bowls.size(); j++)
       if (bowls[j] == false){
 	bowls[j] = true;
 	ing[i].bowl = j;
 	goto achei;
       }
     bowls.push_back(1);
     ing[i].bowl = bowls.size()-1;
    
   achei:

    //Esvazia os bowls dos filhos
    for (int j=0; j<ing[i].adj.size(); j++){
      node dst = ing[ing[i].adj[j]];
      bowls[ dst.bowl ] = false;
    }
  }
  return bowls.size();
}

int main (){

  int t, n, cases = 1;
  string S, D;
  int src, dst;

  scanf("%d", &t);
  while (t--){
    
    scanf("%d", &n);

    ing = vector<node>(n);
    order = vector<int>(n);
    dic.clear();

    for (int i=0; i<n; i++){
      cin >> S;
      src = find_include(S);
      order[i] = src;

      ing[src].adj.clear();

      int m;
      scanf("%d",&m);
      for (int j=0; j<m; j++){
	cin >> D;
	if (D[0] >= 'A' && D[0] <= 'Z'){
	  dst = find_include(D);
	  ing[src].adj.push_back(dst);
	} 
      }
    }

    //Testa lista 
//     for (int i=0; i<n; i++){
//       cout << dic[i] << ": ";
//       for (int j=0; j<ing[i].adj.size(); j++)
// 	cout << dic[ing[i].adj[j]] << " " ;
//       cout << endl;
//     }
      

    int best = INF;

    do {

      for (int i=0; i<ing.size(); i++)
	ing[i].vis = false;

      best = min(best, visita());

    } while (next_permutation(order.begin(), order.end()));
    
    printf("Case #%d: %d\n", cases++, best);
    
  }
  
  return 0;
}
