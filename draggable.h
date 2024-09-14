#ifndef DRAGGABLE_H
#define DRAGGABLE_H

#include <memory>
#include <vector>

class Draggable
{
public:
    virtual bool willDrag(float x, float y) = 0;

    virtual void keepDragging(float x, float y) = 0;

    virtual void dragRelease(float x, float y) = 0;
};

typedef std::shared_ptr<Draggable> DraggablePointer;

class DragManager
{
public:
    static void insertDraggable(DraggablePointer newDraggable);

    static void onMouseDown(float x, float y);
    static void onMouseMove(float x, float y);
    static void onMouseUp(float x, float y);

private:
    static DraggablePointer currentDraggable;
    static std::vector<DraggablePointer> draggables;

};


#endif // DRAGGABLE_H
