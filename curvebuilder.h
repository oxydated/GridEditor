#ifndef CURVEBUILDER_H
#define CURVEBUILDER_H

#include "drawable.h"
#include "point.h"
#include "handle.h"

class CurveBuilder
{
public:
    static bool willBuildCurve();
    static void onClick();
    static void onEsc();
    static void startCurve();
    static void buildPiece();
    static void closeCurve();
    static void setTreeWidget();
    static void InsertCurveInTree();
    static void createPoint();
    static void createHandle();
    static void createCurvePiece();

};

#endif // CURVEBUILDER_H
