#include "GUI.h"

GUI::GUI(CEGUI::Event::Subscriber* spSub, CEGUI::Event::Subscriber* clientSub, CEGUI::Event::Subscriber* serverSub)
{
	// Initialize CEGUI
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	// Set CEGUI resource groups
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	//Set the CEGUI skin
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	// create window manager and sheet
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *menuSheet = wmgr.createWindow("DefaultWindow", "GUI/Sheet");

	// create buttons
    p1score = wmgr.createWindow("TaharezLook/Button", "UI/Score1");
	p2score = wmgr.createWindow("TaharezLook/Button", "UI/Score2");
	spBTN = wmgr.createWindow("TaharezLook/Button", "Menu/SingleButton");
	clientBTN = wmgr.createWindow("TaharezLook/Button", "Menu/ClientButton");
	serverBTN = wmgr.createWindow("TaharezLook/Button", "Menu/SeverButton");
	
	//bounds message
	gameMessage = wmgr.createWindow("TaharezLook/Button", "Message/gameMessage");
	gameMessage->setVisible(false);
	gameMessage->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
	gameMessage->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25,0), CEGUI::UDim(0.25, 0)));

	// set buttons sizes and locations
	spBTN->setText("TUTORIAL");
	spBTN->setSize(CEGUI::UVector2(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.05, 0)));
	spBTN->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0), CEGUI::UDim(0.3, 0)));
	clientBTN->setText("CLIENT");
	clientBTN->setSize(CEGUI::UVector2(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.05, 0)));
	clientBTN->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0), CEGUI::UDim(0.4, 0)));
	serverBTN->setText("SERVER");
	serverBTN->setSize(CEGUI::UVector2(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.05, 0)));
	serverBTN->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0), CEGUI::UDim(0.5, 0)));
    p1score->setText("PLAYER 1");
    p1score->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	p2score->setText("PLAYER 2");
	p2score->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	p2score->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85,0), CEGUI::UDim(0, 0)));	


    // find the hostname of this machine
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);

	// create text fields for IP addresses and ports
	serverIPBox = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","SIP"));
	serverIPBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	serverIPBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.32,0), CEGUI::UDim(0.65,0)));
	serverIPBox->setText(hostname);
	serverPortBox = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","SPORT"));
	serverPortBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	serverPortBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.52,0), CEGUI::UDim(0.65,0)));
	serverPortBox->setText("55554");

	spBTN->subscribeEvent(CEGUI::PushButton::EventClicked, spSub);
	clientBTN->subscribeEvent(CEGUI::PushButton::EventClicked, clientSub);
	serverBTN->subscribeEvent(CEGUI::PushButton::EventClicked, serverSub);


	// add buttons to sheet
	menuSheet->addChildWindow(spBTN);
	menuSheet->addChildWindow(clientBTN);
	menuSheet->addChildWindow(serverBTN);
	menuSheet->addChildWindow(p1score);
	menuSheet->addChildWindow(p2score);
	menuSheet->addChildWindow(serverIPBox);
	menuSheet->addChildWindow(serverPortBox);

	//add gameMessage to sheet
	menuSheet->addChildWindow(gameMessage);

    CEGUI::System::getSingleton().setGUISheet(menuSheet);
}

char* GUI::getIP(void)
{
	serverIP = CEGUIStringToString(serverIPBox->getText());
	return serverIP;
}

int GUI::getPort(void)
{
	serverPort = atoi(CEGUIStringToString(serverPortBox->getText()));
	return serverPort;
}

bool GUI::makeSP(const CEGUI::EventArgs &e)
{
	isSP = true;
	return true;
}

bool GUI::makeClient(const CEGUI::EventArgs &e)
{
	isClient = true;
	return true;
}

bool GUI::makeServer(const CEGUI::EventArgs &e)
{
	isServer = true;
	return true;
}

void GUI::destroyMenu(bool sp)
{
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	if (sp)
		wmgr.destroyWindow(p2score);
	wmgr.destroyWindow(spBTN);
	wmgr.destroyWindow(clientBTN);
	wmgr.destroyWindow(serverBTN);
	wmgr.destroyWindow(serverIPBox);
	wmgr.destroyWindow(serverPortBox);
	CEGUI::MouseCursor::getSingleton().hide();
}

char* GUI::CEGUIStringToString(CEGUI::String cestr)
{
	char * newstr = (char *) malloc((cestr.size()+1) * sizeof(char));
	memset(newstr,0xFF,cestr.size()+1);
	for(int i = 0; i < cestr.size(); i++) {
		newstr[i] &= cestr[i];
	}
	newstr[cestr.size()] = 0;
	return newstr;
}

void GUI::setSinglePlayerScore(int score) {
	std::ostringstream stream;
	stream << "score: " << score;
	p1score->setText(stream.str());
}

void GUI::setMultiplayerScores(int player1, int player2) {
	std::ostringstream stream;
	stream << "server score: " << player1;
	p1score->setText(stream.str());
	stream.str("");
	stream << "client score: " << player2;
	p2score->setText(stream.str());
}

void GUI::setGameMessage(std::ostringstream& stream){
	gameMessage->setText(stream.str());
}

void GUI::setGameMessageVisible(bool vis){
	gameMessage->setVisible(vis);
}
