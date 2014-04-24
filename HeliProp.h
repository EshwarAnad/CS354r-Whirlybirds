#ifndef __heliprop__
#define __heliprop__

#include "Simulator.h"
#include <Ogre.h>

class Heli;

class HeliProp : public GameObject {
protected:
    void updateNode(Ogre::String);
    Heli* parent;

public:
    HeliProp(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real scale, 
        Ogre::Real m, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Heli* p,
        Ogre::String
        );
    virtual void updateTransform();
    virtual void update();
    void hit(CollisionContext&, int, bool);
    void spin(Ogre::Real);
    void setVisible(bool);
};

#endif