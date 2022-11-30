// Final Project: 8 Puzzle
// Team: Aseem Mishra, Mayank Pandey, Gopal Krishna Shukla, Soumil Saxena
// This project finds the cheapest solultion for 8 puzzle using Djikstra's Algorithm, DFS and BFS.
// Assuming the input is a 3 newRow 3 matrix as per the samples provided.

#include <algorithm>
#include <list>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

// global variables used for the directions of neighbours
// bottom, left, top, right
int rowDir[] = { 1, 0, -1, 0 };
int colDir[] = { 0, -1, 0, 1 };

struct Node
{
	int matrix[3][3]; 			// Initializing the matrix of 3 rows and 3 columns
	Node* parent; 				// Used to store the parent
	int rowCoordinates;			// Used to store the row coordinates
	int columnCoordinates; 		// Used to store the column coordinates
	int cost; 					// Used to store the cost in terms of misplaced titles
	int rank; 				    // Used to store the number of tiles that are moved
	int distance; 				// distance property used in all BFS, DFS, and Djikstra's algorithm to get the shortest path cost/
	bool seen = false; 		    // Marks the explored nodes when using Djisktra's Algorithm 	
	bool visited = false; 		// Marks the visited nodes when using BFS
};

// void print(): Prints the matrix
void print(int matrix[3][3])
{
	// loops through the matrix to print the elements
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			cout << matrix[i][j];
		cout << endl;
	}
}

// printP(): This function prints the root node to the destination node
void printP(Node* root)
{
	if (root == NULL)
		return;
	printP(root->parent);
	print(root->matrix);

	cout << endl;
}

// newNode(): This function assigns new nodes and helps make those nodes with certain properties.
Node* newNode(int matrix[3][3], int initialRow, int initialCol, int newRow,
	int newCol, int rank, int distance, Node* parent)
{
	Node* node = new Node; 											// creating a new node called 'node'
	node->parent = parent; 											// setting the parent property of 'node' to the parent received as an argument to trace back to root
	memcpy(node->matrix, matrix, sizeof node->matrix); 				// copying the data from parent to the 'node'
	swap(node->matrix[initialRow][initialCol], node->matrix[newRow][newCol]); 	// swapping the fields to make a change for 1 tile
	node->cost = INT_MAX; 											// set initial cost
	node->rank = rank; 												// set the rank received for the tile to be moved
	node->distance = distance; 										// set the distance property to be the distance of the current node from the starting node
	node->rowCoordinates = newRow;									// set the new coordinates for row
	node->columnCoordinates = newCol;								// set the new coordinates for column

	return node;
}

// int valid():  checks if the given coordinates are valid (in range to be processed as a valid neighbour)
int valid(int newRow, int newCol)
{
	return (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3);
}

// bool compare(): Function compares two given node pointers and checks if their matrices are same or not
bool compare( Node* left,  Node* right) 
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			if (left->matrix[i][j] != right->matrix[i][j])
				return false;
	}
	return true;
}

// calCost(): Function calculates the number of non-blank tiles not in their desired positions
int calCost(int inpMatrix[3][3], int outMatrix[3][3])
{
	int counter = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (inpMatrix[i][j] && inpMatrix[i][j] != outMatrix[i][j])
			{
				counter++;
			}
		}
	}
	return counter;
}

// nodeCompare(): Compares the distance property of the two nodes and the returns a boolean value
bool nodeCompare(const Node* left, const Node* right)
{
	return (left->distance) < (right->distance);
}

// BFS_Solution(): This function takes the input matrix and desired matrix as inputs and gives cheapest solution using BFS
// It also inputs the coordinates of blank tiles.
void BFS_Solution(int inpMatrix[3][3], int outMatrix[3][3], int newRow, int newCol)
{
	vector<Node*> data; // used to store the vertices
	Node* root = newNode(inpMatrix, newRow, newCol, newRow, newCol, 0, 0, NULL); // Root node intialization
	root->visited = true;
	data.push_back(root);

	while (!data.empty()) 				// while loop to visit each node in the queue
	{
		Node* min = data.front();
		for (auto node : data) 			// Take out unexplored vertex first
		{ 
			if (node->seen == false)
			{
				min = node;
				break;
			}
				
		}
		
		min->seen = true; // Helps in identifying if the vertex is already explored

		if (min->cost == 0)
		{
			printP(min);  // helper function to print the path from root to this node
			cout << "Cost for BFS: " << min->distance << endl;
			return;
		}

		for (int i = 0; i < 4; i++) // Visit all possible paths
		{
			if (valid(min->rowCoordinates + rowDir[i], min->columnCoordinates + colDir[i]))
			{				
				Node* nChild = newNode(min->matrix, min->rowCoordinates, min->columnCoordinates, min->rowCoordinates + rowDir[i],
				min->columnCoordinates + colDir[i], 0, 0, min);
				nChild->cost = calCost(nChild->matrix, outMatrix);
				nChild->distance = min->distance + 1;  // updating the cost
				int dist = 0;

				while (dist < data.size())
				{
					if (compare(nChild, data[dist]) && data[dist]->seen != true)
						break;
					dist++;
				}

				if (dist != data.size()) // Update distance if nChild in queue or else add it to queue
				{
					data[dist]->distance = nChild->distance;		
				}
				else // adding the child to the queue otherwise
				{
					data.push_back(nChild);
					nChild->visited = true;
				}

			}
		}
		
	}

}

// DFS_Solver(): Utilitiy function to help with the DFS solution
void DFS_Solver(Node* min, Node* Solution, int outMatrix[3][3])
{
	if (min->cost == 0)
	{ 
		if (Solution->distance > min->distance) // checking for minimum cost
		{
			Solution->parent = min->parent;
			Solution->distance = min->distance;
      		memcpy(Solution->matrix,min->matrix , sizeof Solution->matrix); // copying the matrix
		}

	}
	else
	{
		for (int i = 0; i < 4; i++) // go through each vertex
		{
			if (valid(min->rowCoordinates + rowDir[i], min->columnCoordinates + colDir[i]))
			{
				Node* nChild = newNode(min->matrix, min->rowCoordinates, min->columnCoordinates, min->rowCoordinates + rowDir[i],
				min->columnCoordinates + colDir[i], 0, 0, min); // creating new child node
				nChild->cost = calCost(nChild->matrix, outMatrix); // calculating the cost
				nChild->distance = min->distance + 1;

				if ( nChild->distance < 15)
				{
					DFS_Solver(nChild, Solution, outMatrix); // recursively calling it again and again
				}
			}
		}
	}
}

// DFS_Solution(): This function takes the input matrix and desired matrix as input and gives cheapest solution using DFS
// It also inputs the coordinates of blank tiles.
void DFS_Solution(int inpMatrix[3][3], int outMatrix[3][3], int newRow, int newCol)
{
	Node* currNode = newNode(inpMatrix, newRow, newCol, newRow, newCol, 0, 0, NULL);
	Node* temporary = newNode(outMatrix, newRow, newCol, newRow, newCol, 0, INT_MAX, NULL);
	currNode->cost = calCost(inpMatrix, outMatrix);
	DFS_Solver(currNode,temporary,outMatrix);
	printP(temporary);
	cout << "Cost for DFS: " << temporary->distance << endl;
}

// Dijkstra_Solution(): This function takes the input matrix and desired matrix as input and gives cheapest solution using Dijkstra's Algorithm
// It also inputs the coordinates of blank tiles.
void Dijkstra_Solution(int inpMatrix[3][3], int outMatrix[3][3],int newRow, int newCol)
{
	vector<Node*> solDijk;
	Node* node1 = newNode(inpMatrix, newRow, newCol, newRow, newCol, 0,0, NULL); // Root node intialization
	node1->cost = calCost(inpMatrix, outMatrix);
	solDijk.push_back(node1);

	while (!solDijk.empty()) // while there are still nodes in the queue
	{
		Node* min = *min_element(solDijk.begin(), solDijk.end(), nodeCompare);
		min->seen = true; 
		 
		if (min->cost == 0) //Prints path
		{
			printP(min);
			cout << "Cost for Dijkstra's Algorithm: " << min->distance << endl;
			return;
		}
		
		// Go through all vertices and their immediate neighbours if valid
		for (int i = 0; i < 4; i++)
		{
			if (valid(min->rowCoordinates + rowDir[i], min->columnCoordinates + colDir[i])) // checks whether that is a valid move or not
			{
				Node* nChild = newNode(min->matrix, min->rowCoordinates, min->columnCoordinates, min->rowCoordinates + rowDir[i], min->columnCoordinates + colDir[i],
				min->matrix[min->rowCoordinates + rowDir[i]][min->columnCoordinates + colDir[i]], 0, min);
				nChild->cost = calCost(nChild->matrix, outMatrix);
				nChild->distance = min->distance + nChild->rank;

				// Comparing and updating the vertex
				int dist = 0;
				while (dist < solDijk.size())
				{
					if (compare(nChild, solDijk[dist]) && solDijk[dist]->seen != true)
						break;
					dist++;
				}
			
				if (dist != solDijk.size()) // when the vertex is there in the queue
				{
					if (nChild->distance < solDijk[dist]->distance) // updating the distance if the new distance is smaller than the previous one
					{
						solDijk[dist]->distance = nChild->distance;
					}
				}
				else // adding child to the queue
				{
					solDijk.push_back(nChild);
				}

			}
		}
		min->distance = INT_MAX; // a security check to keep the min_element function from exploring already visited nodes.
	}
}

// Main for the program
int main()
{
    // Declaration and Initialization of 3X3 Matrix with 0's.
	int inpMatrix[3][3] =
	{
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

    // Reading Input from the file
	ifstream input;
	input.open("Second.txt");
	if (input) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				input >> inpMatrix[i][j];
			}
		}
	}
	input.close();

	int newRow = 1, newCol = 1;
	
	// Desired Output Matrix given to the functions
	int outMatrix[3][3] =
	{
		{1, 2, 3},
		{8, 0, 4},
		{7, 6, 5}
	};

	cout << "      8 Puzzle Solution using BFS, DFS, and Dijkstra algorithms    " << endl << endl;

	cout << endl << "Solution using BFS" << endl << endl;
	BFS_Solution(inpMatrix, outMatrix, newRow, newCol);

	cout << endl << "Solution using DFS" << endl << endl;
	DFS_Solution(inpMatrix, outMatrix, newRow, newCol);

	cout << "Solution using Dijkstra's Algortihm" << endl << endl;
	Dijkstra_Solution(inpMatrix, outMatrix, newRow, newCol);

	return 0;
}
