#include <SFML/Graphics.hpp>
#include <iostream>
#include <limits>

#include "PathFinder.cpp"

int main() {
    // Create grid
    const int TILE = 32; // Tile Size
    const int W = 20;  // Columns
    const int H = 20;  // Rows
    Grid grid(W, H);

    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Window");
    window.setFramerateLimit(60);

    // Add start and end nodes
    Node* startNode = &grid.nodes[0][0];
    //Node* endNode = &grid.nodes[15][15];
    sf::CircleShape player(TILE / 2 - 4);
    player.setFillColor(sf::Color::Green);
    player.setPosition({500, 500});
    int playerX = 15;
    int playerY = 15;
    Node* endNode = &grid.nodes[playerY][playerX];
    float speed = 5;

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
    grid.nodes[12][13].wall = !grid.nodes[12][13].wall;
    grid.nodes[12][14].wall = !grid.nodes[12][14].wall;
    grid.nodes[12][15].wall = !grid.nodes[12][15].wall;

    // Call A Star pathfinding
    std::vector<Node*> path = aStar(startNode, endNode, grid);

    // Create object to move
    sf::CircleShape traveller(TILE / 2 - 4);
    traveller.setFillColor(sf::Color::Green);
    traveller.setOrigin({traveller.getRadius(), traveller.getRadius()});

    // Movement control
    size_t pathIndex = 0;
    sf::Vector2f targetPos(path[pathIndex]->x * TILE + TILE / 2, path[pathIndex]->y * TILE + TILE / 2);
    traveller.setPosition(targetPos);
    float moveSpeed = 100.0f;

    while (window.isOpen()) {
        
        // Process events
        std::vector<sf::Event> events;
        while (const std::optional event = window.pollEvent())
        {
            sf::Event ev = *event;
            events.push_back(ev);
            // Ensure window is closed properly
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Place or remove walls when mouse is left clicked
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if ( mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    int mouseX = sf::Mouse::getPosition(window).x;
                    int mouseY =  sf::Mouse::getPosition(window).y;
                    int gridX = mouseX / TILE;
                    int gridY = mouseY / TILE;
                    // If within the grid
                    if (grid.inBounds(gridX, gridY)) {
                        Node& clickedNode = grid.nodes[gridY][gridX];
                        // Can not change the start and end node
                        if (&clickedNode != startNode && &clickedNode != endNode) {
                            // Reverse the role of the tile (if a wall then become empty or if empty then become a wall)
                            clickedNode.wall = !clickedNode.wall;
                        }
                    }
                }
            }
        }

        // Move player with arrow keys
        // Diagonal
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
            player.move({-speed / 1.5f, -speed / 1.5f});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
            player.move({-speed / 1.5f, speed / 1.5f});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
            player.move({speed / 1.5f, -speed / 1.5f});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
            player.move({speed / 1.5f, speed / 1.5f});
        }
        // Non diagonal
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
            player.move({-speed, 0.0f});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
            player.move({speed, 0.0f});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
            player.move({0.0f, -speed});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
            player.move({0.0f, speed});
        }

        // Move the end node with the player
        // Get player current grid position
        sf::Vector2f playerPos = player.getPosition();
        int playerGridX = static_cast<int>(playerPos.x) / TILE;
        int playerGridY = static_cast<int>(playerPos.y) / TILE;
        // Stick to grid bounds and avoid walls
        if (grid.inBounds(playerGridX, playerGridY) && !grid.nodes[playerGridY][playerGridX].wall) {
            endNode = &grid.nodes[playerGridY][playerGridX];
        }

        // Reset pathfinding
        // Reset costs
        for (auto& row : grid.nodes) {
            for (auto& node : row) {
                node.visited = false;
                node.previousNode = nullptr;
                node.g = std::numeric_limits<float>::infinity();
                node.h = 0;
            }
        }
        // Run pathfinding again
        path = aStar(startNode, endNode, grid);
        pathIndex = 0;

        // Move the start node with the traveller
        // Get traveller current grid position
        sf::Vector2f travellerPos = traveller.getPosition();
        int travellerGridX = static_cast<int>(travellerPos.x) / TILE;
        int travellerGridY = static_cast<int>(travellerPos.y) / TILE;
        // Stick to grid bounds and avoid walls
        if (grid.inBounds(travellerGridX, travellerGridY) && !grid.nodes[travellerGridY][travellerGridX].wall) {
            startNode = &grid.nodes[travellerGridY][travellerGridX];
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

        // Move the object along the path
        if (pathIndex + 1 < path.size()) {
            sf::Vector2f currentPos = traveller.getPosition();
            sf::Vector2f nextPos(path[pathIndex + 1]->x * TILE + TILE / 2, path[pathIndex + 1]->y * TILE + TILE / 2);
            sf::Vector2f direction = nextPos - currentPos;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (distance > 1.0f) {
                direction /= distance;
                traveller.move(direction * moveSpeed * (1.0f / 60.0f));
            } else {
                traveller.setPosition(nextPos);
                pathIndex++;
            }
        }

        // Draw objects
        window.draw(traveller);
        window.draw(player);

        // Display screen
        window.display();
    }
    return 0;
}
