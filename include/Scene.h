#ifndef __SCENE_H__
#define __SCENE_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <list>
#include "Particle.h"
#include "AbstractOctree.h"
#include "ParticleOctree.h"

/// @file Scene.h
/// @brief a simple Scene class, Revised from Jon's ParticleFactory
/// @author Jonathan Macey, Xiaosong Yang
/// @version 1.0
/// @date 24/01/13
/// @class Scene
/// @brief this class acts as an Scene for the particles, manage all particles including collision detection
class Particle;

typedef struct WALL{
    ngl::Vec3   centre;
    float   size;
    float   a;
    float   b;
    float   c;
    float   d;
    bool    draw_flag;
} Wall;

class Scene
{
  public :
    /// @brief ctor
    /// @param[in] _t a pointer to the global transform stack
    /// @param[in] _cam a pointer to the global camera
    Scene(ngl::Transformation *_t,  ngl::Camera *_cam);

    /// @dtor will explicitly call the Particle dtors as they are stored as pointers
    ~Scene();
    /// @brief update all the particles stored, if the particle died, delete it
    void update();
    /// @brief draw all the particles using the particle draw method
    void draw(const ngl::Mat4 &_globalMouseTx) const;
    /// @brief add a new particle to the end of the list
    /// @param[in] _pos position of the new particle
    /// @param[in] _dir speed of the particle
    /// @param[in] _c   the color of this sphere
    /// @param[in] _r   the radius of this particle sphere
    /// @param[in] _ml  the maximum life of this particle
    void addParticle(ngl::Vec3 _pos, ngl::Vec3 _dir, ngl::Colour _c, GLfloat _r);
    /// @brief accessor for the number of particles stored in the class
    /// @returns the number of particles
    inline unsigned int getNumParticles()const {return m_numParticles;}
    /// @brief get the global camera
    inline   ngl::Camera *getCamera()   {return m_cam;}
    /// @brief get the global transform stack
    inline ngl::Transformation *getTransform() const {return m_transform;}
    /// @brief a method to clear all particles
    void clearParticles();
    /// @brief add a new wall
    /// @param[in] _point center of the wall
    /// @param[in] _size size of the wall
    /// @param[in] _normal normal of the wall
    /// @param[in] _draw if the wall will be drew in the scene
    void addNewWall(ngl::Vec3 _point, float size, ngl::Vec3 _normal, bool _draw);
    /// @brief add a new wall
    void deleteAllWalls();
    /// @brief get rotation angle, rotate y axis to normal
    ngl::Vec3 getRotationFromY(ngl::Vec3 _vec) const;
    /// @brief collision detection between ball and walls
    void collisionWithWalls();
    /// @brief collision detection between balls
    void collisionWithBalls();
  private :
    /// @brief the particle list stores a pointer to each particle, these are created by the
    /// particle factory
    std::vector <Particle *> m_particles;
    /// @brief the global transform stack, used for global rotation only so could be replaced with
    /// a single matrix if required
    ngl::Transformation *m_transform;
    /// @brief a pointer to the current camera, this is used for accesing the VP matrix to load to the
    /// shader
    ngl::Camera *m_cam;
    /// @brief the number of particles
    unsigned int m_numParticles;
    /// @brief the wall list
    std::vector <Wall *> m_walls;
    /// @brief octree for collision detection
    std::unique_ptr<ParticleOctree>  collisionTree;
};


#endif
