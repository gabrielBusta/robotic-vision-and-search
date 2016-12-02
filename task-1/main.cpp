#include <iostream>
#include <string>
#include "AStar/AStar.hpp"
#include "Viz/Viz.hpp"

int main()
{
    AStar::Vec2i worldSize = {10, 10}, start = {0, 0}, goal = {9, 9};
    
    AStar::CoordinateList collisions = {{4, 2},
                                        {5, 2},
                                        {5, 3},
                                        {5, 4},
                                        {4, 4},
                                        {4, 5},
                                        {4, 6},
                                        {3, 6},
                                        {3, 7},
                                        {2, 7}};
    
    AStar::Generator generator;
    generator.setWorldSize(worldSize);
    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(true);
    
    for (auto coordinate : collisions) {
        generator.addCollision(coordinate);
    }

    auto path = generator.findPath(start, goal);
    
    for (auto& coordinate : path) {
        std::cout << coordinate.x << " " << coordinate.y << "\n";
    }
    
    Viz viz(worldSize);
    viz.displayWorld();
}
