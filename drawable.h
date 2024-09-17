#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include <vector>
#include "draggable.h"

using namespace std;

class DrawableItem: public std::enable_shared_from_this<DrawableItem>, public Draggable
{
public:
    virtual void draw() = 0;
    void setIndex(int argIndex);

protected:
    int index {-1};
    bool firstDraw {true};
};
typedef std::shared_ptr<DrawableItem> DrawableItemPtr;

class DrawableManager
{
public:
    static void insertItem(DrawableItemPtr item);

    static void init();
    static void Update();
    static void drawItems();

    static int drawPoint(float x, float y, int offset = -1);
    static int drawLine(float sx, float sy, float ex, float ey, int offset = -1);
    static int drawCurve(float sx, float sy, float ex, float ey, int offset = -1);

    static void setMinDistance(float d);
    static float getMinDistance();

    static void setToUpdate();



private:
    static std::vector<DrawableItemPtr> itemsToDraw;
    static bool initialized;
    static std::vector<float> pointsToDraw;
    static std::vector<float> linesToDraw;
    static std::vector<float> curvesToDraw;

    static int drawSegment(float sx, float sy, float ex, float ey, bool isCurve, int offset = -1);

    static float minDistance;
    static bool willUpdate;
};

#endif // DRAWABLE_H
