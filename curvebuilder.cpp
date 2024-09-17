#include "curvebuilder.h"

bool CurveBuilder::IsReadyToBuild = false;
std::function<void(void)> CurveBuilder::onCloseCurveCallback = {};

std::vector<std::shared_ptr<point>> CurveBuilder::points;
std::list<std::shared_ptr<curvePiece>> CurveBuilder::pieces;

void CurveBuilder::startBuildingCurve(){
    CurveBuilder::IsReadyToBuild = true;
}

bool CurveBuilder::willBuildCurve(){
    return CurveBuilder::IsReadyToBuild;
}

void CurveBuilder::cancelBuildCurve(){
    CurveBuilder::IsReadyToBuild = false;
}

void CurveBuilder::setCloseCurveCallback(std::function<void(void)> onCloseCurve){
    onCloseCurveCallback = onCloseCurve;
}

void CurveBuilder::onClick(float x, float y, QWidget* widget){
    if(points.empty()){
        startCurve(x, y);
    } else {
        buildPiece(x, y);
    }
}

void CurveBuilder::startCurve(float x, float y){
    auto newPoint = std::make_shared<point>(x, y);
    DrawableManager::insertItem(newPoint);
    DragManager::insertDraggable(newPoint);
    points.push_back(newPoint);
}

void CurveBuilder::buildPiece(float x, float y){
    shared_ptr<point> newPoint = std::make_shared<point>(x, y);

    if(pieces.empty()){
        float x1 = points.back()->getPosX() + (x - points.back()->getPosX())*(1./3.);
        float y1 = points.back()->getPosY() + (y - points.back()->getPosY())*(1./3.);
        float x2 = points.back()->getPosX() + (x - points.back()->getPosX())*(2./3.);
        float y2 = points.back()->getPosY() + (y - points.back()->getPosY())*(2./3.);

        auto newpiece = std::make_shared<curvePiece>(points.back(), x1, y1, x2, y2, newPoint);
        newpiece->setToDraw();
        newpiece->setToDrag();
        pieces.push_back(newpiece);
    } else {
        auto firstPoint = pieces.back()->point3();

        auto curveStart = pieces.front()->point0();
        float sx = curveStart->getPosX();
        float sy = curveStart->getPosY();
        float dist = std::sqrt(std::pow(sx - x,2) + std::pow(sy - y,2));

        float x2 = -1;
        float y2 = -1;

        bool willCloseTheCurve = false;

        if(dist < DrawableManager::getMinDistance()){
            newPoint = curveStart;
            float v1x = pieces.front()->v0x();
            float v1y = pieces.front()->v0y();

            x2 = (-v1x + 3*newPoint->getPosX())/3.;
            y2 = (-v1y + 3*newPoint->getPosY())/3.;

            willCloseTheCurve = true;

        } else {
            x2 = firstPoint->getPosX() + (x - firstPoint->getPosX())*(2./3.);
            y2 = firstPoint->getPosY() + (y - firstPoint->getPosY())*(2./3.);

        }

        float v0x = pieces.back()->v1x();
        float v0y = pieces.back()->v1y();
        float x1 = (-v0x + 3*firstPoint->getPosX())/3.;
        float y1 = (-v0y + 3*firstPoint->getPosY())/3.;

        auto newpiece = std::make_shared<curvePiece>(firstPoint, x1, y1, x2, y2, newPoint);
        newpiece->setToDraw();
        newpiece->setToDrag();
        pieces.push_back(newpiece);

        if(willCloseTheCurve)
            closeCurve();
    }
}

void CurveBuilder::closeCurve(){
    points.clear();
    IsReadyToBuild = false;
    onCloseCurveCallback();
}

