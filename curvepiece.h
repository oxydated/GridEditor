#ifndef CURVEPIECE_H
#define CURVEPIECE_H

#include "drawable.h"
#include "point.h"
#include "handle.h"

class curvePiece : public DrawableItem
{
public:
    curvePiece(std::shared_ptr<point> sp, std::shared_ptr<handle> sh, std::shared_ptr<point> ep, std::shared_ptr<handle> eh );

    float evaluateX(float a);
    float evaluateY(float a);

    void draw() override;

private:
    std::shared_ptr<point> sPoint;
    std::shared_ptr<handle> sHandle;
    std::shared_ptr<point> ePoint;
    std::shared_ptr<handle> eHandle;
};

#endif // CURVEPIECE_H
