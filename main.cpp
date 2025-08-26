#include <SFML/Graphics.hpp>
#include <iostream>

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
    bool wall = false;
};

// Grid structure, made up of nodes
struct Grid {
    int width, height;
    std::vector<std::vector<Node>> nodes; // nodes[y][x]

    explicit Grid(int w, int h) : width(w), height(h), nodes(h, std::vector<Node>(w)) {}

    bool inBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};

int main() {
    // Create grid
    const int TILE = 32;
    const int W = 20;  // columns
    const int H = 20;  // rows
    Grid grid(W, H);

    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Create wall
    grid.nodes[2][2].wall = !grid.nodes[2][2].wall;

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
                tileShape.setPosition({x * TILE, y * TILE});
                // Empty tiles are white, wall tiles are red
                if (grid.nodes[y][x].wall)
                    tileShape.setFillColor(sf::Color::Red);
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
