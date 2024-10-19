#ifndef CAMERA_H
#define CAMERA_H

#include <array>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <memory>

using namespace std;

class camera {
public:
    camera(const std::array<float, 3> &pos, const std::array<float, 3> &target);

    void panTiltZoom(float x, float y);

    void updateZoom(float dZoom);

    void buildProjection();
    void buildWorldMatrix();
    void BuildViewMatrix(float w, float h);

    static void onMouseDown(float x, float y);
    static void onMouseMove(float x, float y);
    static void wheelEvent(float amount);

    static void setCurrentCamera(std::shared_ptr<camera> newCurrent);
    static void updateCurrentCamera(float w, float h);

private:
    glm::vec3 pos;
    glm::vec3 target;
    float zFactor {1.0};
    static std::shared_ptr<camera> current;

    static float currentX;
    static float currentY;
};

#endif // CAMERA_H
