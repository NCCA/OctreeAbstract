#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Camera.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include "Scene.h"
#include "Particle.h"
#include "WindowParams.h"
#include <QTime>
#include <QOpenGLWindow>
#include <memory>

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the method is called upon window resize
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();
    /// @brief a method to add more particles to the system
    void addParticles();
    inline void clearParticles(){m_scene->clearParticles(); }

    void updateEmitterPos(float _dx, float _dy, float _dz);
    void updateEmitterDir(float _d_Theta, float _d_Phi);
    inline void toggleAnimation() { m_animate ^=true;}

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the windows params such as mouse and rotations etc
    //----------------------------------------------------------------------------------------------------------------------
    WinParams m_win;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the global mouse transforms
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_mouseGlobalTX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Camera m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief transformation stack for the gl transformations etc
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation m_transform;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a container for our particles
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<Scene> m_scene;
    /// @brief our font
    std::unique_ptr<ngl::Text> m_text;
    /// @brief calculate framerate
    QTime currentTime;
    int m_updateTimer;
    bool m_animate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event);
    /// @brief timer event trigered by startTimer
    void timerEvent(QTimerEvent *_event);


};



#endif
