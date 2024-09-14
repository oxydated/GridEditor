#include <cmath>
#include "curvepiece.h"

curvePiece::curvePiece(
        std::shared_ptr<point> sp,
        std::shared_ptr<handle> sh,
        std::shared_ptr<point> ep,
        std::shared_ptr<handle> eh ):
    sPoint(sp),
    sHandle(sh),
    ePoint(ep),
    eHandle(eh)
{}


//    auto newPoint = std::make_shared<point>(0.75, 0.25);
//    auto newHandle = std::make_shared<handle>(newPoint, -0.5, -0.5, true);

//    auto newPoint2 = std::make_shared<point>(0.45, 0.15);
//    auto newHandle2 = std::make_shared<handle>(newPoint2, -0.25, -0.75, false);

//    auto newCurvePiece = std::make_shared<curvePiece>(newPoint, newHandle, newPoint2, newHandle2);

//    DrawableManager::insertItem(newPoint);
//    DrawableManager::insertItem(newPoint2);
//    DrawableManager::insertItem(newHandle);
//    DrawableManager::insertItem(newHandle2);
//    DrawableManager::insertItem(newCurvePiece);

curvePiece::curvePiece(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3):
    sPoint(std::make_shared<point>(x0, y0)),
    sHandle(std::make_shared<handle>(sPoint, std::make_shared<point>(x1, y1), false)),
    ePoint(std::make_shared<point>(x3, y3)),
    eHandle(std::make_shared<handle>(ePoint, std::make_shared<point>(x2, y2), true))
{}

curvePiece::curvePiece(std::shared_ptr<point> sp, float x1, float y1, float x2, float y2, std::shared_ptr<point> ep):
    sPoint(sp),
    sHandle(std::make_shared<handle>(sPoint, std::make_shared<point>(x1, y1), false)),
    ePoint(ep),
    eHandle(std::make_shared<handle>(ePoint, std::make_shared<point>(x2, y2), true))
{}

float curvePiece::evaluateX(float a){
    float x0 = sPoint->getPosX();
    float x1 = sHandle->getPosX();
    float x3 = ePoint->getPosX();
    float x2 = eHandle->getPosX();
    return (1 + a*(-3 + (3 - a)*a))*x0 + a*(3*x1 + a*(-6*x1 + 3*x2 + a*(3*x1 - 3*x2 + x3)));
}
float curvePiece::evaluateY(float a){
    float y0 = sPoint->getPosY();
    float y1 = sHandle->getPosY();
    float y3 = ePoint->getPosY();
    float y2 = eHandle->getPosY();
    return (1 + a*(-3 + (3 - a)*a))*y0 + a*(3*y1 + a*(-6*y1 + 3*y2 + a*(3*y1 - 3*y2 + y3)));
}

float curvePiece::v0x(){
    float x0 = sPoint->getPosX();
    float x1 = sHandle->getPosX();
    return -3*(x0 - x1);
}

float curvePiece::v0y(){
    float y0 = sPoint->getPosY();
    float y1 = sHandle->getPosY();
    return -3*(y0 - y1);
}

float curvePiece::v1x(){
    float x3 = ePoint->getPosX();
    float x2 = eHandle->getPosX();
    return 3*(x2 - x3);
}

float curvePiece::v1y(){
    float y3 = ePoint->getPosY();
    float y2 = eHandle->getPosY();
    return 3*(y2 - y3);
}

const std::shared_ptr<point> curvePiece::point0(){
    return sPoint;
}

const std::shared_ptr<point> curvePiece::point3(){
    return ePoint;
}

void curvePiece::setToDraw(){
    DrawableManager::insertItem(sPoint);
    DrawableManager::insertItem(sHandle);
    DrawableManager::insertItem(ePoint);
    DrawableManager::insertItem(eHandle);
    DrawableManager::insertItem(shared_from_this());
    DrawableManager::Update();
}

void curvePiece::setToDrag(){
    DragManager::insertDraggable(sPoint);
    DragManager::insertDraggable(sHandle);
    DragManager::insertDraggable(ePoint);
    DragManager::insertDraggable(eHandle);
}

void curvePiece::draw(){
    float a = 0;
    int steps = 25;
    for(int i = 0; i < steps; i++){
        float b = a + 1./float(steps);
        float sx = evaluateX(a);
        float sy = evaluateY(a);
        float ex = evaluateX(b);
        float ey = evaluateY(b);
        DrawableManager::drawCurve(sx, sy, ex, ey);
        a = b;
    }
    sPoint->draw();
    sHandle->draw();
    ePoint->draw();
    eHandle->draw();
}

bool curvePiece::willDrag(float x, float y) {
    return false;
}

void curvePiece::keepDragging(float x, float y) {

}

void curvePiece::dragRelease(float x, float y) {

}

