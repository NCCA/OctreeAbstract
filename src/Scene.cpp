#include "Scene.h"
#include "Particle.h"
#include <ngl/Random.h>
#include <boost/foreach.hpp>
#include <ngl/NGLStream.h>

Scene::Scene(ngl::Transformation *_t,  ngl::Camera *_cam)
{
    m_numParticles = 0;
    m_transform=_t;
    m_cam=_cam;
    BoundingBox bb;
    bb.m_minx = bb.m_miny = bb.m_minz = -10.0;
    bb.m_maxx = bb.m_maxy = bb.m_maxz = 10.0;
    collisionTree.reset( new ParticleOctree (4, bb)); // control the height of the octree
}

void Scene::addParticle(ngl::Vec3 _pos, ngl::Vec3 _dir, ngl::Colour _c, GLfloat _r)
{
  ngl::Random *rng=ngl::Random::instance();
  float r=rng->randomPositiveNumber(_r)+0.05;
  Particle *p = new Particle(_pos, _dir, _c, r, this);
  m_particles.push_back(p);
  ++m_numParticles;
}

Scene::~Scene()
{
    // we need to call the dtor for each of the particles as pointer
    // however the std::vector will clear it's self at the end
    BOOST_FOREACH(Particle *p, m_particles)
    {
        delete p;
    }
    deleteAllWalls();
}

void Scene::addNewWall(ngl::Vec3 _point, float _size, ngl::Vec3 _normal, bool _draw)
{
    Wall *newWall = new Wall;
    _normal.normalize();
    newWall->centre = _point;
    newWall->size = _size;
    newWall->a = _normal.m_x;
    newWall->b = _normal.m_y;
    newWall->c = _normal.m_z;
    newWall->d = -(newWall->a * _point.m_x + newWall->b * _point.m_y + newWall->c * _point.m_z);
    newWall->draw_flag = _draw;

    m_walls.push_back(newWall);
}

void Scene::deleteAllWalls()
{
  BOOST_FOREACH(Wall *w, m_walls)
  {
      delete w;
  }
  m_walls.clear();
}

ngl::Vec3 Scene::getRotationFromY(ngl::Vec3 _vec) const
{
    ngl::Vec3 rot;
    rot.m_z = 0.0;
    if(fabs(_vec.m_y)< 0.0001)
    {
        if (_vec.m_z>= 0.0)
            rot.m_x = -90;
        else
            rot.m_x = 90;
    }
    else
        rot.m_x = atan(_vec.m_z/_vec.m_y);
    if(fabs(_vec.m_y) + fabs(_vec.m_z) < 0.0001)
    {
        if(_vec.m_x > 0)
            rot.m_y = -90;
        else
            rot.m_y = 90;
    }
    else
        rot.m_z = atan(_vec.m_x/sqrt(_vec.m_y*_vec.m_y + _vec.m_z*_vec.m_z));

    return rot;
}

void Scene::draw(const ngl::Mat4 &_globalMouseTx) const
{
    BOOST_FOREACH(Particle *p, m_particles)
    {
        p->draw(_globalMouseTx);
    }
    // draw the walls
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["nglColourShader"]->use();
    BOOST_FOREACH(Wall *w, m_walls)
    {
        if(w->draw_flag)
        {
          m_transform->reset();
          {
              m_transform->setPosition(w->centre);
              m_transform->setScale(w->size, w->size, w->size);
              m_transform->setRotation(getRotationFromY(ngl::Vec3(w->a,w->b,w->c)));
              ngl::Mat4 MVP= m_transform->getMatrix()
                             *_globalMouseTx* m_cam->getVPMatrix();
              shader->setShaderParamFromMat4("MVP",MVP);
              prim->draw("wall");
          }
        }
    }
}

void Scene::update()
{
    // call the update method for each particle
    BOOST_FOREACH(Particle *p, m_particles)
    {
        p->update(); // update the position of each particles
    }
    collisionWithWalls();
    collisionWithBalls();
}

void Scene::collisionWithBalls()
{
    collisionTree->clearTree();

    BOOST_FOREACH(Particle *p, m_particles)
    {
        collisionTree->addObject(p);
    }
    collisionTree->checkCollision();
}

void Scene::collisionWithWalls()
{
    ngl::Vec3 oldP, oldV, newP, newV, wallNormal;
    float radius;
    float dist;
    // call the update method for each particle
    //for(std::vector<Particle *>::iterator itr = m_particles.begin(); itr!= m_particles.end(); ++itr)
    BOOST_FOREACH(Particle *p, m_particles)
    {
        oldP = p->getPosition();
        oldV = p->getCurrentSpeed();
        radius = p->getRadius();
        BOOST_FOREACH(Wall *w, m_walls)
        {
            wallNormal.m_x = w->a; wallNormal.m_y = w->b; wallNormal.m_z = w->c;
            dist = oldP.m_x * w->a + oldP.m_y * w->b + oldP.m_z * w->c + w->d - radius;
            if(dist < 0.0) // penerate the wall
            {
               newP = oldP - 2.0*dist*wallNormal;
               newV = -oldV.dot(wallNormal)*wallNormal+(oldV-oldV.dot(wallNormal)*wallNormal);
               p->setPosition(newP);
               p->setSpeed(newV);
            }
        }
    }
}

void Scene::clearParticles()
{
    // however the std::vector will clear it's self at the end
  BOOST_FOREACH(Particle *p, m_particles)
  {
      delete p;
  }
  // must remember to re-size the std::vector back to 0
  m_particles.clear();
  m_numParticles=0;
}
