#include <QMouseEvent>
#include <QGuiApplication>
#include "NGLScene.h"
#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include <ngl/NGLStream.h>

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT = 0.01f;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM = 0.1f;

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate = false;
  // mouse rotation values set to 0
  m_spinXFace = 0;
  m_spinYFace = 0;
  setTitle("Using the AbstractOctree Class");
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_project = ngl::perspective(45.0f, (float)_w / _h, 0.05f, 350.0f);
  m_width = _w * devicePixelRatio();
  m_height = _h * devicePixelRatio();
}

void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0, 0, 35);
  ngl::Vec3 to(0, 0, 0);
  ngl::Vec3 up(0, 1, 0);
  m_view = ngl::lookAt(from, to, up);
  ngl::ShaderLib::use("nglDiffuseShader");
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 0.0f, 1.0f);
  ngl::ShaderLib::setUniform("lightPos", 1.0f, 1.0f, 1.0f);
  ngl::ShaderLib::setUniform("lightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);

  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

  ngl::VAOPrimitives::createSphere("sphere", 1.0, 20);
  ngl::VAOPrimitives::createLineGrid("wall", 1, 1, 5);

  // create the default particle inital position
  m_scene = std::make_unique<Scene>(&m_transform, &m_view, &m_project);
  m_scene->addNewWall(ngl::Vec3(-10, 0, 0), 20, ngl::Vec3(1.0, 0.0, 0.0), true);
  m_scene->addNewWall(ngl::Vec3(10, 0, 0), 20, ngl::Vec3(-1.0, 0.0, 0.0), true);
  m_scene->addNewWall(ngl::Vec3(0, 10, 0), 20, ngl::Vec3(0.0, -1.0, 0.0), true);
  m_scene->addNewWall(ngl::Vec3(0, -10, 0), 20, ngl::Vec3(0.0, 1.0, 0.0), true);
  m_scene->addNewWall(ngl::Vec3(0, 0, 10), 20, ngl::Vec3(0.0, 0.0, -1.0), false);
  m_scene->addNewWall(ngl::Vec3(0, 0, -10), 20, ngl::Vec3(0.0, 0.0, 1.0), true);
  m_text = std::make_unique<ngl::Text>("fonts/Arial.ttf", 14);

  currentTime = currentTime.currentTime();

  glViewport(0, 0, width(), height());
  m_text->setScreenSize(width(), height());

  startTimer(50);
  m_updateTimer = startTimer(20);
  m_animate = true;
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_width, m_height);
  m_text->setScreenSize(width(), height());

  // calculate the framerate
  QTime newTime = currentTime.currentTime();
  int msecsPassed = currentTime.msecsTo(newTime);
  currentTime = newTime;

  // clear the screen and depth buffer
  ngl::ShaderLib::use("nglDiffuseShader");

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  auto rotX = ngl::Mat4::rotateX(m_spinXFace);
  auto rotY = ngl::Mat4::rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX = rotY * rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
  m_scene->draw(m_mouseGlobalTX);
  m_text->setColour(1, 1, 1);
  m_text->renderText(10, 700, fmt::format("number of particles {}", m_scene->getNumParticles()));
  m_text->renderText(10, 680, "1 add Particle");
  m_text->renderText(10, 660, fmt::format("framerate is {}", (1000.0f / msecsPassed)));
  m_text->renderText(10, 640, "Space clears all particles");
}

void NGLScene::addParticles()
{
  m_scene->clearParticles();
  // initial box [-1.0, 1.0]
  // evenly create 1000 particles inside the box
  ngl::Vec3 dir(0.0f, 0.0f, 0.0f);
  ngl::Vec3 emitterPos;

  for (float i = -9.5; i < 10; i += 1)
  {
    emitterPos.m_x = i;
    for (float j = -9.5; j < 10; j += 1)
    {
      emitterPos.m_y = j;
      for (float k = -9.5; k < 10; k += 1)
      {
        emitterPos.m_z = k;
        dir = ngl::Random::getRandomVec3();
        dir *= 0.05f;
        m_scene->addParticle(emitterPos, dir, ngl::Random::getRandomNormalizedVec3(), 0.3f); // emitterPos, initialSpeed, colour, radius,
      }
    }
  }
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  if (m_animate == false)
    return;
  // for(unsigned int i=0;i<5;i++)
  if (_event->timerId() == m_updateTimer)
    m_scene->update();
  else
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent(QMouseEvent *_event)
{
// note the method buttons() is the button state when event was called
// this is different from button() which is used to check which button was
// pressed when the mousePress/Release event is generated
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif
  if (m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx = position.x() - m_origX;
    int diffy = position.y() - m_origY;
    m_spinXFace += (float)0.5f * diffy;
    m_spinYFace += (float)0.5f * diffx;
    m_origX = position.x();
    m_origY = position.y();
    update();
  }
  // right mouse translate code
  else if (m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(position.x() - m_origXPos);
    int diffY = (int)(position.y() - m_origYPos);
    m_origXPos = position.x();
    m_origYPos = position.y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent(QMouseEvent *_event)
{
// this method is called when the mouse button is pressed in this case we
// store the value where the maouse was clicked (x,y) and set the Rotate flag to true
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif
  if (_event->button() == Qt::LeftButton)
  {
    m_origX = position.x();
    m_origY = position.y();
    m_rotate = true;
  }
  // right mouse translate mode
  else if (_event->button() == Qt::RightButton)
  {
    m_origXPos = position.x();
    m_origYPos = position.y();
    m_translate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent(QMouseEvent *_event)
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate = false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

  // check the diff of the wheel position (0 means no change)
  if (_event->angleDelta().x() > 0)
  {
    m_modelPos.m_z += ZOOM;
  }
  else if (_event->angleDelta().x() < 0)
  {
    m_modelPos.m_z -= ZOOM;
  }
  update();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape:
    QGuiApplication::exit(EXIT_SUCCESS);
    break;
  // turn on wirframe rendering
  case Qt::Key_W:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  // turn off wire frame
  case Qt::Key_S:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  // show full screen
  case Qt::Key_F:
    showFullScreen();
    break;
  // show windowed
  case Qt::Key_N:
    showNormal();
    break;
  case Qt::Key_Space:
    addParticles();
    break;
  default:
    break;
  }
  // finally update the GLWindow and re-draw
  // if (isExposed())
  update();
}
