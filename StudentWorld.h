//start of StudentWorld.h
#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "GameController.h"
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
 
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {;}
    
    virtual ~StudentWorld()
    {
        cleanUp();
    }
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    // Add an actor to the world.
    void addActor(Actor* a);
    
    // Clear a 4x4 region of Ice. &&&
    bool clearIce(int x, int y);
    
    // Can actor move to x,y?, is there a boulder, or protestor?
    bool canActorMoveTo(Actor* a, int x, int y) const;
    
    // Annoy all other actors within radius of annoyer, returning the
    // number of actors annoyed.
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);
    
    // Reveal all objects within radius of x,y.
    void revealAllNearbyObjects(int x, int y, int radius);
    
    // If the IceMan is within radius of a, return a pointer to the
    // IceMan, otherwise null.
    Actor* findNearbyIceMan(Actor* a, int radius) const;
    
    // If at least one actor that can pick things up is within radius of a,
    // return a pointer to one of them, otherwise null.
    Actor* findNearbyPickerUpper(Actor* a, int radius) const;
    
    // Annoy the IceMan.
    void annoyIceMan();
    
    // Give IceMan some sonar charges.
    void giveIceManSonar();
    
    // Give IceMan some water.
    void giveIceManWater();
    
    // Is the Actor a facing toward the IceMan?
    bool facingTowardIceMan(Actor* a) const;
    
    // If the Actor a has a clear line of sight to the IceMan, return
    // the direction to the IceMan, otherwise GraphObject::none.
    GraphObject::Direction lineOfSightToIceMan(Actor* a) const;
    
    // Return whether the Actor a is within radius of IceMan.
    bool isNearIceMan(Actor* a, int radius) const{return false;}
    
    // Determine the direction of the first move a quitting protester
    // makes to leave the oil field.
    GraphObject::Direction determineFirstMoveToExit(int x, int y);
    
    // Determine the direction of the first move a hardcore protester
    // makes to approach the IceMan.
    GraphObject::Direction determineFirstMoveToIceMan(int x, int y);
    
    bool iceAt(int x, int y) const;
    
    void decBarrels(){--m_barrelsLeft;}
    
    int getCurrentGameLevel(){return getLevel();}
    int getNumLivesLeft(){return getLives();}
    int getCurrentHealth(){return 100;}//fix
    int getSquirtsLeftInSquirtGun(){return m_iceman->getWater();}
    int getPlayerGoldCount(){return m_iceman->getGold();}
    int getNumberOfBarrelsRemainingToBePickedUp(){return m_barrelsLeft;}
    int getPlayerSonarChargeCount(){return m_iceman->getSonar();}
    int getCurrentScore(){return getScore();}
    
private:
    IceMan* m_iceman;
    int m_barrelsLeft;
    int level = 0;
    
    vector<RegularProtester*> m_Rprotester;
    vector<HardcoreProtester*> m_Hprotester;
    
    vector<Boulder*> m_boulder{};
    vector<GoldNugget*> m_gold{};
    vector<OilBarrel*> m_barrel{};
    vector<SonarKit*> m_sonar{};
    vector<WaterPool*> m_pool{};
    int nIce;
    Ice* IcePointers[64][60];
    
    int m_sonarTimer = 0;
    int m_poolTimer = 0;
    bool m_sonarActive;
    
    int m_itemTimer;
    int m_toggleState;
    
    char ObjPositions[64][64];
};

#endif //STUDENTWORLD_H
//end of StudentWorld.h


