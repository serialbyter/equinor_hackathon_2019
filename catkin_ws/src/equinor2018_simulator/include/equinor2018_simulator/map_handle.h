#ifndef MAP_HANDLE_H
#define MAP_HANDLE_H

#include <ros/ros.h>
#include <ascend_msgs/Map.h>

#include "equinor2018_simulator/zone.h"

#include <string>
#include <vector>

class MapCell {
    public:
        enum class Type { EMPTY, START, GOAL, WALL, OBJECT };

        MapCell() = delete;
        MapCell(Type type, int num = 0);
        MapCell(const std::string& desc);
        Type getType() const;
        bool isObstruction() const;
        bool isWall() const;
        int getNum() const;
        std::vector<int> getMetadata() const;
    private:
        Type type_;
        int num_ = 0;  // used for distinguishing between similar objects
        std::vector<int> metadata_;
};

class MapHandle {
    private:
        std::vector<std::vector<MapCell>> map_vector_;
        
    public:
        MapHandle() = delete;
        MapHandle(const std::string& csv_file);
        
        std::vector<Zone> getGoalzones() const;
        std::pair<float,float> getStartPosition() const;

        ascend_msgs::Map createMapMessage() const;
};


#endif // MAP_HANDLE_H
