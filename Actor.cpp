#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void IceMan::doSomething(){ //during a tick
    int ch;
    
    if (getWorld() -> getKey(ch) == true)
    {
        switch(ch)
        {
            case KEY_PRESS_UP:
            case 'w':
                if(getY() + 1 < 61)
                {
                    setDirection(up);
                    
                    if(getWorld()->canActorMoveTo(this, getX(), getY()+1))
                    {
                        moveTo(getX(), getY() + 1);
                        if (getWorld()->clearIce(getX(), getY()))
                            getWorld()->playSound(SOUND_DIG);
                    }
                }
                break;
            case KEY_PRESS_DOWN:
            case 's':
                if(getY() - 1 >= 0)
                {
                    setDirection(down);
                    
                    if(getWorld()->canActorMoveTo(this, getX(), getY()-1))
                    {
                        moveTo(getX(), getY() - 1);
                        if (getWorld()->clearIce(getX(), getY()))
                            getWorld()->playSound(SOUND_DIG);
                    }
                }
                break;
            case KEY_PRESS_LEFT:
            case 'a':
                if(getX() - 1 >= 0)
                {
                    setDirection(left);
                    
                    if(getWorld()->canActorMoveTo(this, getX()-1, getY()))
                    {
                        moveTo(getX()-1, getY());
                        if (getWorld()->clearIce(getX(), getY()))
                            getWorld()->playSound(SOUND_DIG);
                    }
                }
                break;
            case KEY_PRESS_RIGHT:
            case 'd':
                if(getX() + 1 < 64)
                {
                    setDirection(right);
                    
                    if(getWorld()->canActorMoveTo(this, getX()+1, getY()))
                    {
                        moveTo(getX() + 1, getY());
                        if (getWorld()->clearIce(getX(), getY()))
                            getWorld()->playSound(SOUND_DIG);
                    }
                }
                break;
            case KEY_PRESS_SPACE:
                if (getWater() > 0) {
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    m_water--; // reduce water by 1
                    
                    int newX = getX();
                    int newY = getY();
                    
                    switch (getDirection()) {
                        case up:    newY += 4;
                            setDirection(up);
                            break;
                        case down:
                            setDirection(down);
                            newY -= 4; break;
                        case left:  newX -= 4;
                            setDirection(left);
                            break;
                        case right:
                            setDirection(right);
                            newX += 4; break;
                        default: break;
                    }
                    
                    if (newX >= 0 && newX < 64 && newY >= 0 && newY < 60) {
                        bool blocked = false;
                        
                        for (int i = 0; i < 4 && !blocked; ++i) {
                            for (int j = 0; j < 4 && !blocked; ++j) {
                                if (getWorld()->iceAt(newX + i, newY + j)) {
                                    blocked = true;
                                }
                            }
                        }
                        
                        for (auto b : getWorld()->getBoulders()) {
                            if (b && b->isAlive()) {
                                int dx = b->getX() - newX;
                                int dy = b->getY() - newY;
                                if (sqrt(dx*dx + dy*dy) <= 3.0) {
                                    blocked = true;
                                    break;
                                }
                            }
                        }
                        
                        // if not blocked, spawn a new Squirt
                        if (!blocked) {
                            Squirt* s = new Squirt(getWorld(), newX, newY, getDirection());
                            getWorld()->addActor(s);
                        }
                    }
                }
                break;
            case 'z':
                if (getSonar() > 0) {
                    getWorld()->revealAllNearbyObjects(12, getX(), getY());
                    m_sonar--;
                    getWorld()->playSound(SOUND_SONAR);
                }
                break;
                
            case KEY_PRESS_TAB:
                if (getGold() > 0) {
                    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
                    m_gold--; // reduce gold by 1
                    
                    int newX = getX();
                    int newY = getY();
                    
                    switch (getDirection()) {
                        case up:    newY += 4;
                            setDirection(up);
                            break;
                        case down:
                            setDirection(down);
                            newY -= 4; break;
                        case left:  newX -= 4;
                            setDirection(left);
                            break;
                        case right:
                            setDirection(right);
                            newX += 4; break;
                        default: break;
                    }
                    
                    if (newX >= 0 && newX < 64 && newY >= 0 && newY < 60) {
                        bool blocked = false;
                        
                        for (int i = 0; i < 4 && !blocked; ++i) {
                            for (int j = 0; j < 4 && !blocked; ++j) {
                                if (getWorld()->iceAt(newX + i, newY + j)) {
                                    blocked = true;
                                }
                            }
                        }
                        
                        for (auto b : getWorld()->getBoulders()) {
                            if (b && b->isAlive()) {
                                int dx = b->getX() - newX;
                                int dy = b->getY() - newY;
                                if (sqrt(dx*dx + dy*dy) <= 3.0) {
                                    blocked = true;
                                    break;
                                }
                            }
                        }
                        
                        // if not blocked, spawn a new Squirt
                        if (!blocked) {
                            GoldNugget* g = new GoldNugget(newX, newY, getWorld(), true, true, true);
                            getWorld()->addActor(g);
                        }
                    }
                }
                break;
                
        }
    }
}

void IceMan::annoy(int amount) {
    
    // limit and subtract
    m_health = std::max(0, int(m_health) - amount);
    
    if (m_health == 0) {              // IceMan is dead
        setDead();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    } else {                          // still alive
        getWorld()->playSound(SOUND_PLAYER_ANNOYED);
    }
}

void Boulder::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
    
    int x = getX();
    int y = getY();
    
    
    switch (m_state)
    {
        case STABLE:
        {
            // Check the 4 squares below for ice
            bool iceBelow = false;
            for (int i = 0; i < 4; ++i)
            {
                if (getWorld()->iceAt(x + i, y - 1)) {
                    iceBelow = true;
                    break;
                }
            }
            
            if (!iceBelow)
            {
                m_state = WAITING;
                m_waitTicks = 0;
                
            }
            break;
        }
            
        case WAITING:
        {
            m_waitTicks++;
            if (m_waitTicks >= 5)
            {
                m_state = FALLING;
                getWorld()->playSound(SOUND_FALLING_ROCK);
            }
            break;
        }
            
        case FALLING:
        {
           if (y - 1 < 0) {
                
                setDead();
                setVisible(false);
                return;
            }
            if (y - 1 < 0 || !getWorld()->canActorMoveTo(this, x, y - 1) || getWorld()->iceAt(x, y - 1)) {
                   return;
            }
               // move down one unit
               moveTo(x, y - 1);
               y--;
            
            // check if iceman is within 3 units
             IceMan* iceman = getWorld()->getIceMan();
             if (iceman && iceman->isAlive()) {
                 int ix = iceman->getX();
                 int iy = iceman->getY();

                 // overlapping area
                 if (x + 3 >= ix && x <= ix + 3 &&
                     y - 1 + 3 >= iy && y - 1 <= iy + 3) {
                     getWorld()->annoyIceMan(100); // kill iceman
                 }
             }
          
            for (Protester* p : getWorld()->getRegProtester())
                  if (p && p->isAlive() &&
                      inRadius(p->getX(), p->getY(), getX(), getY(), 3.0)){
                      p->annoy(100); // dead
                      p->setDead();
                  }

              for (Protester* p : getWorld()->getHardProtester())
                  if (p && p->isAlive() &&
                      inRadius(p->getX(), p->getY(), getX(), getY(), 3.0)) {
                      p->annoy(100);
                      p->setDead();
                  }
            
            // check for ice beneath
            bool iceBelow = false;
            for (int i = 0; i < 4; ++i)
            {
                if (getWorld()->iceAt(x + i, y - 1)) {
                    iceBelow = true;
                    break;
                }
            }
            if (iceBelow) {
                m_state = STABLE;
                return;
            }
               break;
        }
    }
}

void Squirt::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
    
    int x = getX();
    int y = getY();

    switch (getDirection()) {
        case up:    setDirection(up); y++; break;
        case down:  setDirection(down); y--; break;
        case left:  setDirection(left); x--; break;
        case right: setDirection(right); x++; break;
        default: break;
    }

    // check bounds
    if (x < 0 || x >= 64 || y < 0 || y >= 60 || !getWorld()->canActorMoveTo(this, x, y)) {
        setDead();
        return;
    }

    moveTo(x, y);
    m_ticksToLive++;

    if (m_ticksToLive >= 4) {  // dies after 4 moves, not 5 ticks
        setDead();
        return;
    }
    
    for (Actor* actor : getWorld()->getActors())      
    {
        // ignore dead things
        if (!actor->isAlive()) continue;

        // protester that’s close enough to the squirt?
        Protester* p = dynamic_cast<Protester*>(actor);
        if (p && inRadius(p->getX(), p->getY(), getX(), getY(), 4.0))
        {
            // regular loses 5 HP, hardcore 15 HP
            p->annoy(5);

            setDead();           // the squirt disappears after hitting
            return;              // one squirt only hurts once
        }
    }
}


void GoldNugget::doSomething()
{
    if (!isAlive()) return;
    
    Actor* a = getWorld()->findNearbyPickerUpper(this, 3);
    Actor* radius = getWorld()->findNearbyPickerUpper(this, 12);
    
    if (IceMan* im = dynamic_cast<IceMan*>(a))
    {
        if (wasDropped())
            return;                       // ignore IceMan’s own bribe
        
        im->addGold();
        getWorld()->increaseScore(10);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
        return;
    }
    else if (Protester* p = dynamic_cast<Protester*>(a))      // protester standing on nugget
    {
        p->setStunned(50);
        p->receiveBribe();
        setDead();
        return;
    }
    
    if (radius != nullptr && radius->canPickThingsUp())
        setVisible(true);
}

void OilBarrel::doSomething()
{
   if (!isAlive()) return;

    Actor* picker = getWorld()->findNearbyPickerUpper(this, 3);
    Actor* radius = getWorld()->findNearbyPickerUpper(this, 12);
    
    if (radius != nullptr && radius->canPickThingsUp())
        setVisible(true);
    
    if (picker != nullptr && picker->canPickThingsUp())
    {
        IceMan* iceman = dynamic_cast<IceMan*>(picker);
        if (iceman != nullptr)
        {
            iceman->addBarrels();        // put the barrel in IceMan’s inventory
            getWorld()->playSound(SOUND_FOUND_OIL);
            getWorld()->increaseScore(1000);
            getWorld()->decBarrels();
            setVisible(false);
            setDead();
        }
    }
}

void SonarKit::doSomething() {
    if (!isAlive()) return;
    
    setVisible(true);
    
    m_ticksToLive++;
    int T = std::max(100, 300 - 10 * getWorld()->getCurrentGameLevel());
    if (m_ticksToLive >= T) {
        setDead();
        return;
    }
    
    Actor* picker = getWorld()->findNearbyPickerUpper(this, 3);
    if (picker != nullptr && picker->canPickThingsUp()) {
        IceMan* iceman = dynamic_cast<IceMan*>(picker);
        if (iceman != nullptr) {
            iceman->addSonar();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(75);
            setVisible(false);
            setDead();
        }
    }
}

void WaterPool::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
    
    m_ticksToLive++;
    
    int T = std::max(100, 300 - 10 * getWorld()->getCurrentGameLevel());
    
    if (m_ticksToLive >= T) {
        setDead();
        return;
    }
    
    Actor* picker = getWorld()->findNearbyPickerUpper(this, 3);
    Actor* radius = getWorld()->findNearbyPickerUpper(this, 12);
    
    if (radius != nullptr && radius->canPickThingsUp())
        setVisible(true);
    
    if (picker != nullptr && picker->canPickThingsUp()) {
        IceMan* iceman = dynamic_cast<IceMan*>(picker);
        if (iceman != nullptr) {
            iceman->addWater();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(100);
            setVisible(false);
            setDead();
        }
    }
}

bool Protester::moveInDirection(GraphObject::Direction dir) {
    int nx = getX(), ny = getY();
    switch (dir) {
        case up:    ++ny; break;
        case down:  --ny; break;
        case left:  --nx; break;
        case right: ++nx; break;
        default:    return false;
    }
    
    if (dir == up || dir == down)                 // moving vertically?
        if (nx >= 30 && nx <= 33 && ny > 3)       // in the tunnel rows
            nx = 30;
    
    if (nx < 0 || nx > 60 || ny < 0 || ny > 60)    return false;
    
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (getWorld()->iceAt(nx + i , ny + j)){
                return false;}
    
    if (!getWorld()->canActorMoveTo(this, nx, ny)) return false;
    
    setDirection(dir);
    moveTo(nx, ny);
    return true;
}

bool Protester::annoy(unsigned int amt)
{
    if (!isAlive()) return false;
    
    m_hitPoints -= amt;
    
    if (m_hitPoints <= 0)          // going home
    {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setLeaving();
        return true;
    }
    
    // stunned N ticks
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    m_restTicks = 20;
    return false;
}

void Protester::addGold() {
    m_restTicks = 20;
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}

void RegularProtester::doSomething()
{
    setVisible(true);
    
    if (!isAlive()) return;
    
    if (m_restTicks > 0) { --m_restTicks; return; }
    
    if (m_leaving) {
        if (getX() == 60 && getY() == 60) { setDead(); return;}
        
        Direction d = getWorld()->determineFirstMoveToExit(getX(), getY());
        if (d != none) moveInDirection(d);
        return;
    }
    
    if (m_moveCool) { --m_moveCool; return; }      // skip this tick
    m_moveCool = 3;
    
    if (m_yellCool  > 0)   --m_yellCool;              // yell cool-down
    
    ++m_ticksAlive;
    
    int R = std::max(100, 500 - 10 * getWorld()->getCurrentGameLevel());
    if (!m_leaving && m_ticksAlive >= R)          // Regular only
        setLeaving();
    
    IceMan* im = getWorld()->getIceMan();
    if (im && inRadius(getX(), getY(), im->getX(), im->getY(), 4.0) &&
        getWorld()->facingTowardIceMan(this) &&
        m_yellCool == 0)
    {
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->annoyIceMan(5);
        m_yellCool = 15;
        return;                    // done for this tick
    }
    
    GraphObject::Direction dir = getWorld()->determineFirstMoveToIceMan(getX(), getY());
    
    if (dir == GraphObject::none)
        dir = getDirection();
    
    moveInDirection(dir);
}


void HardcoreProtester::doSomething()
{
    setVisible(true);
    
    if (!isAlive()) return;
    
    if (m_restTicks > 0) {--m_restTicks; return;}
    
    if (m_leaving)
    {
        Direction d = getWorld()->determineFirstMoveToExit(getX(), getY());
        bool moved  = (d != none) && moveInDirection(d);

        if (!moved) {
            static const Direction cand[4] = { up, right, down, left };
            for (Direction alt : cand)
                if (alt != d && moveInDirection(alt)) { moved = true; break; }
        }

        return;
    }
    
    if (m_moveCool) { --m_moveCool; return; }      // skip this tick
    m_moveCool = 2;
    
    if (m_yellCool > 0)             // yell cool-down
        --m_yellCool;
    
    ++m_ticksAlive;
    
    int R = std::max(100, 500 - 10 * getWorld()->getCurrentGameLevel());
    if (!m_leaving && m_ticksAlive >= R)
        setLeaving();
    
    IceMan* im = getWorld()->getIceMan();
    if (!im) return;   // safety
    
    moveInDirection(getWorld()->determineFirstMoveToIceMan(getX(), getY()));
    
    if (inRadius(getX(), getY(), im->getX(), im->getY(), 4.0) &&
        getWorld()->facingTowardIceMan(this) && m_yellCool == 0)
    {
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->annoyIceMan(15);        // hardcore hurts 15
        m_yellCool = 15;   // reset cool-down
        return;
    }
}

void RegularProtester::move(){;}

void RegularProtester::giveUp(){m_leaving = true;}

bool RegularProtester::annoy(unsigned int amount) {
    Protester::annoy(amount);          // still plays sounds, sets stun
    setLeaving();
    return true;
}

void RegularProtester::receiveBribe() {
    getWorld()->increaseScore(25);
    m_restTicks = 10;
    setLeaving();
}

void RegularProtester::setLeaving() { m_leaving = true; }

bool HardcoreProtester::annoy(unsigned int amount)
{
    ++m_annoyCount;
    Protester::annoy(amount);
    
    if (m_annoyCount >= 3 || m_hitPoints <= 0)   // 3 hits OR HP zero
        setLeaving();
    return m_hitPoints <= 0;
}

void HardcoreProtester::receiveBribe()
{
    m_bribeCount++;
    m_restTicks = 10;
    if (m_bribeCount >= 2) {
        getWorld()->increaseScore(50);
        setLeaving();
    }
}
