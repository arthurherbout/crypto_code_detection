#include<iostream>
#include<list>
#include<algorithm>
#include<stack>
#include<vector>
using namespace std;
vector <vector <int>(2,0)> starr;



class node
{
public:
  int startval;
  int endval;

  bool contains(int v)
  {
    if((startval <= v) && (endval >= v))
      {
	return true;
      }
    return false;
  }

  bool issuperset(node* p)
  {
    if((startval <= p->startval) && (endval >= p->endval))
      {
	return true;
      }
    return false;
  }

};



class range
{
public :
  list<node> rlist;
  int min;
  int max;

  //to be called after min and max are set.
  void initialize()
  {
    node t;
    t.startval = min;
    t.endval = max;
    rlist.push_back(t);
  }

  void printranges()
  {
    list<node>::iterator itr;
    for(itr = rlist.begin();itr != rlist.end();itr++)
      {
	cout<<(*itr).startval<<" "<<(*itr).endval<<endl;
      }

  }

  void andinterval(node t)
  {
    list<node>::iterator itr;

    for(itr = rlist.begin();itr != rlist.end();itr++)
      {

 	//checking for no-intersection-possible-any-further.
	if(t.endval < (*itr).startval)
	  {
	    return;
	  }

	if((*itr).issuperset(&t))
	  {
	    (*itr) = t;
	    return;
	  }

	if((*itr).contains(t.startval))
	  {
	    (*itr).startval = t.startval;
	  }
	
	if((*itr).contains(t.endval))
	  {
	    (*itr).endval = t.endval;
	  }
      }
    return;
  }


  //min to l and r to max
  void removeinterval(node t)
  {
    list<node>::iterator itr;

    for(itr = rlist.begin();itr != rlist.end();itr++)
      {


 	//checking for no-intersection-possible-any-further.
	if((*itr).startval  > t.endval)
	  {
	    return;
	  }

	//no effect condition.
	if((*itr).endval < t.startval)
	  {
	    continue;
	  }

	//remove all intervals that are subsets of t
	if(t.issuperset(&(*itr)))
	  {
	    rlist.erase(itr);
	    continue;
	  }

	//contains condition. have to break the node into two.
	if((*itr).issuperset(&t))
	  {
	    node temp;
	    temp.startval = (*itr).startval;
	    temp.endval = t.startval;
	    (*itr).startval = t.endval;
	    rlist.insert(itr,temp);
	    return;
	  }

	if((*itr).contains(t.startval))
	  {
	    (*itr).endval = t.startval;
	  }

	if((*itr).contains(t.endval))
	  {
	    (*itr).startval = t.endval;
	  }
      }
  }
};





class bytelandian
{
public:
  range pvals;
  int truthful;
  int knights_so_far;
  bool forced;

  bytelandian();
  bool icanspeak(bool x);
};

bytelandian::bytelandian()
{
  truthful = 1;
  knights_so_far = 0;
  forced = false;
}

//should not be called without initializing pvals
bool icanspeakthetruth()
{






main()
{
  int num_of_testcases;
  cin >>num_of_testcases;

  int num_of_bytelandians;

  while(num_of_testcases-- > 0)
    {
      cin>>num_of_bytelandians;
      starr.clear();

      for(int i = 0;i<num_of_bytelandians;i++)
	{
	  vector<int> tmp;
	  int tm;
	  cin>>tm;
	  tmp.push_back(tm);
	  cin>>tm;
	  tmp.push_back(tm);
	  starr.push_back(tmp);
	}



      bytelandian first;
      first.pvals.min = 0;
      first.pvals.max = num_of_bytelandians;
      first.pvals.initialize();
      first.icanspeak(
      
      stack<bytelandian> stk;



