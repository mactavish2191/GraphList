#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <iterator>
#include <limits>
#include <iomanip>

using namespace std;
constexpr int INF = numeric_limits<int>::max();
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;


//Comparator functions and classes
struct sortDistance {
	bool operator() (const pii& lhs, const pii& rhs) {
		return lhs.second > rhs.second;
	}
};



//Adjacency list definition
class GraphList {
private:
	int m_V;  //vertices
	int m_E = 0;  //edges
	bool DAG = false; // to check if the graph is a Directed Graph

	//0 index means null pointer in Graph structure pointing to nothing. N -> null
	map<int, vector<pair<int, int>> > Adj; //for storing undirected and digraph
	vector<int> indegree; // for storing indegree data


	//DFS Utility private function
	void DFS_PathTrack(vector<bool>& visited, int index) {
		//mark the current node as visted and print the node 
		visited[index] = true;
		cout << index << " ";

		//Recur for all the vertices adjacent to the vertex
		for (const auto& i : Adj[index]) {
			if (visited[i.first] == false)
				DFS_PathTrack(visited, i.first);
		}
	}

public:

	GraphList(int V = 0) :m_V(V)
	{
		if (V == 0) {
			cout << "Graph cannot be created. \n";
			exit(0);
		}
		indegree.assign(V + 1, 0);
	}

	void AddEdge(int a, int b) { //undirected edge graph
		if ((a > m_V || b > m_V) || (a < 0 || b < 0)) {
			cout << "The vertex does not exits \nPlease enter new value \n";
		}
		else if (a == 0 || b == 0) {
			if (a == 0) {
				Adj[b].push_back({ 0, 0 });
				m_E++;
			}
			else {
				Adj[a].push_back({ 0, 0 });
				m_E++;
			}
		}
		else {
			Adj[a].push_back({ b, 0 });
			Adj[b].push_back({ a, 0 });
			m_E++;
		}
	}

	void AddDiEdge(int a, int b, int weight) { //weighted directed edge graph
		if ((a > m_V || b > m_V) || (a < 0 || b < 0)) {
			cout << "The vertex does not exits \nPlease enter new value \n";
		}
		else if (a == 0 || b == 0) {
			if (a == 0) {
				Adj[b].push_back({ 0, 0 });
				m_E++;
			}
			else {
				Adj[a].push_back({ 0, 0 });
				m_E++;
			}
			DAG = true;
		}
		else {
			Adj[a].push_back({ b, weight });
			m_E++;
			indegree[b]++;
			DAG = true;
		}
	}

	void PrintUnDiGraph() {
		if (m_V == 0) {
			cout << "No graph vertices are available\n";
		}
		else {
			for (const auto& x : Adj) {
				cout << x.first;
				for (const auto& y : x.second) {
					cout << "->" << y.first;
				}
				cout << "\n";
			}
		}
	}

	void PrintDiGraph() {
		if (m_V == 0) {
			cout << "No graph vertices are available\n";
		}
		else {
			for (const auto& x : Adj) {
				cout << x.first;
				for (const auto& y : x.second) {
					cout << "-> " << y.first << ":" << y.second;
				}
				cout << "\n";
			}
		}
	}

	void BreadthFirstSearch() {
		vector<bool> visited(m_V + 1, false); //start with index 1
		cout << "BFS Traversal order: ";

		for (int i = 1; i < m_V + 1; i++) {
			if (Adj.find(i) != Adj.end()) {
				if (visited[i] == false) {
					queue<int> Q;
					Q.push(i);
					visited[i] = true;

					while (Q.empty() != true) {
						int key = Q.front(); Q.pop();
						cout << key << " ";

						for (const auto& node : Adj[key]) {
							if (visited[node.first] == false) {
								Q.push(node.first);
								visited[node.first] = true;
							}
						}

					}//end while
				}
			}
		}//end for loop for all vertices

		cout << "\n";
	}

	void DepthFirstSearch() {
		vector<bool> visited(m_V + 1, false); //start with index 1
		cout << "DFS Traversal order: ";

		//It enables to travel the whole set of graph even if the nodes are not connected
		for (int i = 1; i < m_V + 1; i++) {
			if (Adj.find(i) != Adj.end()) {
				if (visited[i] == false)
					DFS_PathTrack(visited, i);
			}
		}
		cout << "\n";
	}

	void DepthFirstSearch_Loop() {
		vector<bool> visited(m_V + 1, false); //start with index 1
		cout << "DFS Traversal Loop order: ";

		for (int i = 1; i < m_V + 1; i++) {
			if (Adj.find(i) != Adj.end()) {
				if (visited[i] == false) {
					stack<int> S;
					S.push(i);

					visited[i] = true;

					while (S.empty() != true)
					{
						int key = S.top(); S.pop();
						cout << key << " ";

						vector<pair<int, int>>::reverse_iterator ritr;
						for (ritr = Adj[key].rbegin(); ritr != Adj[key].rend(); ritr++) {
							if (visited[ritr->first] == false) {
								S.push(ritr->first);
								visited[ritr->first] = true;
							}
						}
					}// end while
				}// end if 
			}
		}//end for
		cout << "\n";
	}

	void TopSort() {
		if (DAG == true) {
			queue<int> q;
			int counter = 0;
			
			//copy all the vertex with indegree 0 into the queue
			for (size_t i = 1; i < indegree.size(); i++) {
				if (indegree[i] == 0) {
					q.push(i);
				}
			}

			cout << "Top Sort Ordering: ";
			while (q.empty() != true) {
				int v = q.front(); q.pop();
				cout << v << " ";
				counter++;
				
				for (auto& val : Adj[v]) {
					if (--indegree[val.first] == 0) {
						q.push(val.first);
					}
				}
			}
			cout << "\n";

			if (counter != m_V) {
				cout << "Cycle Found in Graph!.\n";
			}
		}
		else
			cout << "Cannot perform a Topological Sort on a Undirected Graph\n";
	}

	int FindComponents() { //count the no.of connected componenets
		vector<bool> visited(m_V + 1, false); //start with index 1
		int count = 0;

		for (int i = 1; i < m_V + 1; i++) {
			if (Adj.find(i) != Adj.end()) {
				if (visited[i] == false) {
					count++;
					DFS_PathTrack(visited, i);
				}
			}
		}
		cout << "\n";
		cout << "Connected components: ";
		return count;
	}

	void UnweightedShortestPath(int start, int end) {
		if (start <= 0 && start > m_V) {
			cout << "Please check the start vertex\n";
		}
		else if (end <= 0 && end > m_V) {
			cout << "Please check the end vertex\n";
		}
		else {
			cout << "Running Unweighted Shortest Path Algorithm......\n";
			vector<int> distance(m_V + 1, INF); //starts with index 1
			vector<int> path(m_V + 1, INF); //starts with index 1
			queue<int> Q;
			Q.push(start);
			distance[start] = 0;
			bool flag = false;

			while (Q.empty() != true) {
				int idx = Q.front(); Q.pop();
				for (const auto& node : Adj[idx]) { //check the surrounding nodes
					if (distance[node.first] == INF) { //if the node is not visited then visit and update the distance
						distance[node.first] = distance[idx] + 1; 
						path[node.first] = idx;
						Q.push(node.first);

						if (node.first == end) {
							flag = true;
							break;
						}

					}
				}
				if (flag == true)
					break;
			}

			//cout << "Distance :";
			//for (int i = 0; i <= m_V; i++) {
			//	cout << distance[i] << " ";
			//}

			//cout << "\nPath :";
			 //for(int i = 0; i <= m_V; i++){
			 //  cout << path[i] << " ";
			 //}

			cout << "\nShortest Path :";
			vector<int> track;
			int i = end;

			//backtrack from the end to start of the path array
			while (path[i] != INF) {
				track.push_back(i);
				i = path[i];
			}
			track.push_back(i); //trailing node

			for (int i = track.size() - 1; i >= 0; i--) {
				cout << track[i] << " ";
			}
			cout << "\n";
		}
	}

	void Dijkstra(int start, int end) { //Weighted shortest path
		if (start <= 0 && start > m_V) {
			cout << "Please check the start vertex\n";
		}
		else if (end <= 0 && end > m_V) {
			cout << "Please check the end vertex\n";
		}
		else {
			cout << "Running Dijsktra Shortest Path Algorithm......\n";
			vector<int> distance(m_V + 1, INF); //starts with index 1
			vector<int> path(m_V + 1, INF); //starts with index 1

			distance[start] = 0;
			bool flag = false;

			//minimim priority queue associated with distance in the distance table
			priority_queue<pii, vector<pii>, sortDistance > pq;
			//Entry		[vertex, distance value]
			pq.push({ start, distance[start] });
			

			while (pq.empty() != true) {
				int node = pq.top().first; pq.pop();

				for (const auto& w : Adj[node]) { // get all the neighbour vertices
					int new_dist = distance[node] + w.second;

					if (distance[w.first] == INF) {
						distance[w.first] = new_dist;
						pq.push({ w.first, new_dist });
						path[w.first] = node;
					}

					if (distance[w.first] > new_dist) {
						distance[w.first] = new_dist;
						pq.push({ w.first, new_dist });
						path[w.first] = node;
					}
				}
			}

			//cout << "Distance :";
			//for (int i = 0; i <= m_V; i++) {
			//	cout << distance[i] << " ";
			//}

			cout << "\nMinimum Distance :" << distance[end];
			
			cout << "\nShortest Path :";
			vector<int> track;
			int i = end;

			//backtrack from the end to start of the path array
			while (path[i] != INF) {
				track.push_back(i);
				i = path[i];
			}
			track.push_back(i); //trailing node

			for (int i = track.size() - 1; i >= 0; i--) {
				cout << track[i] << " ";
			}
			cout << "\n";

		}//end else
	}

	void BellmanFordShortestPath(int start, int end) { //Weighted shortest path with negative edges //avg:On2 worst:On3
		if (start <= 0 && start > m_V) {
			cout << "Please check the start vertex\n";
		}
		else if (end <= 0 && end > m_V) {
			cout << "Please check the end vertex\n";
		}
		else { //INF 2147483647 INTMAX 
			cout << "Running Bellman Ford Shortest Path Algorithm......\n";
			vector<int> distance(m_V + 1, INF); //starts with index 1
			vector<int> path(m_V + 1, INF); //starts with index 1

			distance[start] = 0;

      //Relaxation code to get rid of negative edge values
      for (int cnt = 0; cnt < m_V - 1; cnt++) { // |V| - 1
				for (int node = 1; node <= m_V; node++) { // scan all the vertices 
          for (const auto& v : Adj[node]) {
						if (distance[node] + v.second < distance[v.first]) {
							distance[v.first] = distance[node] + v.second;
							path[v.first] = node;
						}
          }
        }
      }


			//Check for the negative weight cycle
			for (int node = 1; node <= m_V; node++) { // scan all the vertices 
				for (const auto& v : Adj[node]) {
					if (distance[node] + v.second < distance[v.first]) {
						cout << "Graph has negative cycle\n";
						return; // exit from here
					}
				}
			}
			
			cout << "\nMinimum Distance :" << distance[end];

			cout << "\nShortest Path :";
			vector<int> track;
			int i = end;

			//backtrack from the end to start of the path array
			while (path[i] != INF) {
				track.push_back(i);
				i = path[i];
			}
			track.push_back(i); //trailing node

			for (int i = track.size() - 1; i >= 0; i--) {
				cout << track[i] << " ";
			}
			cout << "\n";

		}

	}
	void GetStat() {
		cout << "Vertices:" << m_V << "\n";
		cout << "Edges:" << m_E << "\n";
	}

};

int main() {

	vector<pii> V0 = {
		{1, 2},
		{1, 4},
		{2, 4},
		{2, 5},
		{3, 1},
		{3, 6},
		{4, 3},
		{4, 5},
		{4, 6},
		{4, 7},
		{5, 7},
		{7, 6}
	};

	//Using directed acyclic graph DAG
	vector<pii> V1 = {
		{1, 2},
		{1, 4},
		{1, 3},
		{2, 5},
		{2, 4},
		{3, 6},
		{4, 3},
		{4, 6},
		{4, 7},
		{5, 4},
		{5, 7},
		{7, 6}
	};

	//GraphList connComponent(vertices);
	vector<pii> V2 = {
		{1, 2},
		{1, 3},
		{2, 3},
		{4, 0},
		{5, 6},
		{5, 9},
		{6, 7},
		{6, 8},
		{6, 9},
		{7, 8},
		{9, 8},
		{10, 11},
		{11, 12},
		{11, 13},
		{14, 15},
		{15, 16},
		{15, 17},
		{16, 17},
		{17, 18}
	};

	//directed weightd graph
	vector<tiii> V3 = {
		{1, 2, 4},
		{1, 3, 1},
		{2, 5, 4},
		{3, 2, 2},
		{3, 4, 4}
	};

	//directed weightd graph
	vector<tiii> V4 = {
		{1, 2, 10},
		{1, 3, 15},
		{2, 4, 12},
		{2, 6, 15},
		{3, 5, 10},
		{4, 5, 2},
		{4, 6, 1}
	};

	//directed weighted graph with negative edges
	vector<tiii> V5 = {
		{1, 2, 6},
		{1, 3, 5},
		{1, 4, 5},
		{2, 5, -1},
		{3, 2, -2},
		{3, 5, 1},
		{4, 3, -2},
		{4, 6, -1},
		{5, 7, 3},
		{6, 7, 3},
		{7, 0, 0}
	};

	GraphList gl(7);

	for (const auto& i : V5) {
		auto &[from, to, weight] = i;
		gl.AddDiEdge(from, to, weight);
	}

	gl.PrintDiGraph();
	gl.BellmanFordShortestPath(1, 7);


	

	return 0;
}