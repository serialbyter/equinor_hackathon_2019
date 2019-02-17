#include "game_master/global_map_service.h"
#include <ascend_msgs/GlobalMap.h>

#include <equinor2018_simulator/map_handle.h>


GlobalMapService::GlobalMapService(std::shared_ptr<ros::NodeHandle> nh, const std::string& csv_file)
    : map_handle_(csv_file)
{
    service_ = nh->advertiseService("/GlobalMap", &GlobalMapService::mapServiceHandlerService, this);
}

bool GlobalMapService::mapServiceHandlerService(ascend_msgs::GlobalMap::Request& req, ascend_msgs::GlobalMap::Response& res) {
	ascend_msgs::Map map_msg = map_handle_.createMapMessage();
    res.dim = map_msg.dim;
    res.data = map_msg.data;

    return true;
}

const MapHandle& GlobalMapService::getMapHandleRef() const {
    return map_handle_;
}

