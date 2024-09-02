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

float curvePiece::evaluateX(float a){
    float x0 = sPoint->getPosX();
    float v0x = sHandle->getVecX();
    float x3 = ePoint->getPosX();
    float v1x = eHandle->getVecX();
    return x0 + a*(v0x + a*(-2*v0x - v1x - 3*x0 + a*(v0x + v1x + 2*x0 - 2*x3) + 3*x3));
}
float curvePiece::evaluateY(float a){
    float y0 = sPoint->getPosY();
    float v0y = sHandle->getVecY();
    float y3 = ePoint->getPosY();
    float v1y = eHandle->getVecY();
    return y0 + a*(v0y + a*(-2*v0y - v1y - 3*y0 + a*(v0y + v1y + 2*y0 - 2*y3) + 3*y3));
}

void curvePiece::draw(){
    float a = 0;
    for(int i = 0; i < 100; i++){
        float b = a + 0.01;
        float sx = evaluateX(a);
        float sy = evaluateY(a);
        float ex = evaluateX(b);
        float ey = evaluateY(b);
        DrawableManager::drawLine(sx, sy, ex, ey);
        a = b;
    }
}

