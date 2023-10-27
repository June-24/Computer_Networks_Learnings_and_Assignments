#include <bits/stdc++.h>
using namespace std;
vector<vector<string>> ans;
void findCycle(map<string, vector<string>> &mp, string k, set<string> &recursive_stack, vector<string> potential_deadlock_path)
{
	recursive_stack.insert(k);
	potential_deadlock_path.push_back(k);
	for (auto i : mp[k])
	{
		if (recursive_stack.find(i) == recursive_stack.end())
		{
			findCycle(mp, i, recursive_stack, potential_deadlock_path);
		}
		else if (recursive_stack.find(i) != recursive_stack.end())
		{
			if (!(potential_deadlock_path.size() == 2 || i[0] == 'r'))
				ans.push_back(potential_deadlock_path);
		}
	}
	potential_deadlock_path.pop_back();
	recursive_stack.erase(k);
}
int main()
{
	int n;
	cout << "Enter the no of processes: ";
	cin >> n;
	int m;
	cout << "Enter the no of resources: ";
	cin >> m;
	map<string, vector<string>> mp;
	set<string> processes, resources;
	for (int i = 0; i < n; i++)
	{
		string s;
		cout << "Enter the process: ";
		cin >> s;
		processes.insert(s);
		int k;
		cout << "Enter the no of resources required by it: ";
		cin >> k;
		cout << "Enter the resources: ";
		while (k--)
		{
			string r;
			cin >> r;
			resources.insert(r);
			mp[s].push_back(r);
		}
	}
	for (int i = 0; i < m; i++)
	{
		string s;
		cout << "Enter the resource: ";
		cin >> s;
		resources.insert(s);
		string k1;
		cout << "Enter the process allocated to it: ";
		cin >> k1;
		processes.insert(k1);
		mp[s].push_back(k1);
	}
	set<string> vis, temp;
	for (auto i : processes)
	{
		temp.insert(i);
		vis.insert(i);
	}
	for (auto i : resources)
	{
		temp.insert(i);
		vis.insert(i);
	}
	while (!vis.empty())
	{
		string i = *vis.begin();
		vis.erase(i);
		if (i[0] != 'p')
			continue;
		set<string> recursive_stack;
		vector<string> potential_deadlock_path;
		findCycle(mp, i, recursive_stack, potential_deadlock_path);
	}
	cout << "Deadlocks are: " << endl;
	for (int i = 0; i < ans.size(); i++)
	{
		cout << i + 1 << " : ";
		for (int j = 0; j < ans[i].size(); j++)
		{
			cout << ans[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
/*
2
2
p1
1
r1
p2
1
r2
r1
p2
r2
p1
*/
