#include <pthread.h>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <time.h>
using namespace std;

typedef pair<int,int> pii;

vector <unordered_set <int> > edges;
unordered_map <unsigned int, int> node_map;
vector <unordered_map <int, int> > cost_map;

void new_node (int x) {
	int s = node_map.size();
	node_map[x] = s;
	unordered_set <int> new_set;
	edges.push_back(new_set);
	unordered_map <int, int> cost;
	cost[s] = 0;
	cost_map.push_back(cost);
}

void read_input() {
	char line[100];
	while (gets(line)) {
		if (line[0] == 'S') {
			return;
		}
		unsigned int x, y;
		sscanf (line, "%u %u", &x, &y);
		if (node_map.find(x) == node_map.end()) {
			new_node(x);
		}
		if (node_map.find(y) == node_map.end()) {
			new_node(y);
		}
		edges[node_map[x]].insert(node_map[y]);
	}
}

void query(pii *p) {
	if (cost_map[p->first].find(p->second) == cost_map[p->first].end()) {
		printf ("-1\n");
	} else {
		printf ("%d\n", cost_map[p->first][p->second]);
	}
}

void ssp (int src) {

	queue <int> q;
	unordered_set <int> vis;
	q.push(src);
	vis.insert(src);
	cost_map[src].clear();
	cost_map[src][src] = 0;

	unordered_set <int> :: iterator it;

	while (!q.empty()) {
		int c = q.front();
		q.pop();

		for (it = edges[c].begin(); it != edges[c].end(); it++) {
			int nxt = *it;
			if (vis.find(nxt) == vis.end()) {
				vis.insert(nxt);
				q.push(nxt);
				cost_map[src][nxt] = cost_map[src][c]+1;
			}
		}
	}
}

void add_edge (int x, int y, int ind) {
	unordered_map <unsigned int, int> :: iterator it;
	if (cost_map[ind].find(x) != cost_map[ind].end()) {
		for (it = node_map.begin(); it != node_map.end(); it++) {
			int ind2 = it->second;
			if (cost_map[y].find(ind2) != cost_map[y].end()) {
				if (cost_map[ind].find(ind2) == cost_map[ind].end() || cost_map[ind][ind2] > cost_map[ind][x] + cost_map[y][ind2] + 1) {
					cost_map[ind][ind2] = cost_map[ind][x] + cost_map[y][ind2] + 1;
				}
			}
		}
	}
}

void remove_edge (int x, int y, int ind) {
	unordered_map <unsigned int, int> :: iterator it;
	for (it = node_map.begin(); it != node_map.end(); it++) {
		int ind2 = it->second;
		if (cost_map[ind].find(ind2) != cost_map[ind].end()) {
			if (cost_map[ind].find(x)  != cost_map[ind].end() &&
					cost_map[y].find(ind2) != cost_map[y].end()) {
				if (cost_map[ind][ind2] == cost_map[ind][x] + cost_map[y][ind2] + 1) {
					ssp (ind);
					return;
				}
			}
		}
	}
}

int main () {

	clock_t t1, t2;

	t1 = clock();

	freopen ("graph3.txt", "r", stdin);

	read_input();
	unordered_map <unsigned int, int> :: iterator it;

	for (it = node_map.begin(); it != node_map.end(); it++) {
		int ind = it->second;
		ssp(ind);
	}

	printf ("R\n");

	char line[100];

	while (gets(line)) {
		if (line[0] == 'F') {
			break;
		}
		char command;
		int x, y;
		sscanf (line, "%c %u %u", &command, &x, &y);
		if (node_map.find(x) == node_map.end()) {
			new_node(x);
		}
		if (node_map.find(y) == node_map.end()) {
			new_node(y);
		}
		if (command == 'Q') {
			pii p = pii (node_map[x], node_map[y]);
			query (&p);
		} else if (command == 'A') {
			if (edges[node_map[x]].find(node_map[y]) == edges[node_map[x]].end()) {
				edges[node_map[x]].insert(node_map[y]);
				for (it = node_map.begin(); it != node_map.end(); it++) {
					add_edge (node_map[x], node_map[y], it->second);
				}
			}
		} else if (command == 'D') {
			if (edges[node_map[x]].find(node_map[y]) != edges[node_map[x]].end()) {
				edges[node_map[x]].erase(node_map[y]);
				for (it = node_map.begin(); it != node_map.end(); it++) {
					remove_edge(node_map[x], node_map[y], it->second);
				}
			}
		} else if (command == 'F') {
			break;
		} else {
			printf ("INVALID COMMAND\n");
		}
	}

	t2 = clock();
	float diff ((float)t2-(float)t1);
	cout << diff/CLOCKS_PER_SEC << endl;

	return 0;
}