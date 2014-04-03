#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Ball.h"
#include "Target.h"


Ball::Ball(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    Ogre::Real radius, 
    Ogre::Real m, 
    Ogre::Vector3 pos, 
    Ogre::Real restitution, 
    Ogre::Real friction,
    Ogre::String tex = ""
    ) 
: GameObject(nym, mgr, sim, restitution, friction)
{
	nym = nym;
    if (mgr) {
        geom = mgr->createEntity(name, "sphere.mesh");
        if(tex != "")
            geom->setMaterialName(tex);
        geom->setCastShadows(false);
        rootNode->attachObject(geom);

        // sphere starts at 100 units radius
        rootNode->scale(
            radius * 0.01f,
            radius * 0.01f,
            radius * 0.01f
            );

        rootNode->setPosition(pos);
    } else {
        // updateTransform aint gonna work so we have to set the transform ourselves
        tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    }

    shape = new btSphereShape(radius);
    mass = m;
    hitFlag = false;
    score = 0;
}

void Ball::update() {
    if (callback->ctxt.hit) {
        Ogre::String& objName = callback->ctxt.theObject->name;

        if (objName == "mytarget") {
			simulator->soundPlayed = BALLTARGET;
            if (simulator->soundOn) {
                simulator->soundSystem->playTargetHit();
            }
            score++;
            Target* target = static_cast<Target*>(callback->ctxt.theObject);
            target->movePlacement();
        }
        else if (objName == "clientpaddle") {
			simulator->soundPlayed = CLIENTBALLPADDLE;
            if (simulator->soundOn) {
                simulator->soundSystem->playP2Hit();
            }
		}
		else if (objName == "serverpaddle") {
			simulator->soundPlayed = SERVERBALLPADDLE;
			if (simulator->soundOn) {
				simulator->soundSystem->playRaquetHit();
        	}
		}
        else if (isBox(objName)){
			simulator->soundPlayed = (this->nym == "serverball") ? BALL1WALL : BALL2WALL;
            if (simulator->soundOn) {
				int b = (this->nym == "serverball") ? 1 : 2;
                simulator->soundSystem->playWallHit(b);
            }
        }
    }
    else {
        //nothing was hit
    }
}

int& Ball::getScore(){
    return score;
}
