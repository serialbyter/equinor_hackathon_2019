#include <simulator/csv_parser.h>

#include <std_msgs/MultiArrayDimension.h>

#include "simulator/map_handle.h"
#include "simulator/zone.h"
#include <ascend_msgs/Map.h>

#include <iostream>
#include <algorithm>


MapHandle::MapHandle(const std::string& csv_file) 
{
	map_vector_ = ParseCSVMap(csv_file);
}

// TODO: std_msg dim
ascend_msgs::Map 
MapHandle::createMapMessage() const{
	ascend_msgs::Map map_msg;
	std_msgs::MultiArrayDimension dim0;
	std_msgs::MultiArrayDimension dim1;

	dim0.label = 'y';
	dim0.stride = 1;
	dim0.size = map_vector_.size();
	map_msg.dim.push_back(dim0);
	
	dim1.label = 'x';
	dim1.stride = 1;
	dim1.size = map_vector_.at(0).size();
	map_msg.dim.push_back(dim1);


    for (const auto row : map_vector_ ) {
        for (const auto cell : row) {
            map_msg.data.push_back(static_cast<int>(cell.isObstruction()));
        }
    }

    return map_msg;
}

std::vector<Zone> MapHandle::getGoalzones() const {
    struct ZoneOrder {
        Zone zone;
        int id;
    };

    std::vector<ZoneOrder> zonesWithOrder;
    for (int y = 0; y < map_vector_.size(); y++) {
        const auto& col = map_vector_.at(y);
        for (int x = 0; x < col.size(); x++) {
            if (col.at(x).getType() == MapCell::Type::GOAL) {
                Zone z;
                z.setZoneBox(
                        static_cast<float>(x) + 0.5 - 1.0, // x_min
                        static_cast<float>(y) + 0.5 - 1.0, // y_min
                        static_cast<float>(x) + 0.5 + 1.0, // x_max
                        static_cast<float>(y) + 0.5 + 1.0  // y_max
                );
                const auto metadata = col.at(x).getMetadata();
                if (metadata.size() == 2) {
                    z.setYaw((3.1415 / 180.0) * static_cast<float>(metadata.at(0)));
                    z.setCorrectObservation(metadata.at(1));
                }
                else if (metadata.size() != 0) {
                    throw std::invalid_argument("Invalid metadata, expected g<i>_<yaw>_<correctobject>");
                }
                
                const int num = col.at(x).getNum();
                zonesWithOrder.push_back({z, num});
            }
        }
    }
    
    std::sort(zonesWithOrder.begin(), zonesWithOrder.end(),
            [](const ZoneOrder& a, const ZoneOrder& b) { return a.id < b.id; });
    
    std::vector<Zone> zones;
    for (const auto& zone : zonesWithOrder) {
        zones.push_back(zone.zone);
    }
    
    return zones;
}

std::vector<Zone> MapHandle::getBoostZones() const{
    std::vector<Zone> boostZones;

    for (int y = 0; y < map_vector_.size(); y++){
        const auto &col = map_vector_.at(y);
        for (int x = 0; x < col.size(); x++) {
            if (col.at(x).getType() == MapCell::Type::BOOST)
            {
                Zone z;
                z.setZoneBox(
                    static_cast<float>(x) + 0.5 - 1.0, // x_min
                    static_cast<float>(y) + 0.5 - 1.0, // y_min
                    static_cast<float>(x) + 0.5 + 1.0, // x_max
                    static_cast<float>(y) + 0.5 + 1.0  // y_max
                );
                boostZones.push_back(z);
            }
        }
    }
    return boostZones;
}

std::pair<float,float> MapHandle::getStartPosition() const {
    for (int y = 0; y < map_vector_.size(); y++) {
        const auto& col = map_vector_.at(y);
        for (int x = 0; x < col.size(); x++) {
            if (col.at(x).getType() == MapCell::Type::START) {
                return {static_cast<float>(x) + 0.5, static_cast<float>(y) + 0.5};
            }
        }
    }
    
    throw std::invalid_argument("Couldn't locate startzone in map");

}

class WordDelimitedByUnderscore: public std::string {};

std::istream& operator>>(std::istream& is, WordDelimitedByUnderscore& output) {
    std::getline(is, output, '_');
    return is;
}

std::vector<int> extractMetadata(const std::string& raw) {
    std::istringstream iss(raw, std::ios_base::in);
    std::vector<std::string> data((std::istream_iterator<WordDelimitedByUnderscore>(iss)),
                                    std::istream_iterator<WordDelimitedByUnderscore>());

    std::vector<int> result;
    result.resize(data.size());
    std::transform(data.cbegin(), data.cend(), result.begin(), 
            [](const std::string& input) { return std::stoi(input); }
    );
    
    return result;
}


MapCell::MapCell(const MapCell::Type type, int num) {
    type_ = type;
    num_ = num;
}



MapCell::MapCell(const std::string& desc) {
    if (desc.empty()) {
        type_ = Type::EMPTY;
    }
    else {
        if (desc == "w") {
            type_ = Type::WALL;
        }
        else if (desc.front() == 'g') {
            type_ = Type::GOAL;
            const auto metadata = extractMetadata(desc.substr(1));
            if (!metadata.empty()) {
                num_ = metadata.front();
                metadata_ = std::vector<int>(metadata.cbegin() + 1, metadata.cend());
            }
        }
        else if (desc.front() == 'o') {
            type_ = Type::OBJECT;
            const std::string numberPart = desc.substr(1);
            num_ = std::stoi(numberPart); // note: no length checking. All object must have a numberpart
        }
        else if (desc == "s") {
            type_ = Type::START;
        }
        else if (desc == "b"){
            type_ = Type::BOOST;
        }
        else if (desc == "d") {
          type_ = Type::DYNAMIC_OBSTACLE;
        }
        else {
            const std::string errormsg = "No matching cell for: " + desc;
            throw std::invalid_argument(errormsg); 
        }
    }
}

bool MapCell::isObstruction() const {
    return (type_ != Type::EMPTY && type_ != Type::START && type_ != Type::GOAL);
}

bool MapCell::isWall() const {
    return type_ == Type::WALL;
}

MapCell::Type MapCell::getType() const {
    return type_;
}

int MapCell::getNum() const {
    return num_;
}

std::vector<int> MapCell::getMetadata() const {
    return metadata_;
}


