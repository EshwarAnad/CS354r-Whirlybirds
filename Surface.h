#ifndef __Surface_h_
#define __Surface_h_

#include "GameObject.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Simulator.h"
#include <Ogre.h>

class Surface : public GameObject {
	public:
		Surface(Ogre::String, 
			Ogre::SceneManager*, 
			Simulator*, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Real, 
			Ogre::Real,
			Ogre::Real,
			Ogre::Real,
			Ogre::String);

        void update();
};

#endif
