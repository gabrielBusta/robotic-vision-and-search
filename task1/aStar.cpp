#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>

using namespace std;

const int n = 10; // horizontal size of the map
const int m = 10; // vertical size size of the map
int map[n][m];
int closed_nodes_map[n][m]; // map of closed (tried-out) nodes
int open_nodes_map[n][m]; // map of open (not-yet-tried) nodes
int dir_map[n][m]; // map of directions
const int dir = 8; // number of possible directions to go at any position
int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

class node
{
private:
	// current position
	int xPos;
	int yPos;
	// total distance already travelled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority
public:
	node(int, int, int, int);
	int getxPos() const
	 	{ return xPos; }
	int getyPos() const
    	{ return yPos; }
	int getLevel() const
		{ return level; }
	int getPriority() const
		{ return priority; }
	void updatePriority(const int &, const int &);
	// give better priority to going strait instead of diagonally
	void nextLevel(const int &); // i: direction
	// Estimation function for the remaining distance to the goal.
	const int & estimate(const int &, const int &) const;
};

// Determine priority (in the priority queue)
bool operator<(const node &, const node &);

// A-star algorithm.
// The route returned is a string of direction digits.
string pathFind(const int &, const int &, const int &, const int &);

//--------------------------------------------
int main()
{
	// create empty map
	for (auto y = 0; y < m; y++)
	{
		for (auto x = 0; x < n; x++)
			map[x][y] = 0;
	}

	// Obstacle generators  3,5  4,6   5,5 6,5 7,4  8,3
	map[2][4] = 1;
	map[3][5] = 1;
	map[4][4] = 1;
	map[5][4] = 1;
	map[6][3] = 1;
	map[7][2] = 1;

	// randomly select start and finish locations
	auto xA = 0,
		 yA = 0,
		 xB = 9,
		 yB = 9;

	cout << "Map Size (X,Y): " << n << "," << m << endl;
	cout << "Start: " << xA << "," << yA << endl;
	cout << "Finish: " << xB << "," << yB << endl;
	// get the route
	string route = pathFind(xA, yA, xB, yB);
	if (route == "") cout << "An empty route generated!" << endl;
	cout << "Route:" << endl;
	cout << route << endl << endl;

	// follow the route on the map and display it
	if (route.length() > 0)
	{
		int j; char c;
		auto x = xA;
		auto y = yA;
		map[x][y] = 2;
		for (auto i = 0; i < route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x][y] = 3;
		}
		map[x][y] = 4;

		// display the map with the route
		for (auto y = 0; y < m; y++)
		{
			for (auto x = 0; x < n; x++)
				if (map[x][y] == 0)
					cout << ".";
				else if (map[x][y] == 1)
					cout << "O"; //obstacle
				else if (map[x][y] == 2)
					cout << "S"; //start
				else if (map[x][y] == 3)
					cout << "R"; //route
				else if (map[x][y] == 4)
					cout << "F"; //finish
			cout << endl;
		}
	}
	getchar(); // wait for a (Enter) keypress
	return(0);
}
//-------------------------------

// Node Class Members
node::node(int xp, int yp, int d, int p){
	xPos = xp; yPos = yp; level = d; priority = p;
}
void node::updatePriority(const int & xDest, const int & yDest){
		priority = level + estimate(xDest, yDest) * 10; //A*
}
void node::nextLevel(const int & i){
		level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
}
const int & node::estimate(const int & xDest, const int & yDest) const
{
	static int xd, yd, d;
	xd = xDest - xPos;
	yd = yDest - yPos;
	// Euclidian Distance
	d = static_cast<int>(sqrt(xd*xd + yd*yd));
	return(d);
}

bool operator<(const node & a, const node & b)
{
	return a.getPriority() > b.getPriority();
}

string pathFind(const int & xStart, const int & yStart,
	const int & xFinish, const int & yFinish)
{
	static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static node* n0;
	static node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;


	// reset the node maps
	for (y = 0; y < m; y++)
	{
		for (x = 0; x < n; x++)
		{
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}
	}

	// create the start node and push into list of open nodes
	n0 = new node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map

											  // A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		if (x == xFinish && y == yFinish)
		{
			// generate the path from finish to start
			// by following the directions
			string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i < dir; i++)
		{
			xdx = x + dx[i]; ydy = y + dy[i];

			if (!(xdx < 0 || xdx > n - 1 || ydy < 0 || ydy > m - 1 || map[xdx][ydy] == 1
				|| closed_nodes_map[xdx][ydy] == 1))
			{
				// generate a child node
				m0 = new node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					// mark its parent node direction
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy] > m0->getPriority())
				{
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

								   // empty the larger size pq to the smaller one
					if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	return ""; // no route found
}
