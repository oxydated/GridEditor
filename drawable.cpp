#include <GL/glew.h>
#include <GL/glxew.h>
#include <vector>
#include "drawable.h"

using namespace std;

std::vector<DrawableItemPtr> DrawableManager::itemsToDraw = std::vector<DrawableItemPtr>();
bool DrawableManager::initialized = false;

std::vector<float> DrawableManager::pointsToDraw = std::vector<float>();
std::vector<float> DrawableManager::linesToDraw = std::vector<float>();
std::vector<float> DrawableManager::curvesToDraw = std::vector<float>();

float DrawableManager::minDistance = 0.0;

bool DrawableManager::willUpdate = false;

unsigned int pointsArrayObj = -1;
unsigned int linesArrayObj = -1;
unsigned int curvesArrayObj = -1;

unsigned int pointsVertexBuffer = -1;
unsigned int pointsIndexBuffer = -1;

unsigned int linesVertexBuffer = -1;
unsigned int linesIndexBuffer = -1;

unsigned int curvesVertexBuffer = -1;
unsigned int curvesIndexBuffer = -1;


void DrawableItem::setIndex(int argIndex){
    index = argIndex;
}

void DrawableManager::insertItem(DrawableItemPtr item){
    itemsToDraw.push_back(item);
    setToUpdate();
}

void DrawableManager::init(){
    glPointSize(5);
    unsigned int vertexArrayObjID[3];
    glGenVertexArrays(3, &vertexArrayObjID[0]);
    pointsArrayObj = vertexArrayObjID[0];
    linesArrayObj = vertexArrayObjID[1];
    curvesArrayObj = vertexArrayObjID[2];

    unsigned int vertexBufferObjID[6];
    glGenBuffers(6, vertexBufferObjID);
    pointsVertexBuffer = vertexBufferObjID[0];
    pointsIndexBuffer = vertexBufferObjID[1];
    linesVertexBuffer = vertexBufferObjID[2];
    linesIndexBuffer = vertexBufferObjID[3];
    curvesVertexBuffer = vertexBufferObjID[4];
    curvesIndexBuffer = vertexBufferObjID[5];
}

int DrawableManager::drawPoint(float x, float y, int offset){
    if(offset == -1){
        int newOffSet = pointsToDraw.size();
        pointsToDraw.push_back(x);
        pointsToDraw.push_back(y);
        pointsToDraw.push_back(0.0);
        return newOffSet;
    } else {
        pointsToDraw[offset] = x;
        pointsToDraw[offset + 1] = y;
        pointsToDraw[offset + 2] = 0.0;
        return offset;
    }
}

int DrawableManager::drawLine(float sx, float sy, float ex, float ey, int offset){
    return drawSegment(sx, sy, ex, ey, false, offset);
}

int DrawableManager::drawCurve(float sx, float sy, float ex, float ey, int offset){
    return drawSegment(sx, sy, ex, ey, true, offset);
}

int DrawableManager::drawSegment(float sx, float sy, float ex, float ey, bool isCurve, int offset){
    auto &segmentToDraw = isCurve ? curvesToDraw : linesToDraw;
    if(offset == -1){
        int newOffSet = segmentToDraw.size();
        segmentToDraw.push_back(sx);
        segmentToDraw.push_back(sy);
        segmentToDraw.push_back(0.0);
        segmentToDraw.push_back(ex);
        segmentToDraw.push_back(ey);
        segmentToDraw.push_back(0.0);
        return newOffSet;
    } else {
        segmentToDraw[offset] = sx;
        segmentToDraw[offset + 1] = sy;
        segmentToDraw[offset + 2] = 0.0;
        segmentToDraw[offset + 3] = ex;
        segmentToDraw[offset + 4] = ey;
        segmentToDraw[offset + 5] = 0.0;
        return offset;
    }
}

void DrawableManager::setMinDistance(float d){
    minDistance = d;
}
float DrawableManager::getMinDistance(){
    return minDistance;
}

void DrawableManager::setToUpdate(){
    willUpdate = true;
}

void DrawableManager::Update(){
    if(!willUpdate){
        return;
    }

    for(auto &item : itemsToDraw){
        item->draw();
    }

    glBindVertexArray(pointsArrayObj);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, pointsToDraw.size() * sizeof(GLfloat), pointsToDraw.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(linesArrayObj);

    glBindBuffer(GL_ARRAY_BUFFER, linesVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, linesToDraw.size() * sizeof(GLfloat), linesToDraw.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(curvesArrayObj);

    glBindBuffer(GL_ARRAY_BUFFER, curvesVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, curvesToDraw.size() * sizeof(GLfloat), curvesToDraw.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glFinish();
    willUpdate = false;
}

void DrawableManager::drawItems(){

    glBindVertexArray(pointsArrayObj);

    glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
    glDrawArrays(GL_POINTS, 0, pointsToDraw.size()/3);

    glBindVertexArray(linesArrayObj);    
    glLineWidth(1.0);

    glVertexAttrib3f((GLuint)1, 0.0, 1.0, 1.0); // set constant color attribute
    glDrawArrays(GL_LINES, 0, linesToDraw.size()/3);

    glBindVertexArray(curvesArrayObj);
    glLineWidth(6.0);

    glVertexAttrib3f((GLuint)1, 1.0, 1.0, 0.0); // set constant color attribute
    glDrawArrays(GL_LINES, 0, curvesToDraw.size()/3);
}
