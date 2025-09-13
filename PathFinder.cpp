#include <SFML/Graphics.hpp>
#include <iostream>
#include <limits>

// A Star Pathfinding basics
// G Cost - Travel distance from a node to the start node
// H Cost - Travel distance from a node to the target node
// F Cost - G Cost + H Cost
// The lower the F Cost the better/quicker the path is

// A Star is general purpose and good at finding the quickest path.

// Things to consider
// Size of object, will it fit through a one node wide path or will it need a bigger path?

// Node structure, can be wall or empty
struct Node {
    // Node types
    bool wall = false;
    bool visited = false;
    Node* previousNode = nullptr;
    // G, H and F cost
    float g = std::numeric_limits<float>::infinity();
    float h = 0;
    float f() const { return g + h; }
    // Position of each node
    int x = 0;
    int y = 0;
};

// Grid structure, made up of nodes
struct Grid {
    int width, height;
    std::vector<std::vector<Node>> nodes; // nodes[y][x]

    explicit Grid(int w, int h) : width(w), height(h), nodes(h, std::vector<Node>(w)) {
        // Initialize each node with its position
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                nodes[y][x].x = x;
                nodes[y][x].y = y;
            }
        }
    }

    // Check if a position is inside the grid bounds
    bool inBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};

// Function to estimate the h cost from node a to node b
float estimatedCost(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

// Function to get the neighbours of a node (does not include diagonal neighbours)
std::vector<Node*> getNeighbours(Node* node, Grid& grid) {
    std::vector<Node*> neighbours;
    const int dx[] = { -1, 1, 0, 0 };
    const int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        int nx = node->x + dx[i];
        int ny = node->y + dy[i];
        if (grid.inBounds(nx, ny)) {
            neighbours.push_back(&grid.nodes[ny][nx]);
        }
    }
    return neighbours;
}

// Function to see if there is enough space for the object to fit through the path
bool isSpaceFree(int x, int y, Grid& grid, int objW, int objH) {
    for (int dy = 0; dy < objH; ++dy) {
        for (int dx = 0; dx < objW; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if (!grid.inBounds(nx, ny) || grid.nodes[ny][nx].wall) {
                return false;
            }
        }
    }
    return true;
}

// The A Star pathfinding algorithm
std::vector<Node*> aStar(Node* start, Node* goal, Grid& grid, int objectWidth, int objectHeight) {
    // List of nodes to be checked
    std::vector<Node*> openSet;
    start->g = 0;
    start->h = estimatedCost(start, goal);
    openSet.push_back(start);

    while (!openSet.empty()) {
        // Sort list of nodes by lowest F cost
        std::sort(openSet.begin(), openSet.end(), [](Node* a, Node* b) {
            return a->f() < b->f();
        });

        Node* current = openSet.front();
        openSet.erase(openSet.begin());

        // If the goal has been reached, reconstruct the path
        if (current == goal) {
            std::vector<Node*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->previousNode;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        current->visited = true;

        // Check each neighbour
        for (Node* neighbour : getNeighbours(current, grid)) {
            // Complete checks
            if (neighbour->x + objectWidth > grid.width || neighbour->y + objectHeight > grid.height) continue;
            if (!isSpaceFree(neighbour->x, neighbour->y, grid, objectWidth, objectHeight)) continue;
            float tentativeG = current->g + 1; // Distance/cost between neighbouring nodes
            // If the cost is less (if this path to neighbor is better), then record it
            if (tentativeG < neighbour->g) {
                neighbour->previousNode = current;
                neighbour->g = tentativeG;
                neighbour->h =  estimatedCost(neighbour, goal);
                // Add neighbour to list of nodes if not already on it
                if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end()) {
                    openSet.push_back(neighbour);
                }
            }
        }
    }
    return {}; // No path found
}
