#ifndef CURVEPIECE_H
#define CURVEPIECE_H

#include "drawable.h"
#include "point.h"
#include "handle.h"
#include <array>

class curvePiece : public DrawableItem
{
public:
    curvePiece(std::shared_ptr<point> sp, std::shared_ptr<handle> sh, std::shared_ptr<point> ep, std::shared_ptr<handle> eh );

    curvePiece(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

    curvePiece(std::shared_ptr<point> sp, float x1, float y1, float x2, float y2, std::shared_ptr<point> ep);

    float evaluateX(float a);
    float evaluateY(float a);

    void setToDraw();
    void setToDrag();

    void draw() override;
    bool willDrag(float x, float y) override;
    void keepDragging(float x, float y) override;
    void dragRelease(float x, float y) override;

    float v0x();
    float v0y();
    float v1x();
    float v1y();

    const std::shared_ptr<point> point0();
    const std::shared_ptr<point> point3();

private:
    std::shared_ptr<point> sPoint;
    std::shared_ptr<handle> sHandle;
    std::shared_ptr<point> ePoint;
    std::shared_ptr<handle> eHandle;
};

#endif // CURVEPIECE_H
