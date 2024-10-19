#include <glm/ext/vector_float2.hpp> // vec2
#include <glm/ext/vector_float3.hpp> // vec3
#include <glm/ext/matrix_float4x4.hpp> // mat4x4
#include <glm/ext/matrix_transform.hpp> // translate, rotate, scale, identity
#include "camera.h"
#include "shaders.h"
#include <GL/glew.h>
#include <cmath>

std::shared_ptr<camera> camera::current = nullptr;

float camera::currentX = 0;
float camera::currentY = 0;

camera::camera(const std::array<float, 3> &inpos, const std::array<float, 3> &intarget):
    pos(inpos[0], inpos[1], inpos[2]),
    target(intarget[0], intarget[1], intarget[2])
{}

void camera::panTiltZoom(float x, float y){
    float norm = std::sqrt(x * x + y * y);
    float ax = -y/norm;
    float ay = x/norm;

    glm::mat4 centerInTarget = glm::translate(glm::identity<glm::mat4>(), target);
    glm::mat4 panAroundTarget = glm::rotate(centerInTarget, float(norm * M_PI/2), glm::vec3(-ax, -ay, 0.0));
//    glm::mat4 TiltAroundTarget = glm::rotate(panAroundTarget, tilt, glm::vec3(0.0, 0.0, 1.0));
    glm::vec4 posTransformed = panAroundTarget * glm::vec4(pos, 1.0);
    pos = glm::vec3(posTransformed.x, posTransformed.y, posTransformed.z);
}

void camera::updateZoom(float dZoom){
    zFactor += dZoom * 0.001;
}

void camera::buildProjection(){
    glm::mat4 orthoMatrix = glm::ortho(-zFactor, zFactor, -zFactor, zFactor, -zFactor * 10, zFactor * 10);
    setProjMatrix(glm::value_ptr(orthoMatrix));
}

void camera::buildWorldMatrix(){
    glm::mat4 lookAtMatrix = glm::lookAt(pos, target, glm::vec3(0.0, 1.0, 0.0));
    setWorldMatrix(glm::value_ptr(lookAtMatrix));
}

void camera::onMouseDown(float x, float y){
    currentX = x;
    currentY = y;
}

void camera::onMouseMove(float x, float y){
    float dx = x - currentX;
    float dy = y - currentY;
    currentX = x;
    currentY = y;

    if(current){
        current->panTiltZoom(dx, dy);
    }
}

void camera::wheelEvent(float amount){
    if(current){
        current->updateZoom(amount);
    }
}

void camera::BuildViewMatrix(float w, float h){
    int width = w;
    int height = h;
    int max = height > width? height: width;
    int x = (width - max)/2;
    int y = (height - max)/2;

    glm::mat4 viewportMatrix(
                float(h)/float(w),  0.0,                0.0,    0.0,
                0.0,                1,                  0.0,    0.0,
                0.0,                0.0,                1.0,    0.0,
                0.0,                0.0,                0.0,    1.0
                );

    glViewport(0, 0, w, h);
    setViewMatrix(glm::value_ptr(viewportMatrix));
}

void camera::setCurrentCamera(std::shared_ptr<camera> newCurrent){
    current = newCurrent;
}
void camera::updateCurrentCamera(float w, float h){
    if(current){
        current->BuildViewMatrix(w, h);
        current->buildProjection();
        current->buildWorldMatrix();
    }
}

