#include <pile.h>

Pile::Pile() {}
Pile::Pile(double _Diameter) : mDiameter(_Diameter) {}
Pile::~Pile() {}
void Pile::setDiameter(double _D) { mDiameter = _D; }

void Pile::setSurfaceRoughness(double _r) { mSurfaceRoughness = _r; }

bool Pile::isSlender(double _WaveLength) {
    return mDiameter < 0.05 * _WaveLength;
}
