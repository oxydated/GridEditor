#ifndef MATHTOOLS_H
#define MATHTOOLS_H

#include <array>

using namespace std;

std::array<float, 2> quadraticRoots(float a, float b, float c);
std::array<float, 3> cubicRoots(float a, float b, float c, float d, float startInterval = 0., float endInterval = 1.);
std::array<float, 4> fourthRoots(float a, float b, float c, float d, float e, float startInterval = 0., float endInterval = 1.);
std::array<float, 5> fifthRoots(float a, float b, float c, float d, float e, float f, float startInterval = 0., float endInterval = 1.);

std::array<float, 3> cubicDegreePolDerivative(float a, float b, float c, float d);
std::array<float, 4> fourthDegreePolDerivative(float a, float b, float c, float d, float e);
std::array<float, 5> fifthDegreePolDerivative(float a, float b, float c, float d, float e, float f);

std::array<float, 2> evaluateCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float a );
std::array<float, 2> nearestPointInCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float px, float py );

std::array<float, 4> cutUntil(float x0, float x1, float x2, float x3, float s);
std::array<float, 4> cutAfter(float x0, float x1, float x2, float x3, float s);

std::array<float, 8> cutPieceUntil(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float s);
std::array<float, 8> cutPieceAfter(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float s);

std::array<float, 4>tangentVectorsAtBoundaries(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);


#endif // MATHTOOLS_H
