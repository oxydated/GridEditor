#include "handle.h"

handle::handle(std::shared_ptr<point> p, float x, float y, bool isLastArg):
    ori(p),
    isLast(isLastArg)
{
    if(isLastArg){
        float x3 = p->getPosX();
        float y3 = p->getPosY();
        float v1x = x;
        float v1y = y;

        float x2 = (-v1x + 3*x3)/3.;
        float y2 = (-v1y + 3*y3)/3.;
        vecX = x3 - x2;
        vecY = y3 - y2;

    }else{
        float x0 = p->getPosX();
        float y0 = p->getPosY();
        float v0x = x;
        float v0y = y;

        float x1 = (v0x + 3*x0)/3.;
        float y1 = (v0y + 3*y0)/3.;
        vecX = x0 - x1;
        vecY = y0 - y1;
    }
}

handle::handle(std::shared_ptr<point> p, std::shared_ptr<point> h, bool isLastArg):
    vecX(h->getPosX() - p->getPosX()),
    vecY(h->getPosY() - p->getPosY()),
    ori(p),
    isLast(isLastArg)
{}

float handle::getPosX(){
    return vecX + ori->getPosX();
}
float handle::getPosY(){
    return vecY + ori->getPosY();
}

float handle::getVecX(){
    return vecX;
}
float handle::getVecY(){
    return vecY;
}

void handle::draw(){
    DrawableManager::drawPoint(getPosX(), getPosY());
    DrawableManager::drawLine(ori->getPosX(), ori->getPosY(), getPosX(), getPosY());
}

bool handle::willDrag(float x, float y) {
    return false;
}

void handle::keepDragging(float x, float y) {

}

void handle::dragRelease(float x, float y) {

}
