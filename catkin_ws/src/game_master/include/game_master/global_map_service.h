#ifndef GLOBAL_MAP_PUBLISHER_H
#define GLOBAL_MAP_PUBLISHER_H

#include <ros/ros.h>
#include <ascend_msgs/GlobalMap.h>
#include <equinor2018_simulator/map_handle.h>

class GlobalMapService {
    public:
        GlobalMapService() = delete;
        GlobalMapService(std::shared_ptr<ros::NodeHandle> nh, const std::string& csv_file);
        
        const MapHandle& getMapHandleRef() const;
    private:
        ros::ServiceServer service_;
        MapHandle map_handle_;
        
        bool mapServiceHandlerService(ascend_msgs::GlobalMap::Request& req, ascend_msgs::GlobalMap::Response& res);
};


#endif // GLOBAL_MAP_PUBLISHER_H

