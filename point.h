#ifndef POINT_H
#define POINT_H

#include "drawable.h"

class point: public DrawableItem
{
public:
    point(float x, float y);

    float getPosX();
    float getPosY();

    void draw() override;

    virtual bool willDrag(float x, float y) override;

    virtual void keepDragging(float x, float y) override;

    virtual void dragRelease(float x, float y) override;

private:
    float posX;
    float posY;
};

#endif // POINT_H
