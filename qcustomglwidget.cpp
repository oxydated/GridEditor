#include <QMouseEvent>
#include <QApplication>
#include <QSize>
#include "qcustomglwidget.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glxew.h>
#include <glm/vec3.hpp>
#include "shaders.h"
#include "drawable.h"
#include "point.h"
#include "handle.h"
#include "curvepiece.h"
#include "curvebuilder.h"
#include "mathtools.h"
#include "camera.h"
#include "memory"

QCustomGLWidget::QCustomGLWidget(QWidget *parent)
    : QWidget{parent}
{

}

void QCustomGLWidget::initializeGL(){
//    auto cRoots = cubicRoots(-1, 0.25, -3, 0.75);

//    auto fRoots = fourthRoots(1, -1.65, 0.9125, -0.196875, 0.0140625);
//    auto fRoots = fourthRoots(1, -1.25, 0.375, 0, 0);

    auto fifRoots = fifthRoots(-262.25, 800., -915., 485.175, -116.75, 9.775);

    auto nearest = nearestPointInCurve(1, -2, 1.5, -3, 2, -2, 0, -1, 1.1, -1.9);

    float length = bezierLength(1, -2, 1.5, -3, 2, -2, 0, -1);

    float lengtha = bezierIntervalLength(1, -2, 1.5, -3, 2, -2, 0, -1, 0.0, 0.5);

    float lengthb = bezierIntervalLength(1, -2, 1.5, -3, 2, -2, 0, -1, 0.5, 0.75);

    float lengthc = bezierIntervalLength(1, -2, 1.5, -3, 2, -2, 0, -1, 0.75, 1.0);

    float lengthS = lengtha + lengthb + lengthc;

    float tenthLength = lengthS / 10.0;

    float tenthlengthTimes5 = tenthLength * 5.0;

    float tenthParameter = bezierParameterForLength(1, -2, 1.5, -3, 2, -2, 0, -1, tenthlengthTimes5);

    float tenthLenghtAgain = bezierIntervalLength(1, -2, 1.5, -3, 2, -2, 0, -1, 0.0, tenthParameter);


    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);

    m_display = XOpenDisplay(nullptr);
    if(m_display == NULL) {
        fprintf(stderr, "Cannot connet to X server\n");
        exit(1);
    }

    int glx_major, glx_minor;

    if(!glXQueryVersion(m_display, &glx_major, &glx_minor) ||
            ((glx_major == 1) && (glx_minor < 3)) ||(glx_major < 1)){
        fprintf(stderr, "Invalid GLX version\n");
        exit(1);
    }

    m_win = winId();

    XWindowAttributes winAttrib;

    if(!XGetWindowAttributes(m_display, m_win, &winAttrib)){
        fprintf(stderr, "Cannot get windows attributes\n");
        XCloseDisplay(m_display);
        exit(1);
    }

    Screen *screen = winAttrib.screen;

    int attributes[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        None
    };

    XVisualInfo *visualInfoTemplate = glXChooseVisual(m_display, XScreenNumberOfScreen(screen), attributes);


    visualInfoTemplate->visualid = XVisualIDFromVisual(winAttrib.visual);


    int visualInfoCount;

    XVisualInfo *visualInfo = XGetVisualInfo(m_display, VisualIDMask, visualInfoTemplate, &visualInfoCount);
    if(visualInfo == NULL || visualInfoCount == 0){
        fprintf(stderr, "No matching visual found\n");
        XCloseDisplay(m_display);
        exit(1);
    }

    XMapWindow(m_display, m_win);

    m_glc = glXCreateContext(m_display, visualInfo, NULL, GL_TRUE);
    if(m_glc == NULL){
        fprintf(stderr, "Unable to create GLX context\n");
        XFree(visualInfo);
        XCloseDisplay(m_display);
        exit(1);
    }

    if(!glXMakeCurrent(m_display, m_win, m_glc)){
        fprintf(stderr, "Unable to make GLX context current\n");
        glXDestroyContext(m_display, m_glc);
        XFree(visualInfo);
        XCloseDisplay(m_display);
        exit(1);
    }

    GLenum err = glewInit();
    if(GLEW_OK != err){
        fprintf(stderr, "glew failed\n");
        exit(1);
    }

    if(!glxewIsSupported("GLX_ARB_create_context")){
        fprintf(stderr, "GLX_ARB_create_context not supported\n");
        exit(1);
    }

    if(!glewIsSupported("GL_VERSION_4_6")){
        fprintf(stderr, "OpenGL 4.6 not supported\n");
        exit(1);
    }

    int nElements = 0;
    GLXFBConfig* fbConfigs = glXChooseFBConfig(m_display, XScreenNumberOfScreen(screen), attributes, &nElements );

    int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        None
    };

    GLXContext glCtx = glXCreateContextAttribsARB(m_display, fbConfigs[191], 0, True, context_attribs);
    if(glCtx == NULL){
        fprintf(stderr, "Unable to create GLX 4.6 context\n");
        glXMakeCurrent(m_display, None, nullptr);
        glXDestroyContext(m_display, m_glc);
        XFree(visualInfo);
        XCloseDisplay(m_display);
        exit(1);
    }


    if(!glXMakeCurrent(m_display, m_win, glCtx)){
        fprintf(stderr, "Unable to make GLX 4.6 context current\n");
        glXDestroyContext(m_display, glCtx);
        glXMakeCurrent(m_display, None, nullptr);
        glXDestroyContext(m_display, m_glc);
        XFree(visualInfo);
        XCloseDisplay(m_display);
        exit(1);
    }
    glXDestroyContext(m_display, m_glc);
    m_glc = glCtx;

//    float x0 = 0.75;
//    float y0 = 0.25;

//    float x3 = 0.95;
//    float y3 = 0.05;

//    float vx1 = (x3 - x0);
//    float vy1 = (y3 - y0);

//    float vx2 = vx1;
//    float vy2 = vy1;

//    auto newPoint = std::make_shared<point>(x0, y0);

//    auto newPoint2 = std::make_shared<point>(x3, y3);

//    float x1 = (-vx1 + 3*x0)/3.;
//    float y1 = (-vy1 + 3*y0)/3.;
//    float x2 = (vx2 + 3*x3)/3.;
//    float y2 = (vy2 + 3*y3)/3.;

//    auto newpiece2 = std::make_shared<curvePiece>(newPoint, x1, y1, x2, y2, newPoint2);

//    newpiece2->draw();

    DrawableManager::init();

    setShaders();
    //updateViewport();
    std::array<float, 3> camPos({0.0, 0.0, 1.0});
    std::array<float, 3> camTarget({0.0, 0.0, 0.0});
    camera::setCurrentCamera(std::make_shared<camera>(camPos, camTarget));
    QRect winRect = geometry();
    int width = winRect.width();
    int height = winRect.height();
    camera::updateCurrentCamera(float(width), float(height));

    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glXSwapBuffers(m_display, m_win);
    glFlush();
}


void QCustomGLWidget::showEvent(QShowEvent* event){
    QWidget::showEvent(event);
    initializeGL();
}

void QCustomGLWidget::closeEvent(QCloseEvent *event){
    destroyShaders();
    glXMakeCurrent(m_display, None, nullptr);
    glXDestroyContext(m_display, m_glc);
    QWidget::closeEvent(event);
}

void QCustomGLWidget::paintEvent(QPaintEvent *event){    
    updateViewport();
    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
for(int i = 0; i < 2; i++){
    DrawableManager::Update();
    DrawableManager::drawItems();
}
    glXSwapBuffers(m_display, m_win);
    glFinish();
}

void QCustomGLWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() == Qt::RightButton){
        auto pointXY = convertCoordinates(event);

        auto modification = QGuiApplication::keyboardModifiers();
        if(modification == Qt::ShiftModifier){
            camera::onMouseMove(pointXY[0], pointXY[1]);
        }else
        {
            DragManager::onMouseMove(pointXY[0], pointXY[1]);
        }
        update();
    }
}

void QCustomGLWidget::wheelEvent(QWheelEvent *event){
    camera::wheelEvent(event->angleDelta().y());
    update();
}

void QCustomGLWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::RightButton){
        auto pointXY = convertCoordinates(event);

        auto modification = QGuiApplication::keyboardModifiers();
        if(modification == Qt::ShiftModifier){
            camera::onMouseDown(pointXY[0], pointXY[1]);
        }else
        {
            DragManager::onMouseDown(pointXY[0], pointXY[1]);
        }
        update();
    }
}

void QCustomGLWidget::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        auto pointXY = convertCoordinates(event);

        if(CurveBuilder::willBuildCurve()){
            CurveBuilder::onClick(pointXY[0], pointXY[1], this);
            update();
        }
    }

    if(event->button() == Qt::RightButton){
        auto pointXY = convertCoordinates(event);
        DragManager::onMouseUp(pointXY[0], pointXY[1]);
        update();
    }

    return;
}

void QCustomGLWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        CurveBuilder::cancelBuildCurve();
    }
}

void QCustomGLWidget::resizeEvent(QResizeEvent *event){
    updateViewport();
}

void QCustomGLWidget::updateViewport(){
    QRect winRect = geometry();
    int width = winRect.width();
    int height = winRect.height();    
    camera::updateCurrentCamera(float(width), float(height));

    auto ori = convertCoordinates(0., 0.);
    auto dest = convertCoordinates(0., 10.);
    DrawableManager::setMinDistance(std::sqrt(std::pow(dest[0] - ori[0],2) + std::pow(dest[1] - ori[1],2)));
}

std::array<float, 2> QCustomGLWidget::convertCoordinates(float xw, float yw){
    QRect winRect = geometry();
    int width = winRect.width();
    int height = winRect.height();

    float px = 2.0*(xw - (float(width)/2.0 - float(height)/2.0))/float(height) - 1.0;
    float py = 2.0 *(float(height) - yw)/float(height) - 1.0;

    return {px, py};
}

std::array<float, 2> QCustomGLWidget::convertCoordinates(QMouseEvent *event){
    qreal x = event->position().rx();
    qreal y = event->position().ry();
    return convertCoordinates(x, y);
}
