#include "mathtools.h"
#include <vector>
#include <set>
#include <stdexcept>
#include <functional>
#include <cmath>
#include <limits>

float bissection( std::function<float(float)> equation, const std::vector<float> intervals){
    if(intervals.empty()){
        return NAN;
    }

    float rootFound = NAN;

    for( int i = 1; i < intervals.size(); i++ ){
        float sI = intervals[i-1];
        float eI = intervals[i];
        while (((equation(sI) < .0 && equation(eI) >= .0 ) || (equation(eI) < .0 && equation(sI) >= .0 ))&&(std::fabs(eI - sI)>0.00001)){
            rootFound = sI + (eI - sI)/2.f;
            if((equation(sI) < .0 && equation(rootFound) >= .0 ) || (equation(rootFound) < .0 && equation(sI) >= .0 )){
                eI = rootFound;
            } else {
                sI = rootFound;
            }
        }
        if(!std::isnan(rootFound)) {
            return rootFound;
        }
    }
    return rootFound;
}

std::array<float, 2> quadraticRoots(float a, float b, float c){
    float delta = std::pow(b,2) - 4*a*c;
    if(delta < 0.0){
        return {NAN, NAN};
    }
    if(std::fabs(delta)<0.0001){
        return {-b/(2.f*a), NAN};
    }
    return {(-b - std::sqrt(delta))/(2.f*a), (-b + std::sqrt(delta))/(2.f*a) };
}

std::array<float, 3> cubicRoots(float a, float b, float c, float d, float startInterval, float endInterval){
    if(endInterval <= startInterval){
        throw std::range_error("invalid range");
    }
    auto dCoeff = cubicDegreePolDerivative(a, b, c, d);
    auto dRoots = quadraticRoots(dCoeff[2], dCoeff[1], dCoeff[0]);

    std::vector<float> intervals;
    intervals.push_back(startInterval);
    if(!std::isnan(dRoots[0]) && startInterval < dRoots[0] && dRoots[0] < endInterval){
        intervals.push_back(dRoots[0]);
    }
    if(!std::isnan(dRoots[1]) && startInterval < dRoots[1] && dRoots[1] < endInterval){
        intervals.push_back(dRoots[1]);
    }
    intervals.push_back(endInterval);

    auto equation = [a, b, c, d](float x){
        return float(d + c*x + b*std::pow(x,2) + a*std::pow(x,3));
    };

    float rootFound = bissection(equation, intervals);

    float a1 = a;
    float b1 = b + a1 * rootFound;
    float c1 = c + b1 * rootFound;
    auto qRoots = quadraticRoots(a1, b1, c1);

    bool insertRootFound = true;
    std::set<float> toSort;
    for( auto &root : qRoots){
        if(!std::isnan(root)){
            if(std::fabs(root - rootFound) < 0.00001){
                insertRootFound = false;
                toSort.insert(root + (root - rootFound)/2.);
            } else {
                toSort.insert(root);
            }
        }
    }
    if(insertRootFound)toSort.insert(rootFound);

    std::vector<float> retVec(3, NAN);
    auto toSortIterator = toSort.begin();
    for(int i = 0; i < toSort.size(); i++){
        retVec[i] = *(toSortIterator++);
    }
    return {retVec[0], retVec[1], retVec[2]};
}

std::array<float, 4> fourthRoots(float a, float b, float c, float d, float e, float startInterval, float endInterval){
    if(endInterval <= startInterval){
        throw std::range_error("invalid range");
    }
    auto dCoeff = fourthDegreePolDerivative(a, b, c, d, e);
    auto dRoots = cubicRoots(dCoeff[3], dCoeff[2], dCoeff[1], dCoeff[0]);

    std::vector<float> intervals;
    intervals.push_back(startInterval);
    for(auto &root : dRoots){
        if(!std::isnan(root) && startInterval < root && root < endInterval){
            intervals.push_back(root);
        }
    }
    intervals.push_back(endInterval);

    auto equation = [a, b, c, d, e](float x){
        return float(e + d*x + c*std::pow(x,2) + b*std::pow(x,3) + a*std::pow(x,4));
    };

    float rootFound = bissection(equation, intervals);
    if(std::isnan(rootFound)){
        return {NAN, NAN, NAN, NAN};
    }

    float a1 = a;
    float b1 = b + a1*rootFound;
    float c1 = c + b1*rootFound;
    float d1 = d + c1*rootFound;
    auto qRoots = cubicRoots(a1, b1, c1, d1);

    bool insertRootFound = true;
    std::set<float> toSort;
    for( auto &root : qRoots){
        if(!std::isnan(root)){
            if(std::fabs(root - rootFound) < 0.00001){
                insertRootFound = false;
                toSort.insert(root + (root - rootFound)/2.);
            } else {
                toSort.insert(root);
            }
        }
    }
    if(insertRootFound)toSort.insert(rootFound);

    std::vector<float> retVec(4, NAN);
    auto toSortIterator = toSort.begin();
    for(int i = 0; i < toSort.size(); i++){
        retVec[i] = *(toSortIterator++);
    }
    return {retVec[0], retVec[1], retVec[2], retVec[3]};
}

std::array<float, 5> fifthRoots(float a, float b, float c, float d, float e, float f, float startInterval, float endInterval){
    if(endInterval <= startInterval){
        throw std::range_error("invalid range");
    }
    auto dCoeff = fifthDegreePolDerivative(a, b, c, d, e, f);
    auto dRoots = fourthRoots(dCoeff[4], dCoeff[3], dCoeff[2], dCoeff[1], dCoeff[0]);

    std::vector<float> intervals;
    intervals.push_back(startInterval);
    for(auto &root : dRoots){
        if(!std::isnan(root) && startInterval < root && root < endInterval){
            intervals.push_back(root);
        }
    }
    intervals.push_back(endInterval);

    auto equation = [a, b, c, d, e, f](float x){
        return float(f + e*x + d*std::pow(x,2) + c*std::pow(x,3) + b*std::pow(x,4) + a*std::pow(x,5));
    };

    float rootFound = bissection(equation, intervals);
    if(std::isnan(rootFound)){
        return {NAN, NAN, NAN, NAN, NAN};
    }

    float a1 = a;
    float b1 = b + a1*rootFound;
    float c1 = c + b1*rootFound;
    float d1 = d + c1*rootFound;
    float e1 = e + d1*rootFound;
    auto qRoots = fourthRoots(a1, b1, c1, d1, e1);

    bool insertRootFound = true;
    std::set<float> toSort;
    for( auto &root : qRoots){
        if(!std::isnan(root)){
            if(std::fabs(root - rootFound) < 0.00001){
                insertRootFound = false;
                toSort.insert(root + (root - rootFound)/2.);
            } else {
                toSort.insert(root);
            }
        }
    }
    if(insertRootFound)toSort.insert(rootFound);

    std::vector<float> retVec(5, NAN);
    auto toSortIterator = toSort.begin();
    for(int i = 0; i < toSort.size(); i++){
        retVec[i] = *(toSortIterator++);
    }
    return {retVec[0], retVec[1], retVec[2], retVec[3], retVec[4]};

}

std::array<float, 3> cubicDegreePolDerivative(float a, float b, float c, float d){
    return {c, 2*b, 3*a};
}

std::array<float, 4> fourthDegreePolDerivative(float a, float b, float c, float d, float e){
    return {d,2*c,3*b,4*a};
}

std::array<float, 5> fifthDegreePolDerivative(float a, float b, float c, float d, float e, float f){
    return {e,2*d,3*c,4*b,5*a};
}

std::array<float, 2> evaluateCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float a ){
    float x = x0 + a*(-3*x0 + 3*x1) + std::pow(a,2)*(3*x0 - 6*x1 + 3*x2) + std::pow(a,3)*(-x0 + 3*x1 - 3*x2 + x3);
    float y = y0 + a*(-3*y0 + 3*y1) + std::pow(a,2)*(3*y0 - 6*y1 + 3*y2) + std::pow(a,3)*(-y0 + 3*y1 - 3*y2 + y3);
    return {x, y};
}

std::array<float, 2> nearestPointInCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float px, float py ){

    float a = -std::pow(x0,2) + 6*x0*x1 - 9*std::pow(x1,2) - 6*x0*x2 + 18*x1*x2 - 9*std::pow(x2,2) + 2*x0*x3 - 6*x1*x3 + 6*x2*x3 - std::pow(x3,2) - std::pow(y0,2) + 6*y0*y1 - 9*std::pow(y1,2) - 6*y0*y2 + 18*y1*y2 - 9*std::pow(y2,2) + 2*y0*y3 - 6*y1*y3 + 6*y2*y3 - std::pow(y3,2);
    float b = 5*std::pow(x0,2) - 25*x0*x1 + 30*std::pow(x1,2) + 20*x0*x2 - 45*x1*x2 + 15*std::pow(x2,2) - 5*x0*x3 + 10*x1*x3 - 5*x2*x3 + 5*std::pow(y0,2) - 25*y0*y1 + 30*std::pow(y1,2) + 20*y0*y2 - 45*y1*y2 + 15*std::pow(y2,2) - 5*y0*y3 + 10*y1*y3 - 5*y2*y3;
    float c = -10*std::pow(x0,2) + 40*x0*x1 - 36*std::pow(x1,2) - 24*x0*x2 + 36*x1*x2 - 6*std::pow(x2,2) + 4*x0*x3 - 4*x1*x3 - 10*std::pow(y0,2) + 40*y0*y1 - 36*std::pow(y1,2) - 24*y0*y2 + 36*y1*y2 - 6*std::pow(y2,2) + 4*y0*y3 - 4*y1*y3;
    float d = -(px*x0) + 10*std::pow(x0,2) + 3*px*x1 - 30*x0*x1 + 18*std::pow(x1,2) - 3*px*x2 + 12*x0*x2 - 9*x1*x2 + px*x3 - x0*x3 - py*y0 + 10*std::pow(y0,2) + 3*py*y1 - 30*y0*y1 + 18*std::pow(y1,2) - 3*py*y2 + 12*y0*y2 - 9*y1*y2 + py*y3 - y0*y3;
    float e = 2*px*x0 - 5*std::pow(x0,2) - 4*px*x1 + 10*x0*x1 - 3*std::pow(x1,2) + 2*px*x2 - 2*x0*x2 + 2*py*y0 - 5*std::pow(y0,2) - 4*py*y1 + 10*y0*y1 - 3*std::pow(y1,2) + 2*py*y2 - 2*y0*y2;
    float f = -(px*x0) + std::pow(x0,2) + px*x1 - x0*x1 - py*y0 + std::pow(y0,2) + py*y1 - y0*y1;

    auto roots = fifthRoots(a, b, c, d, e, f);
    std::vector<float> candidates;
    candidates.push_back(0.f);
    for(auto &root : roots){
        if(!std::isnan(root) && 0.f < root && root < 1.f ){
            candidates.push_back(root);
        }
    }
    candidates.push_back(1.f);

    float minDist = std::numeric_limits<float>::max();
    float minDistParameter = 0;
    for(auto &candidate : candidates){
        auto pointInCurve = evaluateCurve(x0, y0, x1, y1, x2, y2, x3, y3, candidate );
        float dist = std::sqrt(std::pow(px - pointInCurve[0],2) + std::pow(py - pointInCurve[1],2));
        if(minDist > dist){
            minDist = dist;
            minDistParameter = candidate;
        }
    }

    return {minDistParameter, minDist};
}

std::array<float, 4> cutUntil(float x0, float x1, float x2, float x3, float s){
    float nx3 = -(std::pow(-1 + s,3)*x0) + 3*(-1 + s)*s*((-1 + s)*x1 - s*x2) + std::pow(s,3)*x3;
    float nx2 = std::pow(-1 + s,2)*x0 - 2*(-1 + s)*s*x1 + std::pow(s,2)*x2;
    float nx1 = -((-1 + s)*x0) + s*x1;
    float nx0 = x0;
    return {nx0, nx1, nx2, nx3};
}

std::array<float, 4> cutAfter(float x0, float x1, float x2, float x3, float s){
    float nx3 = x3;
    float nx2 = x2 + s*(-x2 + x3);
    float nx1 = x1 + 2*s*(-x1 + x2) + std::pow(s,2)*(x1 - 2*x2 + x3);
    float nx0 = -(std::pow(-1 + s,3)*x0) + 3*(-1 + s)*s*((-1 + s)*x1 - s*x2) + std::pow(s,3)*x3;
    return {nx0, nx1, nx2, nx3};
}

std::array<float, 8> cutPieceUntil(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float s){
    auto xCoords = cutUntil(x0, x1, x2, x3, s);
    auto yCoords = cutUntil(y0, y1, y2, y3, s);
    return {xCoords[0], yCoords[0], xCoords[1], yCoords[1], xCoords[2], yCoords[2], xCoords[3], yCoords[3]};
}

std::array<float, 8> cutPieceAfter(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float s){
    auto xCoords = cutAfter(x0, x1, x2, x3, s);
    auto yCoords = cutAfter(y0, y1, y2, y3, s);
    return {xCoords[0], yCoords[0], xCoords[1], yCoords[1], xCoords[2], yCoords[2], xCoords[3], yCoords[3]};
}

std::array<float, 4>tangentVectorsAtBoundaries(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
    return {-3*(x0 - x1), -3*(y0 - y1), -3*(x2 - x3), -3*(y2 - y3)};
}

float bezierSimpsonsRule(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float a, float b){
    auto func = [=](float t){
        return 3*std::sqrt(
                std::pow(-x0 + x1 + 2*t*(x0 - 2*x1 + x2) + std::pow(t,2)*(-x0 + 3*x1 - 3*x2 + x3),2) +
                std::pow(-y0 + y1 + 2*t*(y0 - 2*y1 + y2) + std::pow(t,2)*(-y0 + 3*y1 - 3*y2 + y3),2)
                );
    };
    return -((a - b)*(func(a) + func(b) + 4*func((a + b)/2.)))/6.;
}

float bezierLength(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3){
    const int n = 10;
    const float delta = 1.0/float(n);

    float length = 0.0;
    for(int i = 0; i < n; i++){
        float a = float(i)* delta;
        float b = a + delta;
        length += bezierSimpsonsRule(x0, y0, x1, y1, x2, y2, x3, y3, a, b);
    }
    return length;
}

float bezierIntervalLength(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float sI, float eI){
    const int n = 10;
    const float delta = (eI - sI)/float(n);

    float length = 0.0;
    for(int i = 0; i < n; i++){
        float a = sI + float(i)* delta;
        float b = a + delta;
        length += bezierSimpsonsRule(x0, y0, x1, y1, x2, y2, x3, y3, a, b);
    }
    return length;
}

float bezierParameterForLength(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float length){
    float totalLength = bezierLength(x0, y0, x1, y1, x2, y2, x3, y3);
    if(length > totalLength){
        return NAN;
    }
    float tol = 0.0001;
    if(std::fabs(totalLength - length) < tol){
        return 1.0;
    }
    if(std::fabs(length) < tol){
        return 0.0;
    }

    float lengthSI = 0;
    float sI = 0.0;
    float eI = 1.0;
    float middle = 0.5;
    float lengthMiddle = lengthSI + bezierIntervalLength(x0, y0, x1, y1, x2, y2, x3, y3, sI, middle);
    while( std::fabs(lengthMiddle - length) > tol){
        if(lengthMiddle > length){
            eI = middle;
        }else{
            lengthSI = lengthMiddle;
            sI = middle;
        }
        middle = sI + (eI-sI)/2.0;
        lengthMiddle = lengthSI + bezierIntervalLength(x0, y0, x1, y1, x2, y2, x3, y3, sI, middle);
    }
    return middle;
}


