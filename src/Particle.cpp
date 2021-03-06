#include "Particle.h"

Particle::Particle(ngl::Vec3 _pos, ngl::Vec3 _dir, ngl::Vec3 _c, GLfloat _r, Scene *_parent)
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

    ngl::ShaderLib::use("nglDiffuseShader");
    ngl::ShaderLib::setUniform("Colour",ngl::Vec4(m_colour,1.0f));
    ngl::Transformation t;
    t.setPosition(m_pos);
    t.setScale(m_radius,m_radius,m_radius);
    ngl::Mat4 MVP=m_parent->getProject()*m_parent->getView()*
                  _globalMouseTx *
                  t.getMatrix();

    normalMatrix=m_parent->getView()*
                 _globalMouseTx*
                 t.getMatrix();

    normalMatrix.inverse().transpose();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::ShaderLib::setUniform("normalMatrix",normalMatrix);


    // get an instance of the VBO primitives for drawing
    ngl::VAOPrimitives::draw("sphere");

}


void Particle::update()
{
    // more the particle by adding the Direction vector to the position
    m_pos+=m_dir;
}
