#ifndef __VIZ_HPP__
#define __VIZ_HPP__

#include <iostream>
#include <string>
#include "../AStar/AStar.hpp"

namespace AStar
{   
    class Viz
    {
        public:
            Viz(Vec2i worldSize_);
            ~Viz();
            const char robot = 'R', collision = 'C';
            void displayWorld();
            void setCell(Vec2i coordinate, char token);
        private:
            Vec2i worldSize;
            std::string line;
            std::string * rows;
    };
}

#endif // __VIZ_HPP__
