#include "equinor2018_simulator/zone.h"

void Zone::setZoneBox(float x_min, float y_min, float x_max, float y_max)
{
    x_min_ = x_min;
    y_min_ = y_min;
    x_max_ = x_max;
    y_max_ = y_max;
}

void Zone::setYaw(float yaw) {
    yaw_ = yaw;
}

void Zone::setCorrectObservation(int correct) {
    observation_ = correct;
}

bool Zone::isInside(float x, float y) const
{
    return (x_min_ < x) && (x < x_max_) && (y_min_ < y) && (y < y_max_);
}

float Zone::getMinX() const { return x_min_; }
float Zone::getMinY() const { return y_min_; }
float Zone::getMaxX() const { return x_max_; }
float Zone::getMaxY() const { return y_max_; }
float Zone::getYaw() const { return yaw_; }
int Zone::getCorrectObservation() const { return observation_; }
