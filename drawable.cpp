#include <GL/glew.h>
#include <GL/glxew.h>
#include <vector>
#include "drawable.h"

using namespace std;

std::vector<DrawableItemPtr> DrawableManager::itemsToDraw = std::vector<DrawableItemPtr>();
bool DrawableManager::initialized = false;

std::vector<float> DrawableManager::pointsToDraw = std::vector<float>();
std::vector<float> DrawableManager::linesToDraw = std::vector<float>();

unsigned int pointsArrayObj = -1;
unsigned int linesArrayObj = -1;

unsigned int pointsVertexBuffer = -1;
unsigned int pointsIndexBuffer = -1;

unsigned int linesVertexBuffer = -1;
unsigned int linesIndexBuffer = -1;

void DrawableManager::insertItem(DrawableItemPtr item){
    itemsToDraw.push_back(item);
}

void DrawableManager::init(){
    glPointSize(5);
    unsigned int vertexArrayObjID[2];
    glGenVertexArrays(2, &vertexArrayObjID[0]);
    pointsArrayObj = vertexArrayObjID[0];
    linesArrayObj = vertexArrayObjID[1];

    unsigned int vertexBufferObjID[4];
    glGenBuffers(4, vertexBufferObjID);
    pointsVertexBuffer = vertexBufferObjID[0];
    pointsIndexBuffer = vertexBufferObjID[1];
    linesVertexBuffer = vertexBufferObjID[2];
    linesIndexBuffer = vertexBufferObjID[3];
}

void DrawableManager::drawPoint(float x, float y){
    pointsToDraw.push_back(x);
    pointsToDraw.push_back(y);
    pointsToDraw.push_back(0.0);
}

void DrawableManager::drawLine(float sx, float sy, float ex, float ey){
    linesToDraw.push_back(sx);
    linesToDraw.push_back(sy);
    linesToDraw.push_back(0.0);
    linesToDraw.push_back(ex);
    linesToDraw.push_back(ey);
    linesToDraw.push_back(0.0);
}

void DrawableManager::drawItems(){
    for(auto &item : itemsToDraw){
        item->draw();
    }
    glBindVertexArray(pointsArrayObj);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, pointsToDraw.size() * sizeof(GLfloat), pointsToDraw.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
    glDrawArrays(GL_POINTS, 0, pointsToDraw.size()/3);

    glBindVertexArray(linesArrayObj);

    glBindBuffer(GL_ARRAY_BUFFER, linesVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, linesToDraw.size() * sizeof(GLfloat), linesToDraw.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttrib3f((GLuint)1, 0.0, 1.0, 1.0); // set constant color attribute
    glDrawArrays(GL_LINES, 0, linesToDraw.size()/6);
}
