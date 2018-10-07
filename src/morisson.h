#include <pile.h>
#include <Wave.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
class Morisson {
   public:
    Pile* mPile;
    Wave* mWave;
    double mReynolds, mTimestep, mDragCoefficient, mInertiaCoefficient;
    double mNu = 0.000001;
    double mRhoWater = 1026;
    double mTotalHorizontal = 0;
    double mFD_tot, mFM_tot, mLastTime, mLastPlace, mEtaWaveHeight;
    std::vector<double> mForcesM, mForcesD, mAccelerations, mHorVelocities,
        mDepths;
    std::string mForcesStringM, mForcesStringD;
    std::string mForcesInTime;
    std::vector<std::string> mCalculationSteps;

    std::string mForcesMoverT, mForcesDoverT;

    Morisson();
    Morisson(Pile* _Pile, Wave* _Wave);
    ~Morisson();

    double calculateHorizontalForce_D(double _depth, double dz,
                                      double _time = 0, double _x = 0);
    double calculateHorizontalForce_M(double _depth, double dx,
                                      double _time = 0, double _x = 0);
    void setNu(double _nu);
    void calculateReynolds();

    void calculateCsReynolds();
    void storeCalculation();

    void printForcesD();
    void printForcesM();
    void setTimestep(double _timestep = 0.1);

    bool isMorissonValid();
    void calculateForces(double dz, double _time = 0, double _x = 0);

    void writeToFile(std::string _filename = "outputLog.csv");

    void calculateForcesOverTime(double _startTime, double _endTime, double dz,
                                 double _x = 0);
};
