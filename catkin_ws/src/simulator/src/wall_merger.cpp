
#include "simulator/map_handle.h"
#include <iostream>



int expandX(const std::vector<std::vector<MapCell>>& grid, const int x, const int  y) {
    int bestX = x;
    if (y < grid.size()) {
        const auto& row = grid[y];
        while (bestX+1 < row.size() && row[bestX+1].isWall()){ // check next cell
            bestX++;
        }
    }
    
    return bestX - x;
}

int expandY(const std::vector<std::vector<MapCell>>& grid, const int x, const int  y) {
    int bestY = y;
    if (y < grid.size()) {
        if (x < grid.at(y).size()) {
            while (bestY+1 < grid.size() && grid[bestY+1][x].isWall()){ // check next cell
                bestY++;
            }
        }
    }
    
    return bestY - y;
}


int expandDiag(const std::vector<std::vector<MapCell>>& grid, const int x, const int  y) {
    int bestX = x;
    int bestY = y;
    
    bool keep_growing = true;
    while (keep_growing){
        // Try to grow
        if (bestY + 1 < grid.size() && bestX + 1 < grid[bestY+1].size()) {
            bestX++;
            bestY++;
        }
        else {
            break;
        }
        
        // Check if growing caused expansion into non-wall cells
        for (int y_ = y; y_ <= bestY; y_++) {
            for (int x_ = x; x_ <= bestX; x_++) {
                if (!(grid[y_][x_].isWall())) {
                    bestX--;
                    bestY--;
                    keep_growing = false;
                }
            }
        }
    }
    
    return bestX - x; // same as bestY - y
}

