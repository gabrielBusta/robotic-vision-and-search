#ifndef __VIZ_HPP__
#define __VIZ_HPP__

#include <iostream>
#include <string>
#include "../AStar/AStar.hpp"

using AStar::uint;
using AStar::Vec2i;

class Viz
{
    public:
        Viz(Vec2i worldSize_);
        ~Viz();
        void displayWorld();
        void setCell(const Vec2i&, char token);
    private:
        Vec2i worldSize;
        std::string top;
        std::string bottom;
        std::string * rows;
};

#endif // __VIZ_HPP__
