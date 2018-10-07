#include <morisson.h>

Morisson::Morisson() {}
Morisson::~Morisson() {}

Morisson::Morisson(Pile *_Pile, Wave *_Wave) {
    mPile = _Pile;
    mWave = _Wave;
    calculateCsReynolds();
}

bool Morisson::isMorissonValid() {
    return mPile->isSlender(mWave->getWaveLength());
}

void Morisson::setNu(double _nu) { mNu = _nu; }
void Morisson::calculateReynolds() {
    mReynolds = mWave->mMaxHorVelocity * mPile->mDiameter / mNu;
}
void Morisson::calculateCsReynolds() {
    calculateReynolds();
    if (mReynolds < (250000)) {
        mDragCoefficient = 1.2;
        mInertiaCoefficient = 2;
    } else if (mReynolds > 500000) {
        mInertiaCoefficient = 1.5;
        mDragCoefficient = 0.7;
    } else {
        mInertiaCoefficient = (2.5 + mReynolds * 0.00001) * 0.5;
        std::cout << "mInertiaCoefficient: " << mInertiaCoefficient
                  << std::endl;
        mDragCoefficient = 1;
    }
}

double Morisson::calculateHorizontalForce_D(double _depth, double dz,
                                            double _time, double _x) {
    double horizontalWaveVelocity =
        mWave->getHorizontalVelocity(_depth, _x, _time);
    double F_D = 0.5 * mDragCoefficient * mRhoWater * mPile->mDiameter *
                 horizontalWaveVelocity * std::abs(horizontalWaveVelocity) * dz;
    mHorVelocities.push_back(horizontalWaveVelocity);
    return F_D;
}

double Morisson::calculateHorizontalForce_M(double _depth, double dz,
                                            double _time, double _x) {
    double horizontalAcceleration =
        mWave->getHorizontalAcceleration(_depth, _x, _time);
    double F_M = mInertiaCoefficient * mRhoWater * M_PI * mPile->mDiameter *
                 mPile->mDiameter * (0.25) * horizontalAcceleration * dz;
    mAccelerations.push_back(horizontalAcceleration);
    return F_M;
}

void Morisson::calculateForces(double _dz, double _time, double _x) {
    double dz = _dz;
    double waterdepth = mWave->mWaterDepth;
    mEtaWaveHeight = mWave->getEta(_time);
    int numberOfIntervals = std::ceil((mEtaWaveHeight + waterdepth) / dz);

    calculateCsReynolds();

    mForcesD.clear();
    mForcesM.clear();
    mAccelerations.clear();
    mHorVelocities.clear();
    mDepths.clear();

    mAccelerations.reserve(numberOfIntervals);
    mHorVelocities.reserve(numberOfIntervals);
    mForcesM.reserve(numberOfIntervals);
    mForcesD.reserve(numberOfIntervals);
    mDepths.reserve(numberOfIntervals);

    mLastTime = _time;
    mLastPlace = _x;

    mTotalHorizontal = 0;
    mFM_tot = 0;
    mFD_tot = 0;
    double depth = mEtaWaveHeight - dz * 0.5;
    for (auto i = 0; i < numberOfIntervals; i++) {
        if (i == numberOfIntervals - 1) {
            if (((-1) * depth) > mWave->mWaterDepth) {
                dz = std::abs((-1) * mDepths[i - 1] - mWave->mWaterDepth);
                depth = mDepths[i - 1] + dz * 0.5;
            }
        }
        mForcesD.push_back(calculateHorizontalForce_D(depth, dz, _time, _x));
        mForcesM.push_back(calculateHorizontalForce_M(depth, dz, _time, _x));

        mFM_tot += mForcesM[i];
        mFD_tot += mForcesD[i];
        mDepths.push_back(depth);

        mTotalHorizontal += (mForcesD[i] + mForcesM[i]);

        depth -= dz;
    }
    printForcesD();
    printForcesM();
    storeCalculation();
}

void Morisson::printForcesD() {
    for (auto x : mForcesD) {
        mForcesStringD += std::to_string(x) + "\n";
    }
}
void Morisson::printForcesM() {
    for (auto x : mForcesM) {
        mForcesStringM += std::to_string(x) + "\n";
    }
}

void Morisson::writeToFile(std::string _filename) {
    std::string outputText = "";
    for (auto x : mCalculationSteps) {
        outputText += x + "\n";
    }
    outputText += mForcesInTime;
    std::ofstream Ofile;
    Ofile.open(_filename);
    Ofile << outputText;
    Ofile.close();
}
void Morisson::storeCalculation() {
    std::string outputText;
    outputText += "Pile data\nPile Diameter:," +
                  std::to_string(mPile->mDiameter) + ",Pile Roughness:," +
                  std::to_string(mPile->mSurfaceRoughness) + "\nMorisson is,";
    outputText += "Drag coefficient:," + std::to_string(mDragCoefficient) +
                  ",Inertia Coefficient:," +
                  std::to_string(mInertiaCoefficient) + ",Reynolds:," +
                  std::to_string(mReynolds) + "\n";
    outputText += (isMorissonValid() == true) ? "Valid" : "Not Valid";

    outputText += "\nWave data,time:," + std::to_string(mLastTime) +
                  ",location:," + std::to_string(mLastPlace) + "\nPeriod:," +
                  std::to_string(mWave->mPeriod) + ",Wavelength:," +
                  std::to_string(mWave->mWaveLength) + ",waterdepth:," +
                  std::to_string(mWave->mWaterDepth) + ", eta wave height:," +
                  std::to_string(mEtaWaveHeight) + "\n";
    outputText +=
        "Drag Force [N] , Ineria Force [N],Acceleration "
        "[m/s^2],HorVelocity[m/s],Depth [m]\n";

    for (size_t i = 0; i < mForcesD.size(); i++) {
        outputText += std::to_string(mForcesD[i]) + "," +
                      std::to_string(mForcesM[i]) + "," +
                      std::to_string(mAccelerations[i]) + "," +
                      std::to_string(mHorVelocities[i]) + "," +
                      std::to_string(mDepths[i]) + "\n";
    }
    outputText += "Total Drag,Total Inertia\n" + std::to_string(mFD_tot) + "," +
                  std::to_string(mFM_tot) + "\n";
    outputText += "Total Force: ," + std::to_string(mTotalHorizontal) + "\n";
    mCalculationSteps.push_back(outputText);
}

void Morisson::calculateForcesOverTime(double _starTime, double _endTime,
                                       double dz, double _x) {
    int numberOfIntervals = (_endTime - _starTime) / mTimestep + 1;
    mForcesDoverT.reserve(numberOfIntervals);
    mForcesMoverT.reserve(numberOfIntervals);
    double t = _starTime;
    mForcesInTime =
        "Wave forces over time\n time [s],Drag Force [N], Inertia "
        "Force[N],Total Force [N], Eta Wave [m]\n";
    for (size_t i = 0; i < numberOfIntervals; i++) {
        calculateForces(dz, t, _x);
        mForcesDoverT.push_back(mFD_tot);
        mForcesMoverT.push_back(mFM_tot);
        mForcesInTime += std::to_string(t) + "," + std::to_string(mFD_tot) +
                         "," + std::to_string(mFM_tot) + "," +
                         std::to_string(mFD_tot + mFM_tot) + "," +
                         std::to_string(mEtaWaveHeight) + "\n";
        t += mTimestep;
    }
}

void Morisson::setTimestep(double _t) { mTimestep = _t; }
