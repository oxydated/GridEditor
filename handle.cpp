#include "handle.h"

handle::handle(std::shared_ptr<point> p, float x, float y, bool isLastArg):
    vecX(x),
    vecY(y),
    ori(p),
    isLast(isLastArg)
{}

float handle::getPosX(){
    return (isLast? -vecX : vecX) + ori->getPosX();
}
float handle::getPosY(){
    return (isLast? -vecY : vecY) + ori->getPosY();
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
