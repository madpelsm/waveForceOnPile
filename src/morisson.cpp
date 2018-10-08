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
        // interpolate
        mInertiaCoefficient = 2.5 - mReynolds * 0.00001;

        mDragCoefficient = (mReynolds - 250000) * (0.7 - 1.2) / (250000) + 1.2;
    }
    adjustCoefficientsForRoughtness();
}

void Morisson ::adjustCoefficientsForRoughtness() {
    double kD = mPile->mSurfaceRoughness / mPile->mDiameter;
    if (kD > 0.002) {
        mDragCoefficient *= 1.5;
        mInertiaCoefficient *= 0.6;
    } else if (kD <= 0.0 - 2 && kD > 0.0001) {
        mDragCoefficient *= 1.25;
        mInertiaCoefficient *= 0.8;
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

void Morisson::setTimstepCalcStorage(double _toggle) {
    if (_toggle != 1.0) {
        mOverTCalcStorage = false;
    } else {
        mOverTCalcStorage = true;
    }
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
    mForcesDM.clear();

    mAccelerations.reserve(numberOfIntervals);
    mHorVelocities.reserve(numberOfIntervals);
    mForcesM.reserve(numberOfIntervals);
    mForcesD.reserve(numberOfIntervals);
    mDepths.reserve(numberOfIntervals);
    mForcesDM.reserve(numberOfIntervals);

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
        mForcesDM.push_back(mForcesD[i] + mForcesM[i]);

        mTotalHorizontal += mForcesDM[i];

        depth -= dz;
    }
    printForcesD();
    printForcesM();
    calculateWaveImpact();
    mImpactsOverT.push_back(mImpactForce);
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

    printf("writen to file\n");
}
void Morisson::storeCalculation() {
    std::string outputText;
    outputText += "Pile data\nPile Diameter:," +
                  std::to_string(mPile->mDiameter) + ",Pile Roughness:," +
                  std::to_string(mPile->mSurfaceRoughness) + "\nMorisson is,";
    outputText += (isMorissonValid() == true) ? "Valid" : "Not Valid";
    outputText += "Drag coefficient:," + std::to_string(mDragCoefficient) +
                  ",Inertia Coefficient:," +
                  std::to_string(mInertiaCoefficient) + ",Reynolds:," +
                  std::to_string(mReynolds) + "\n";

    outputText += "\nWave data,time:," + std::to_string(mLastTime) +
                  ",location:," + std::to_string(mLastPlace) + "\nPeriod:," +
                  std::to_string(mWave->mPeriod) + ",Wavelength:," +
                  std::to_string(mWave->mWaveLength) + ",waterdepth:," +
                  std::to_string(mWave->mWaterDepth) + ", eta wave height:," +
                  std::to_string(mEtaWaveHeight) + "\n" + "Celerity m/s," +
                  std::to_string(mWave->mCelerity) + ",wave impact:," +
                  std::to_string(mImpactForce) + ",impact angle (deg)," +
                  std::to_string(m_impactAngle) + "\n";
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

    printf("Calculation stored\n");
}

void Morisson::calculateForcesOverTime(double _starTime, double _endTime,
                                       double dz, double _x) {
    int numberOfIntervals = (_endTime - _starTime) / mTimestep + 1;
    mForcesDoverT.clear();
    mForcesMoverT.clear();
    mEtaWaveHeightOverT.clear();
    mImpactsOverT.clear();
    mForcesTotOverT.clear();

    mForcesDoverT.reserve(numberOfIntervals);
    mForcesMoverT.reserve(numberOfIntervals);
    mEtaWaveHeightOverT.reserve(numberOfIntervals);
    mImpactsOverT.reserve(numberOfIntervals);
    mForcesTotOverT.reserve(numberOfIntervals);

    double t = _starTime;
    for (size_t i = 0; i < numberOfIntervals; i++) {
        calculateForces(dz, t, _x);
        if (mOverTCalcStorage == true) storeCalculation();
        mForcesDoverT.push_back(mFD_tot);
        mForcesMoverT.push_back(mFM_tot);
        mForcesTotOverT.push_back(mFM_tot + mFD_tot);
        mEtaWaveHeightOverT.push_back(mEtaWaveHeight);
        t += mTimestep;
    }
    if (m_impactAngle != -1) {
        mt_max = find_time_of_max();
        int index_max_totForce = (int)(mt_max / mTimestep);
        printf("force at %d: %f\n", index_max_totForce,
               mForcesTotOverT[index_max_totForce]);
        mForcesTotOverT[index_max_totForce] += max_impact();
    }
    print_timecalculation();
}

void Morisson::setTimestep(double _t) { mTimestep = _t; }

void Morisson::calculateWaveImpact() {
    mImpactForce = 0.5 * lambda * mEtaWaveHeight * 2 * M_PI * mPile->mDiameter *
                   mWave->mCelerity * mWave->mCelerity * mRhoWater;
}
void Morisson::print_timecalculation() {
    mForcesInTime =
        "Wave forces over time\n time [s],Drag Force [N], Inertia "
        "Force[N],Total Force [N],ImpactForce [N], Eta Wave [m]\n";
    for (size_t i = 0; i < mForcesTotOverT.size(); i++) {
        mForcesInTime += std::to_string(i * mTimestep) + "," +
                         std::to_string(mForcesDoverT[i]) + "," +
                         std::to_string(mForcesMoverT[i]) + "," +
                         std::to_string(mForcesTotOverT[i]) + "," +
                         std::to_string(mImpactsOverT[i]) + "," +
                         std::to_string(mEtaWaveHeightOverT[i]) + "\n";
    }
}

double Morisson::find_time_of_max() {
    double max = 0;
    int i = 0;
    int k = 0;
    for (k = 0; k < mForcesTotOverT.size(); k++) {
        if (mForcesTotOverT[k] > max) {
            max = mForcesTotOverT[k];
            i = k;
        }
    }
    return (double)(i * mTimestep);
}

double Morisson::max_impact() {
    double m = 0;
    for (auto x : mImpactsOverT) {
        if (m < x) m = x;
    }
    return m;
}

void Morisson::setImpactAngle(double _ang) { m_impactAngle = _ang; }
