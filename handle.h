#ifndef HANDLE_H
#define HANDLE_H

#include "drawable.h"
#include "point.h"

class handle: public DrawableItem
{
public:
    handle(std::shared_ptr<point> p, float x, float y, bool isLastArg);

    float getPosX();
    float getPosY();

    float getVecX();
    float getVecY();

    void draw() override;

private:
    float vecX;
    float vecY;
    std::shared_ptr<point> ori;
    bool isLast;
};

#endif // HANDLE_H
