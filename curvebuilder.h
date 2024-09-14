#ifndef CURVEBUILDER_H
#define CURVEBUILDER_H

#include "drawable.h"
#include "point.h"
#include "handle.h"
#include "curvepiece.h"
#include <list>
#include <qwidget.h>
#include <functional>
#include <map>

class CurveBuilder
{
public:
    static void startBuildingCurve();
    static bool willBuildCurve();
    static void onClick(float x, float y, QWidget* widget);
    static void onEsc();
    static void startCurve(float x, float y);
    static void buildPiece(float x, float y);
    static void closeCurve();
    static void setTreeWidget();
    static void InsertCurveInTree();
    static void createPoint();
    static void createHandle();
    static void createCurvePiece();
    static void cancelBuildCurve();
    static void setCloseCurveCallback(std::function<void(void)> onCloseCurve);

private:
    static bool IsReadyToBuild;
    static std::vector<std::shared_ptr<point>> points;
    static std::list<std::shared_ptr<curvePiece>> pieces;
    static std::function<void(void)> onCloseCurveCallback;
//    static std::map<int, std::list<std::shared_ptr<curvePiece>>> curves;
//    static int nextAvailableIndex;

};

#endif // CURVEBUILDER_H
