#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#define grav 9.81

class Wave {
   public:
    double mCelerity, mHeight, mPeriod, mWaterDepth, mPrecision;
    double mWaveLength, mMaxHorVelocity;

    bool gotWaveLength = false;

    Wave();
    ~Wave();

    void setCelerity(double _celerity);
    void setHeight(double _height);
    void setPeriod(double _period);
    void setWaterDepth(double _waterDepth);
    void setPrecision(double _precision);
    void setmaxHorVelocity();

    double getHorizontal(double _depth, double _x = 0, double _t = 0);
    double getVertical(double _depth, double _x = 0, double _t = 0);

    double getHorizontalVelocity(double _depth, double _x = 0, double _t = 0);
    double getVerticalVelocity(double _depth, double _x = 0, double _t = 0);
    double getHorizontalAcceleration(double _depth, double _x = 0,
                                     double _t = 0);
    double getVerticalAcceleration(double _depth, double _x = 0, double _t = 0);
    double getEta(double _time);

    double getWaveLength();
    void calculateWaveLength();
};
