#include <iostream>
#include "Target.h"

Target::Target(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim, 
    Ogre::Real boxX, 
    Ogre::Real boxY, 
    Ogre::Real boxZ, 
    Ogre::Real boxWidth, 
    Ogre::Real boxHeight, 
    Ogre::Real boxDepth,
    Ogre::Real radius
    )
: GameObject(nym, mgr, sim, .25, .1), radius(radius), boxWidth(boxWidth), boxHeight(boxHeight), boxDepth(boxDepth)
{
    Ogre::ManualObject* circle = mgr->createManualObject("Circle");
    circle->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    float accuracy = 30;
    const float thickness = radius/4;
    unsigned int index = 0;
     
    for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy) 
    {
        circle->position(radius * cos(theta),
                         0,
                         radius * sin(theta));
        circle->position(radius * cos(theta - Ogre::Math::PI / accuracy),
                         0,
                         radius * sin(theta - Ogre::Math::PI / accuracy));
        circle->position((radius - thickness) * cos(theta - Ogre::Math::PI / accuracy),
                         0,
                         (radius - thickness) * sin(theta - Ogre::Math::PI / accuracy));
        circle->position((radius - thickness) * cos(theta),
                         0,
                         (radius - thickness) * sin(theta));
        circle->quad(index, index + 1, index + 2, index + 3);
        index += 4;
    }
    circle->end();
    
    rootNode->attachObject(circle);

    shape = new btCylinderShape(btVector3(radius, 5.0, radius)); 

    wall = 0;
    movePlacement();
}

void Target::setPose(int wall, float xOffset, float yOffset) {
    float x, y, z, yaw = 0, pitch = 0;
    
    if (wall == 0 || wall == 1) {
        x = xOffset;
        y = (wall == 0) ? -boxHeight/2.0 : boxHeight/2.0;
        z = yOffset;
        if (wall == 1) { pitch = 180; };
    } else if (wall == 2 || wall == 3) {
        x = xOffset;
        y = yOffset;
        z = (wall == 2) ? boxDepth/2.0 : -boxDepth/2.0;
        pitch = (wall == 2) ? 270 : 90;
    } else if (wall == 4 || wall == 5) {
        x = (wall == 4) ? boxWidth/2.0 : -boxWidth/2.0;
        y = xOffset;
        z = yOffset;
        yaw = (wall == 4) ? 270 : 90;
        pitch = (wall == 4) ? 90 : 90;
    }

    y += boxHeight/2.0 + 10;

    rootNode->resetToInitialState();
    move(x, y, z);
    rotate(yaw, pitch, 0);
    updateTransform();
}

void Target::movePlacement(void) {
    float xOffset = 0, yOffset = 0;
    xOffset = rand()%50 - 25; 
    yOffset = rand()%50 - 25; 
    setPose((wall++)%6, xOffset, yOffset);
}
