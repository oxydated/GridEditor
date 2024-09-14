#include "draggable.h"

DraggablePointer DragManager::currentDraggable = nullptr;

std::vector<DraggablePointer> DragManager::draggables;


void DragManager::insertDraggable(DraggablePointer newDraggable){
    draggables.push_back(newDraggable);
}

void DragManager::onMouseDown(float x, float y){
    if(draggables.empty()){
        return;
    }
    for(auto &dragObject : draggables){
        if((dragObject)->willDrag(x, y)){
            currentDraggable = dragObject;
            break;
        }
    }
}

void DragManager::onMouseMove(float x, float y){
    if(currentDraggable){
        currentDraggable->keepDragging(x, y);
    }
}

void DragManager::onMouseUp(float x, float y){
    if(currentDraggable){
        currentDraggable->dragRelease(x, y);
    }
    currentDraggable = nullptr;
}

