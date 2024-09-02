#include "handle.h"

handle::handle(std::shared_ptr<point> p, float x, float y):
    vecX(x),
    vecY(y),
    ori(p)
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
