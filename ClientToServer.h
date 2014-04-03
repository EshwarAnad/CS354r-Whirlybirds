class ClientToServerData {
public:
    Ogre::Vector3 paddlePos;
    Ogre::Quaternion paddleOrient;
    int gravityChange;

    ClientToServerData();
};

ClientToServerData::ClientToServerData()
: gravityChange(0) 
{}
    
