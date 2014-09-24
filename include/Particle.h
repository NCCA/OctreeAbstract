#ifndef __PARTICLE__H_
#define __PARTICLE__H_

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/TransformStack.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include "Scene.h"

/// @file Particle.h
/// @brief Revised from Jon's ParticleFactory
/// @author Jonathan Macey, Xiaosong Yang
/// @version 1.0
/// @date 24/1/13
/// @class Particle
/// @brief this is the class for particles


// predeclare the scene manager - collision detection
class Scene;


class Particle
{
public:
    /// @brief constructor must be called by the child classes
    /// @param[in] _pos the position of the particle
    /// @param[in] _dir the direction of the particle
    /// @param[in] _c the colour of the particle
    /// @param[in] _shaderName the name of the shader to use
    /// @param[in] _parent the parent (Emitter used to query global values)
    Particle(ngl::Vec3 _pos, ngl::Vec3 _dir, ngl::Colour _c, GLfloat _r,  Scene *_parent);
    /// @brief  brief destructor

    ~Particle();

    /// @brief draw the sphere
    void draw(const ngl::Mat4 &_globalMouseTx) const;
    /// @brief update the sphere
    void update();

    /// @brief get current position
    inline ngl::Vec3 const getPosition(){return m_pos;}
    /// @brief get current speed
    inline ngl::Vec3 const getCurrentSpeed(){return m_dir;}
    /// @brief get radius
    inline GLfloat getRadius() const {return m_radius;}
    /// @brief set new position after collision
    inline void setPosition(ngl::Vec3 _newPos){m_pos = _newPos;}
    /// @brief set new speed after collision
    inline void setSpeed(ngl::Vec3 _newSpeed){m_dir = _newSpeed;}

protected:
    /// @brief  Position of the Particle
    ngl::Vec3 m_pos;
    /// @brief  Direction of the Particle
    ngl::Vec3 m_dir;
    /// @brief   Colour of the Particle
    ngl::Colour m_colour;
    /// @brief the parent Emitter
    Scene *m_parent;
    /// @brief the current radius for the sphere
    GLfloat m_radius;
};


#endif

