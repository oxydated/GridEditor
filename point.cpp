#include "point.h"

point::point(float x, float y):
    posX(x),
    posY(y)
{}

float point::getPosX(){
    return posX;
}
float point::getPosY(){
    return posY;
}

void point::draw(){
    DrawableManager::drawPoint(posX, posY);
}
