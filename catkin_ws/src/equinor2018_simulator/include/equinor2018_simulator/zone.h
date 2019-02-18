#ifndef ENDZONE_H
#define ENDZONE_H

class Zone {
    public:
        void setZoneBox(float x_min, float y_min, float x_max, float y_max);
        void setYaw(float yaw);
        void setCorrectObservation(int correct);
        bool isInside(float x, float y) const;
        float getMinX() const;
        float getMinY() const;
        float getMaxX() const;
        float getMaxY() const;
        float getYaw() const;
        int getCorrectObservation() const;
    private:
        float x_min_, y_min_, x_max_, y_max_, yaw_ = 0.f;
        int observation_ = 0;

};

#endif // ENDZONE_H
