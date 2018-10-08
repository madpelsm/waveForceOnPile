#include <Wave.h>
#include <pile.h>
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
    double lambda = 0.5;
    double mFD_tot, mFM_tot, mLastTime, mLastPlace, mEtaWaveHeight,
        mImpactForce, mt_max, m_impactAngle = -1;
    std::vector<double> mForcesM, mForcesD, mAccelerations, mHorVelocities,
        mDepths, mForcesDM;
    std::string mForcesStringM, mForcesStringD;
    std::string mForcesInTime;
    std::vector<std::string> mCalculationSteps;
    bool mOverTCalcStorage = true;

    std::vector<double> mForcesMoverT, mForcesDoverT, mForcesTotOverT,
        mImpactsOverT, mEtaWaveHeightOverT;

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
    void adjustCoefficientsForRoughtness();
    void setTimstepCalcStorage(double _toggle);

    void printForcesD();
    void printForcesM();
    void setTimestep(double _timestep = 0.1);

    bool isMorissonValid();
    void calculateForces(double dz, double _time = 0, double _x = 0);

    void writeToFile(std::string _filename = "outputLog.csv");

    void calculateForcesOverTime(double _startTime, double _endTime, double dz,
                                 double _x = 0);

    void print_timecalculation();
    void calculateWaveImpact();
    double find_time_of_max();
    double max_impact();
    void setImpactAngle(double _angle);
};
