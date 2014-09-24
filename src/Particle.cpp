#include "Particle.h"

Particle::Particle(ngl::Vec3 _pos, ngl::Vec3 _dir, ngl::Colour _c, GLfloat _r, Scene *_parent)
{
    // set the particles position on direction
    m_pos=_pos;
    m_dir=_dir;
    m_colour=_c;

    m_radius=_r;
    m_parent=_parent;
}

Particle::~Particle()
{
    //std::cout<<"particle  dtor\n";
}

void Particle::draw(const ngl::Mat4 &_globalMouseTx) const
{
  ngl::Mat3 normalMatrix;

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["nglDiffuseShader"]->use();
    shader->setShaderParam4f("Colour",m_colour.m_r,m_colour.m_g,m_colour.m_b,m_colour.m_a);
    ngl::Transformation t;
    t.setPosition(m_pos);
    t.setScale(m_radius,m_radius,m_radius);
    ngl::Mat4 MVP=t.getMatrix() * _globalMouseTx * m_parent->getCamera()->getVPMatrix();
    normalMatrix=t.getMatrix()* _globalMouseTx*m_parent->getCamera()->getViewMatrix();
    normalMatrix.inverse();
    shader->setRegisteredUniform("MVP",MVP);
    shader->setRegisteredUniform("normalMatrix",normalMatrix);


    // get an instance of the VBO primitives for drawing
    ngl::VAOPrimitives::instance()->draw("sphere");

}


void Particle::update()
{
    // more the particle by adding the Direction vector to the position
    m_pos+=m_dir;
}
