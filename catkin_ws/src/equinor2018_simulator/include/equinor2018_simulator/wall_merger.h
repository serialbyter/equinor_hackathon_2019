#ifndef WALL_MERGER_H
#define WALL_MERGER_H

#include "equinor2018_simulator/map_handle.h"

int expandX(const std::vector<std::vector<MapCell>>& grid, const int x, const int  y);

int expandY(const std::vector<std::vector<MapCell>>& grid, const int x, const int  y);

int expandDiag(const std::vector<std::vector<MapCell>>& grid, const int x, const int  y);


#endif // WALL_MERGER_H
