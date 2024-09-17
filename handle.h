#ifndef HANDLE_H
#define HANDLE_H

#include "drawable.h"
#include "point.h"

class handle: public DrawableItem
{
public:
    handle(std::shared_ptr<point> p, float x, float y, bool isLastArg);

    handle(std::shared_ptr<point> p, std::shared_ptr<point> h, bool isLastArg);

    float getPosX();
    float getPosY();

    float getVecX();
    float getVecY();

    void draw() override;

    bool willDrag(float x, float y) override;

    void keepDragging(float x, float y) override;

    void dragRelease(float x, float y) override;

private:
    float vecX;
    float vecY;
    std::shared_ptr<point> ori;
    bool isLast;
    int segmentIndex {-1};
};

#endif // HANDLE_H
