#include <SFML/Graphics.hpp>
#include <iostream>
#include <limits>

// A Star Pathfinding basics
// G Cost - Travel distance from a node to the start node
// H Cost - Travel distance from a node to the target node
// F Cost - G Cost + H Cost
// The lower the F Cost the better/quicker the path is

// Chose C++ and SFML as I am familiar with the basics of C++ and SFML and displaying objects
// Chose A Star as it is general purpose and good at finding the quickest path.

// Plan
// Set up C++ and SFML project
// Learn Pathfinding
// Create nodes and a grid of nodes
// Objects/nodes will include Empty, Obstacles, Path, Start and End points
// Implement pathfinding from the start to end points
// Visualise the algorithm working, move an object from start to end
// Bonus features: Add custom grid building so the user can place their own obstacles or make a specific map

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

// The A Star pathfinding algorithm
std::vector<Node*> aStar(Node* start, Node* goal, Grid& grid) {
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
            if (neighbour->wall || neighbour->visited) continue;
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

int main() {
    // Create grid
    const int TILE = 32; // Tile Size
    const int W = 20;  // Columns
    const int H = 20;  // Rows
    Grid grid(W, H);

    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Add start and end nodes
    Node* startNode = &grid.nodes[0][0];
    Node* endNode   = &grid.nodes[15][15];

    // Create wall
    grid.nodes[1][2].wall = !grid.nodes[1][2].wall;
    grid.nodes[2][1].wall = !grid.nodes[2][1].wall;
    grid.nodes[2][2].wall = !grid.nodes[2][2].wall;
    grid.nodes[2][3].wall = !grid.nodes[2][3].wall;
    grid.nodes[2][4].wall = !grid.nodes[2][4].wall;
    grid.nodes[2][5].wall = !grid.nodes[2][5].wall;
    grid.nodes[2][6].wall = !grid.nodes[2][6].wall;
    grid.nodes[3][7].wall = !grid.nodes[3][7].wall;
    grid.nodes[3][8].wall = !grid.nodes[3][8].wall;
    grid.nodes[4][9].wall = !grid.nodes[4][9].wall;

    // Call A Star pathfinding
    std::vector<Node*> path = aStar(startNode, endNode, grid);

    while (window.isOpen()) {
        
        sf::Event event;
        // Ensure window is closed properly
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear(sf::Color::Black);

        // Fill in grid
        sf::RectangleShape tileShape(sf::Vector2f(TILE - 1, TILE - 1));
        for (float y = 0; y < H; ++y) {
            for (float x = 0; x < W; ++x) {
                bool isPathNode = std::find(path.begin(), path.end(), &grid.nodes[y][x]) != path.end(); // Check if is a path node
                tileShape.setPosition({x * TILE, y * TILE});
                // Start tile is green, end tile is blue, wall tiles are red
                // Path tiles are yellow, empty tiles are white
                if (&grid.nodes[y][x] == startNode)
                    tileShape.setFillColor(sf::Color::Green);
                else if (&grid.nodes[y][x] == endNode)
                    tileShape.setFillColor(sf::Color::Blue);
                else if (grid.nodes[y][x].wall)
                    tileShape.setFillColor(sf::Color::Red);
                else if (isPathNode)
                    tileShape.setFillColor(sf::Color::Yellow);
                else
                    tileShape.setFillColor(sf::Color::White);
                window.draw(tileShape);
            }
        }

        // Display screen
        window.display();
    }
    return 0;
}
