#include "point.h"
#include <cmath>

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

bool point::willDrag(float x, float y) {

    float dist = std::sqrt(std::pow(getPosX() - x,2) + std::pow(getPosY() - y,2));

    return dist < DrawableManager::getMinDistance();
}

void point::keepDragging(float x, float y) {
    posX = x;
    posY = y;
    DrawableManager::setToUpdate();
}

void point::dragRelease(float x, float y) {

}
