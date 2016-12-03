#include "Viz.hpp"
#include <math.h>

AStar::Viz::Viz(Vec2i worldSize_)
{
    worldSize = worldSize_;
    
    line = " +";
    int numberOfDashes = worldSize.y * 2 - 1;
    for (int i = 0; i < numberOfDashes; i++) {
        line += "-";
    }
    line += "+\n";
    
    std::string row = "";
    for (int i = 0; i < worldSize.x; i++) {
        row += " |";
    }
    row += "\n";
    
    rows = new std::string[worldSize.x];
    for (int i = 0; i < worldSize.x; i++) {
        rows[i] = row;
    }
}

AStar::Viz::~Viz()
{
    delete[] rows;
}

void AStar::Viz::displayWorld()
{
    std::cout << std::endl;
    std::cout << " Robot = " << robot << std::endl;
    std::cout << " Collision = " << collision << std::endl;
    std::cout << line;
    for (int i = 0; i < worldSize.x; i++) {
        std::cout << " |" << rows[i];
        std::cout << line;
    }
    std::cout << std::endl;
}

void AStar::Viz::setCell(Vec2i coordinate, char token)
{
    rows[coordinate.x][coordinate.y * 2] = token;
}
