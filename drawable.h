#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include <vector>

using namespace std;

class DrawableItem
{
public:
    virtual void draw() = 0;
};
typedef std::shared_ptr<DrawableItem> DrawableItemPtr;

class DrawableManager
{
public:
    static void insertItem(DrawableItemPtr item);

    static void init();

    static void drawItems();

    static void drawPoint(float x, float y);
    static void drawLine(float sx, float sy, float ex, float ey);
    static void drawCurve(float sx, float sy, float ex, float ey);

private:
    static std::vector<DrawableItemPtr> itemsToDraw;
    static bool initialized;
    static std::vector<float> pointsToDraw;
    static std::vector<float> linesToDraw;
    static std::vector<float> curvesToDraw;

    static void drawSegment(float sx, float sy, float ex, float ey, bool isCurve);
};

#endif // DRAWABLE_H
