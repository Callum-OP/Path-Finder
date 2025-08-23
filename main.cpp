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

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(375, 275);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
