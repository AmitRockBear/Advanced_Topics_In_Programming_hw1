#include <tuple>
#include "General.h"

std::tuple<int, int> getCoordinate(int x, int y, char direction) {
    switch (direction) {
        case 'N': return std::make_tuple(x-1,y);
        case 'E': return std::make_tuple(x,y+1);
        case 'S': return std::make_tuple(x+1,y);
        case 'W': return std::make_tuple(x,y-1);
        default: return std::make_tuple(x,y);
    }
}