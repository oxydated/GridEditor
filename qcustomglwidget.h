#ifndef QCUSTOMGLWIDGET_H
#define QCUSTOMGLWIDGET_H

#include <QWidget>
#include <QDebug>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glew.h>
#include <GL/glxew.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

class QCustomGLWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCustomGLWidget(QWidget *parent = nullptr);

signals:

protected:
    virtual void showEvent(QShowEvent* event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void initializeGL();

private:
    void zoom(float x, float y);
    void updateViewport();
    std::array<float, 2> convertCoordinates(float x, float y);

    Display* m_display;

    Window m_win;
    GLXContext m_glc;

    float screenCenterX;
    float screenCenterY;
    float zoomFactor;

};

#endif // QCUSTOMGLWIDGET_H
