# How to run
To run it simply open main.exe

To compile it again on Windows, assuming you have mingw64 installed you can compile it in Powershell with: 
g++ -Wall -Wextra -g3 main.cpp -o main.exe -I"C:/SFML/include" -L"C:/SFML/lib" -lsfml-graphics -lsfml-window -lsfml-system

# About the code
A pathfinding algorithm using C++ and SFML.
Included SFML files in the project.

Used A Star Pathfinding, which attempts to find the quickest route from a start node to an end node based on G Cost and H Cost.

It can be used to assist enemy AI in avoiding obstacles in video games but as it is general purpose there are other uses for it too, such as visualising the quickest path out of an area or route planning.


# Development process
Set up C++ and SFML project
Learn Pathfinding basics, look at other implementations of the algorithm
Create nodes and a grid of nodes
Objects/nodes will include Empty, Obstacles (walls), Path, Start and End nodes
Implement pathfinding from the start to end nodes
Visualise the algorithm working, move an object from start to end
Bonus features: Add custom grid building so the user can place their own obstacles or make a specific map