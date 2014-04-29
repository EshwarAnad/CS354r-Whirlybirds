#ifndef __GUI_h_
#define __GUI_h_

#include <stdlib.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

class GUI
{
	protected:
		CEGUI::OgreRenderer* mRenderer;
		CEGUI::Window *p1score;
		CEGUI::Window *p2score;
		CEGUI::Editbox *serverIPBox;
		CEGUI::Window *spBTN;
		CEGUI::Window *clientBTN;
		CEGUI::Window *serverBTN;
		CEGUI::Editbox *serverPortBox;

		CEGUI::Window *gameMessage;

		char* serverIP;
		int serverPort;

	public:
		bool isSP;
		bool isClient;
		bool isServer;

		GUI(CEGUI::Event::Subscriber*, CEGUI::Event::Subscriber*, CEGUI::Event::Subscriber*);
		char* getIP(void);
		int getPort(void);
		bool makeSP(const CEGUI::EventArgs &e);
		bool makeClient(const CEGUI::EventArgs &e);
		bool makeServer(const CEGUI::EventArgs &e);
		void destroyMenu(bool);
		char* CEGUIStringToString(CEGUI::String cestr); 
		void setSinglePlayerScore(int score);
		void setMultiplayerScores(int player1, int player2);
		void setGameMessage(std::ostringstream&);
		void setGameMessageVisible(bool);
		
};

#endif
