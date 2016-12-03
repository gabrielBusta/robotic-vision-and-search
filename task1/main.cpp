#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
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
    std::reverse(begin(path), end(path));
    
    AStar::Viz viz(worldSize);
    
    for (auto coordinate : collisions) {
        viz.setCell(coordinate, viz.collision);
    }
    
    for (auto coordinate : path) {
        viz.setCell(coordinate, viz.robot);
        std::this_thread::sleep_for(std::chrono::nanoseconds(600000000));
        std::system("clear");
        viz.displayWorld();
    }
}
