//start of StudentWorld.h
#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "GameController.h"
#include <string>
#include <queue>
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
    void addActor(Actor* a){if (a != nullptr)
        m_squirt.push_back(a);}
    
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
    void annoyIceMan(int amount);
    
    void annoyProtester(int amount);
    
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
    
    void moveOneStepTowardIceMan(Protester* p){;}
    
    bool iceAt(int x, int y) const;
    
    void decBarrels(){--m_barrelsLeft;}
    
    const vector<Boulder*>& getBoulders() const { return m_boulder; }
    
    IceMan* getIceMan() const { return m_iceman; }
    const vector<RegularProtester*>& getRegProtester() const { return m_Rprotester; }
    const vector<HardcoreProtester*>& getHardProtester() const { return m_Hprotester; }
    
    void setLevel(int lvl) { level = lvl; }
    
    vector<Actor*> getActors() const
    {
        std::vector<Actor*> all;

            for (auto* b : m_boulder)
                all.push_back(b);
            for (auto* g : m_gold)
                all.push_back(g);
            for (auto* o : m_barrel)
                all.push_back(o);
            for (auto* s : m_sonar)
                all.push_back(s);
            for (auto* p : m_pool)
                all.push_back(p);
            for (auto* q : m_squirt)
                all.push_back(q);
            for (auto* r : m_Rprotester)
                all.push_back(r);
            for (auto* h : m_Hprotester)
                all.push_back(h);

            // Don't forget IceMan!
            if (m_iceman)
                all.push_back(m_iceman);

            return all;
    }
    
    int getCurrentGameLevel(){return level;}
    int getNumLivesLeft(){return m_livesLeft;}
    int getCurrentHealth(){return m_iceman->getHealth();}
    int getSquirtsLeftInSquirtGun(){return m_iceman->getWater();}
    int getPlayerGoldCount(){return m_iceman->getGold();}
    int getNumberOfBarrelsRemainingToBePickedUp(){return m_barrelsLeft;}
    int getPlayerSonarChargeCount(){return m_iceman->getSonar();}
    int getCurrentScore(){return getScore();}
    
private:
    IceMan* m_iceman;
    int m_barrelsLeft;
    int m_livesLeft = 3;
    int level = 0;
    bool m_resetToLevelZero = false;
    int m_ticksSinceLevelStart = 0;
    bool m_protesterSpawned = false;
    int m_RegprotesterTimer = 0;
    int m_HardprotesterTimer = 0;
    int m_protesterToggle = 0; // 0 = Regular, 1 = Hardcore
    
    vector<RegularProtester*> m_Rprotester;
    vector<HardcoreProtester*> m_Hprotester;
    
    vector<Boulder*> m_boulder{};
    vector<GoldNugget*> m_gold{};
    vector<OilBarrel*> m_barrel{};
    vector<SonarKit*> m_sonar{};
    vector<WaterPool*> m_pool{};
    vector<Actor*> m_squirt{};
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

