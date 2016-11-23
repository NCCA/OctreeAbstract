#ifndef PARTICLEOCTTREE_H_
#define PARTICLEOCTTREE_H_

#include "AbstractOctree.h"

class Particle;

class ParticleOctree : public AbstractOctree <Particle,ngl::Vec3>
{
  public :
     ParticleOctree(int _height, BoundingBox _limit)
       :
       AbstractOctree <Particle,ngl::Vec3> ( _height,  _limit)
     {}

     virtual void  checkCollisionOnNode(std::shared_ptr<TreeNode<Particle> > node);



};

#endif

