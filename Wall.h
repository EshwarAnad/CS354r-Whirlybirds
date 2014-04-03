#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Simulator.h"
#include <Ogre.h>

class Wall : public GameObject {
	public:
		Wall(Ogre::String, 
			Ogre::SceneManager*, 
			Simulator*, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Vector3,
			Ogre::Vector3,
			Ogre::Real,
			Ogre::Real,
			Ogre::String
			);
        void update();
};

#endif
