# Demo Video


https://github.com/user-attachments/assets/09d8967d-5082-4577-a9d1-edb6ac2b37b4


# How to run
To run it simply open main.exe

To compile it again on Windows, assuming you have MSVC installed you can compile it in Powershell with: 
cl /EHsc /std:c++17 /I external/SFML-3.0.0/include main.cpp /link /LIBPATH:external/SFML-3.0.0/lib sfml-graphics.lib sfml-window.lib sfml-system.lib 

# About the code
A pathfinding algorithm using C++ and SFML.
Included SFML files in the project.

Used A Star Pathfinding, which attempts to find the quickest route from a start node to an end node based on G Cost and H Cost.
It can be used to assist enemy AI in avoiding obstacles in video games but as it is general purpose there are other uses for it too, such as visualising the quickest path out of an area or route planning.

## Things to add
If it is not possible to get to the exact end node the pathfinding could still be made to still attempt to get as close as possible.
Could add a detection radius, so that it only chases when when the start node is within a certain distance.
Add other options for the pathfinder? Such as patrol or wander randomly.

## Issues
If traveller is near edges of bounds and too big too fit inside bounds there will be a 0x0000000000000000 error.

# Development process
Set up C++ and SFML project
Learn Pathfinding basics, look at other implementations of the algorithm
Create nodes and a grid of nodes
Objects/nodes will include Empty, Obstacles (walls), Path, Start and End nodes
Implement pathfinding from the start to end nodes
Visualise the algorithm working, move an object from start to end
Bonus features: Add custom grid building so the user can place their own obstacles or make a specific map
