//start of Actor.h
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class StudentWorld;

 class Actor : public GraphObject
 {
 public:
     Actor(StudentWorld* world, int startX, int startY, Direction startDir,
           bool visible, int imageID, double size, int depth): GraphObject(imageID, startX, startY, startDir, size, depth),
     m_world(world), m_alive(true)
{
   setVisible(visible);
}
     virtual void doSomething() = 0;
     
       // Action to perform each tick.
     virtual void move() = 0;

       // Is this actor alive?
     bool isAlive() const{return m_alive;}
     
       // Mark this actor as dead.
     void setDead(){m_alive=false;}

       // Annoy this actor.
     virtual bool annoy(unsigned int amt){return false;}
     
       // Get this actor's world
     StudentWorld* getWorld() const{return m_world;}
     
       // Can other actors pass through this actor?
     virtual bool canActorsPassThroughMe() const {return 0;}
     
       // Can this actor dig through Ice?
     virtual bool canDigThroughIce() const {return false;}
     
       // Can this actor pick items up?
     virtual bool canPickThingsUp() const {return false;}
     
       // Does this actor hunt the IceMan?
     virtual bool huntsIceMan() const {return false;}
     
       // Can this actor need to be picked up to finish the level?
     virtual bool needsToBePickedUpToFinishLevel() const {return false;}

       // Move this actor to x,y if possible, and return true; otherwise,
       // return false without moving.
     bool moveToIfPossible(int x, int y){return false;}
     
     private:
        StudentWorld* m_world;
        bool m_alive;
 };

 class Agent : public Actor
 {
 public:
     Agent(StudentWorld* world, int startX, int startY, Direction startDir,
           int imageID, unsigned int hitPoints):Actor(world, startX, startY, startDir, true, imageID, 1.0, 3.0){;}
     
     virtual void move(){;}
     
       // Pick up a gold nugget.
     virtual void addGold(){;}
     
       // How many hit points does this actor have left?
     unsigned int getHitPoints() const{return 0;}

     virtual bool annoy(unsigned int amount){return false;}
     virtual bool canPickThingsUp() const{return false;}
 };

class Ice : public Actor{
public:
    Ice(int startX, int startY, StudentWorld* sw): Actor(sw, startX, startY, right, true, IID_ICE, 0.25, 3)
    {
        setVisible(true);
    }
    
    virtual ~Ice()
    {;}
    
    virtual void doSomething() {;}
    virtual void move(){;}
};

class Boulder : public Actor {
public:
    
    Boulder(int startX, int startY, StudentWorld* sw)
        : Actor(sw, startX, startY, right, true, IID_BOULDER, 1.0, 0), m_state(STABLE), m_waitTicks(0)
    {
        setVisible(isVisible());
    }
    
    virtual ~Boulder() {;}
    
    virtual void doSomething();
    
    virtual void move(){}
    virtual bool canActorsPassThroughMe() const{return false;}
    
    private:
    enum State { STABLE, WAITING, FALLING };
    State m_state;
    int m_waitTicks;
};

 class Squirt : public Actor
 {
 public:
     Squirt(StudentWorld* world, int startX, int startY, Direction startDir):Actor(world, startX, startY, right, true, IID_WATER_SPURT, 1.0, 2){;}
     
     virtual ~Squirt(){;}
     
     virtual void move();
     virtual void doSomething();
 };
 
 class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,int soundToPlay, bool activateOnPlayer,
                     bool activateOnProtester, bool initiallyActive): Actor(world, startX, startY, right, true, imageID, 1.0, 2){;}
    
    virtual void move(){;}
   // virtual void addBarrels(){;}
   // virtual unsigned int getBarrels() const {return 0;}
    
    // Set number of ticks until this object dies
    void setTicksToLive(){;}
};


class IceMan : public Agent {
public:
    
    IceMan(int startX, int startY, StudentWorld* sw): Agent(sw, 30, 60, right,IID_PLAYER, 10)
    {;}
    
    virtual ~IceMan() {;}
    
    virtual void doSomething();
    virtual void giveUp(){}
    
    virtual void move(){}
    virtual bool annoy(unsigned int amount){return false;}
    virtual bool canDigThroughIce() const{return true;}
    virtual bool canPickThingsUp() const override { return true; }

      // Pick up a sonar kit.
    virtual void addSonar(){m_sonar++;}

      // Pick up water.
    void addWater(){
        m_water += 5;
        //m_water++;
    }

    //increment gold
    void addGold() override { m_gold++; }
    
    // Get amount of gold
    unsigned int getGold() const {return m_gold;}
    
      // Get amount of sonar charges
    unsigned int getSonar() const{return m_sonar;}

      // Get amount of water
    unsigned int getWater() const{return m_water;}
    
    virtual void addBarrels(){m_barrels++;}
    
    
    //virtual unsigned int getBarrels() const {return m_barrels;}
    
private:
    unsigned int m_gold;
    unsigned int m_barrels;
    unsigned int m_sonar;
    unsigned int m_water;
};


class Protester : public Agent
{
public:
    Protester(StudentWorld* sw, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score):Agent(sw, startX, startY, left, imageID, hitPoints){}
    virtual void move();
    virtual bool annoy(unsigned int amount){return false;}
    virtual void addGold(){}
    virtual bool huntsIceMan() const{return false;}

      // Set state to having gien up protest
    void setMustLeaveOilField();

      // Set number of ticks until next move
    void setTicksToNextMove();
};

class OilBarrel : public ActivatingObject
{
public:
    OilBarrel(int startX, int startY, StudentWorld* sw, bool isVisible, bool canPick):ActivatingObject(sw, startX, startY, IID_BARREL, SOUND_FOUND_OIL, false, true, false)
    {
        setVisible(!isVisible);
    }
    virtual ~OilBarrel() {;}
    virtual void doSomething();
    virtual void move(){;}
    virtual bool needsToBePickedUpToFinishLevel() const{return true;}
};

class GoldNugget:public ActivatingObject
{
public:
    GoldNugget(int startX, int startY, StudentWorld* sw, bool isVisible, bool canPick): ActivatingObject(sw, startX, startY, IID_GOLD, SOUND_PROTESTER_FOUND_GOLD, false, true, false)
    {
        setVisible(!isVisible);
    }
    virtual ~GoldNugget() {;}
    virtual void doSomething();
    virtual void move(){;}
};

class SonarKit : public ActivatingObject
{
public:
    SonarKit(int startX, int startY, StudentWorld* sw, bool isVisible, bool canPick): ActivatingObject(sw, startX, startY, IID_SONAR, SOUND_GOT_GOODIE, false, true, false), m_ticksToLive(0)
    {
        setVisible(isVisible);
    }
    virtual ~SonarKit() {;}
    virtual void doSomething();
    virtual void move(){;}
    
private:
    int m_ticksToLive = 0;
};

class WaterPool : public ActivatingObject
{
public:
    WaterPool(int startX, int startY, StudentWorld* sw, bool isVisible, bool canPick): ActivatingObject(sw, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE, false, true, false), m_ticksToLive(0)
    {
        setVisible(isVisible);
    }
    virtual void doSomething();
    virtual ~WaterPool() {;}
    virtual void move(){;}
    
private:
    int m_ticksToLive = 0;
};

class RegularProtester : public Protester {
public:
    
    RegularProtester(int startX, int startY, StudentWorld* sw): Protester(sw, startX, startY, IID_PROTESTER, 5, 0){;}
    
    virtual ~RegularProtester() {;}
    
    virtual void move();
    virtual void addGold(){}
    
    virtual void doSomething();
    virtual void giveUp(){}
};

class HardcoreProtester : public Protester {
public:
    
    HardcoreProtester(int startX, int startY, StudentWorld* sw): Protester(sw, startX, startY, IID_HARD_CORE_PROTESTER, 20, 0){;}
    
    virtual ~HardcoreProtester() {;}
    
    virtual void move();
    virtual void addGold(){}
    
    virtual void doSomething();
    virtual void giveUp(){}
};

#endif //ACTOR_H
//end of Actor.h


