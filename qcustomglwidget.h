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
//glXCreateContextAttribsARBProc glXCreateContextAttribsARB;

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

    void initializeGL();

private:
    Display* m_display;

    Window m_win;
    GLXContext m_glc;

};

#endif // QCUSTOMGLWIDGET_H
