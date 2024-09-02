#include <QMouseEvent>
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
//#include "vertices.h"

QCustomGLWidget::QCustomGLWidget(QWidget *parent)
    : QWidget{parent}
{

}

void QCustomGLWidget::initializeGL(){

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

    auto newPoint = std::make_shared<point>(0.75, 0.25);
    auto newHandle = std::make_shared<handle>(newPoint, -0.5, -0.5);

    auto newPoint2 = std::make_shared<point>(0.45, 0.75);
    auto newHandle2 = std::make_shared<handle>(newPoint2, 0.75, 0.25);

    auto newCurvePiece = std::make_shared<curvePiece>(newPoint, newHandle, newPoint2, newHandle2);

    DrawableManager::insertItem(newPoint);
    DrawableManager::insertItem(newPoint2);
    DrawableManager::insertItem(newHandle);
    DrawableManager::insertItem(newHandle2);
    DrawableManager::insertItem(newCurvePiece);

    auto newCurvePoint = std::make_shared<point>(0.45, 0.75);
    auto newCurveHandle = std::make_shared<handle>(newCurvePoint, 0.75, 0.25);

    auto newCurvePoint2 = std::make_shared<point>(-0.5, 0.25);
    auto newCurveHandle2 = std::make_shared<handle>(newCurvePoint2, -0.5, 0.5);

    auto newCurvePiece2 = std::make_shared<curvePiece>(newCurvePoint, newCurveHandle, newCurvePoint2, newCurveHandle2);

    DrawableManager::insertItem(newCurvePoint);
    DrawableManager::insertItem(newCurveHandle);
    DrawableManager::insertItem(newCurvePoint2);
    DrawableManager::insertItem(newCurveHandle2);
    DrawableManager::insertItem(newCurvePiece2);

    DrawableManager::init();
//    init();

    setShaders();


    QRect winRect = geometry();
    glViewport(0, 0, winRect.width(), winRect.height());

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

    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    displayVertices();
    DrawableManager::drawItems();

    glXSwapBuffers(m_display, m_win);
    glFlush();
    //QWidget::paintEvent(event);
}


void QCustomGLWidget::mouseReleaseEvent(QMouseEvent *event){
    qreal x = event->position().rx();
    qreal y = event->position().ry();

    QRect winRect = geometry();
    float px = 2.0*(x/float(winRect.width())) - 1.0;
    float py = -(2.0*(y/float(winRect.height())) - 1.0);

    auto newPoint = std::make_shared<point>(px, py);
    DrawableManager::insertItem(newPoint);

//    insertNewDot(px, py);
//    displayVertices();
    update();

    return;
}
