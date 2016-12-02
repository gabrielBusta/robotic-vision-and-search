#include "Viz.hpp"
#include <math.h>

Viz::Viz(Vec2i worldSize_)
{
    worldSize = worldSize_;
    
    top = "  ";
    int numberOfUnderscores = worldSize.y * 2 - 1;
    for (int i = 0; i < numberOfUnderscores; i++) {
        top += "_";
    }
    top += "\n";
    
    std::string row = " |";
    for (int i = 0; i < worldSize.x; i++) {
        row += "_|";
    }
    row += "\n";
    
    rows = new std::string[worldSize.x];
    for (int i = 0; i < worldSize.x; i++) {
        rows[i] = row;
    }
}

Viz::~Viz()
{
    delete[] rows;
}

void Viz::displayWorld()
{
    std::cout << top;
    for (int i = 0; i < worldSize.x; i++) {
        std::cout << rows[i];
    }
    std::cout << std::endl;
}
