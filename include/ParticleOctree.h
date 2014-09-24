#ifndef PARTICLEOCTRRE_H__
#define PARTICLEOCTRRE_H__

#include "AbstractOctree.h"

class Particle;

class ParticleOctree : public AbstractOctree <Particle,ngl::Vec3>
{
  public :
     ParticleOctree(int _height, BoundingBox _limit)
       :
       AbstractOctree <Particle,ngl::Vec3> ( _height,  _limit)
     {}

     virtual void  checkCollisionOnNode(TreeNode <Particle> *node);



};

#endif

