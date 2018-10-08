#include <Wave.h>

Wave::Wave() { std::cout << "Wave created" << std::endl; }
Wave::~Wave() {}

double Wave::getWaveLength() {
    if (!gotWaveLength) {
        calculateWaveLength();
    }
    return mWaveLength;
}

void Wave::calculateWaveLength() {
    mWaveLength = 10.0;
    double L = 1.0;
    int i = 0;
    while (std::abs(L - mWaveLength) > mPrecision) {
        mWaveLength = L;
        L = grav * pow(mPeriod, 2) / (2 * M_PI) *
            std::tanh(2 * M_PI * mWaterDepth / mWaveLength);
        i++;
    }
    std::cout << "Required " << i << " iterations, wavelength :" << mWaveLength
              << std::endl;
    gotWaveLength = true;
    setmaxHorVelocity();
    setCelerity();
}

double Wave::getHorizontal(double _depth, double _x, double _t) {
    if (!gotWaveLength) return 0;
    double costheta = std::cos(2 * M_PI * (_x / mWaveLength + _t / mPeriod));
    return mHeight * 0.5 * grav * mPeriod *
           std::cosh(2 * M_PI * (_depth + mWaterDepth) / mWaveLength) /
           (std::cosh(2 * M_PI * mWaterDepth / mWaveLength) * mWaveLength) *
           costheta;
}

double Wave::getVertical(double _depth, double _x, double _t) {
    if (!gotWaveLength) return 0;
    double sintheta = std::sin(2 * M_PI * (_x / mWaveLength + _t / mPeriod));
    return mHeight * 0.5 * grav * mPeriod *
           std::sinh(2 * M_PI * (_depth + mWaterDepth) / mWaveLength) /
           std::cosh(2 * M_PI * mWaterDepth / mWaveLength) * sintheta;
}

double Wave::getHorizontalVelocity(double _depth, double _x, double _t) {
    return getHorizontal(_depth, _x, _t) + mCurrent;
}
double Wave::getVerticalVelocity(double _depth, double _x, double _t) {
    return getVertical(_depth, _x, _t);
}
// cos(2pi(x/L-t/T))'->-2pi/T(-sin(2pi(x/L-t/T))) ->
// 2pi/T*cos(2pi(x/L-t/T)-pi/2)
double Wave::getHorizontalAcceleration(double _depth, double _x, double _t) {
    return 2 * M_PI / mPeriod * getHorizontal(_depth, _x, _t + 0.25 * mPeriod);
}
// sin(2pi(x/L-t/T))'-> -2pi/T*cos(2pi(x/L-t/T))-> 2pi/T*sin(2pi(x/L-t/T)-pi/2)
double Wave::getVerticalAcceleration(double _depth, double _x, double _t) {
    return 2 * M_PI / mPeriod * getVertical(_depth, _x, _t + 0.25 * mPeriod);
}

double Wave::getEta(double _time) {
    return mHeight * 0.5 * std::cos(2 * M_PI * _time / mPeriod);
}
void Wave::setmaxHorVelocity() {
    mMaxHorVelocity = getHorizontalVelocity(mHeight * 0.5);
}
void Wave::setPrecision(double _precision) { mPrecision = _precision; }
void Wave::setCelerity() { mCelerity = mWaveLength / mPeriod; }
void Wave::setHeight(double _height) { mHeight = _height; }
void Wave::setPeriod(double _period) { mPeriod = _period; }
void Wave::setWaterDepth(double _waterDepth) { mWaterDepth = _waterDepth; }
void Wave::setCurrent(double _u) { mCurrent = _u; }
