#include <bits/stdc++.h>
#include <sstream>
using namespace std;

struct node
{
	string name;
};

vector<node> graph;
int pn,rn;
vector<int> resourcetaken;
vector<vector<int> > cyclepaths;

bool checkCycle(int node, vector < int > adj[], int vis[], int dfsVis[] , string str) 
{
	cout<<node<<"  ";
      vis[node] = 1;
      dfsVis[node] = 1;
      for (auto it: adj[node]) 
      {
      	str += graph[it].name;
      	 str += " ";
        if (!vis[it]) 
        {	
          if (checkCycle(it, adj, vis, dfsVis , str)) 
          	return true;
        } 
        else if (dfsVis[it]) 
        {
          cout<<"Cycle is detected !!!"<<endl;
          cout<<"THe cycle is : "<<str<<endl;
          
          stringstream s(str); string word;
          vector<int> apath;
          while (s>>word)
          {
		if (word[0]=='r')
		{
			int index = stoi(word.substr(1));
			resourcetaken[index]++;
			apath.push_back(index);
		}
	  }
	  cyclepaths.push_back(apath);
          return true;
        }
      }
      dfsVis[node] = 0;
      return false;
}

bool isCyclic(int N, vector < int > adj[]) 
{
      int vis[N], dfsVis[N];
     
      for(int i = 0; i < N; i++)
      {
        vis[i] = 0;
        dfsVis[i] = 0;
      }
	
      for (int i = 0; i < N; i++) 
      {
        if (!vis[i]) 
        {	string str;
          checkCycle(i, adj, vis, dfsVis , str);
          cout<<endl;
        }
      }
      
      return false;
}

    

int main ()
{
	
	cout<<"Enter total processes and total recuources "<<endl;
	cin>>pn>>rn;
	resourcetaken.resize(rn);
	graph.resize(pn+rn);
	vector<int> resourcecheck(rn);
	cout<<"Enter total instances of resources : "<<endl;
	for (int i=0;i<rn;i++)
	{
		cin>>resourcecheck[i];
	}
	
	vector<int> edges[pn+rn];
	
	for (int i=0;i<pn;i++)
		graph[i].name = "p"  + to_string(i);
	for (int i=pn;i<pn+rn;i++)
		graph[i].name = "r" + to_string(i-pn);
		
	cout<<"Enter total relations between processes and recources : "<<endl;
	int r; cin>>r;
	for (int i=0;i<r;i++)
	{
		int type; cin>>type; string a,b;
		if (type==1)  // process to relation
		{
			cin>>a>>b;
			edges[stoi(a.substr(1))].push_back(stoi(b.substr(1))+pn);
		}
		else	     // relation to process 
		{
			cin>>a>>b;
			edges[stoi(a.substr(1))+pn].push_back(stoi(b.substr(1)));
		}
	}
	
	
	isCyclic(pn+rn , edges);
	int flag = 0;
	
	for (int i=0;i<cyclepaths.size();i++)
	{
		for (auto j : cyclepaths[i])
		{
			if (resourcecheck[j]!=resourcetaken[j])
			{
				continue;
			}
		}
		flag = 1;
		break;
	}
	
	if (flag)
		cout<<"It contains deadlock "<<endl;
	else
		cout<<"It contains deadlock "<<endl;
}

/*
10
1
p0 r0
1
p1 r1
1
p2 r2
1
p3 r3
2
r1 p0
2
r0 p1
2
r0 p4
2
r0 p2
2
r3 p2
2
r2 p3
*/