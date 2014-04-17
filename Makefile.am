noinst_HEADERS = Whirlybirds.h Ball.h BaseApplication.h GameObject.h GUI.h Simulator.h OgreMotionState.h Box.h ContactSensorCallback.h Surface.h SoundSystem.h Wall.h Target.h UDPNetEnt.h Server.h Client.h ServerToClient.h Heli.h HeliProp.h HeliChass.h Level.h Building.h ClientToServer.h Game.h


bin_PROGRAMS = whirlybirds
whirlybirds_CPPFLAGS = -I$(top_srcdir)
whirlybirds_SOURCES = Whirlybirds.cpp Ball.cpp BaseApplication.cpp GameObject.cpp GUI.cpp Simulator.cpp OgreMotionState.cpp Box.cpp Surface.cpp SoundSystem.cpp Target.cpp Wall.cpp Heli.cpp HeliProp.cpp HeliChass.cpp Level.cpp Building.cpp Server.cpp UDPNetEnt.cpp

 
whirlybirds_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(sdl_CFLAGS) $(SDLMIXER_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(SDLNET_CFLAGS)
whirlybirds_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(sdl_LIBS) $(SDLMIXER_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDLNET_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
