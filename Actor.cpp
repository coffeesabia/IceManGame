//start of Actor.cpp
#include "Actor.h"
#include "StudentWorld.h"

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
                
        }
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
                   m_state = DONE;
                   return;
               }

               // Check if IceMan is within 3 units
               IceMan* iceman = getWorld()->getIceMan();
               if (iceman && iceman->isAlive()) {
                   int ix = iceman->getX();
                   int iy = iceman->getY();

                   // Check 4x4 overlapping area
                   if (x + 3 >= ix && x <= ix + 3 &&
                       y - 1 + 3 >= iy && y - 1 <= iy + 3) {
                       getWorld()->annoyIceMan(100); // Instant kill
                   }
               }

               // Move down one unit
               moveTo(x, y - 1);
            y--;
            
            // Check for ice beneath
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

void IceMan::annoy(int amount) {
    m_health -= amount;
    if (m_health <= 0) {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
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

    // Check bounds and blockages
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
    
    for (Actor* a : getWorld()->getActors()) {
            Protester* p = dynamic_cast<Protester*>(a);
            if (p && p->isAlive()) {
                double dx = p->getX() - getX();
                double dy = p->getY() - getY();
                double dist = sqrt(dx * dx + dy * dy);

                if (dist <= 3.0) {
                    if (p->isHardcore())
                        p->annoy(15);
                    else
                        p->annoy(5);
                    setDead();
                    return;
                }
            }
        }
}


void GoldNugget::doSomething()
{
    if (!isAlive()) return;
    
    Actor* a = getWorld()->findNearbyPickerUpper(this, 3);
    Actor* radius = getWorld()->findNearbyPickerUpper(this, 12);
    
    if (radius != nullptr && radius->canPickThingsUp())
        setVisible(true);
        
    if (a != nullptr && a->canPickThingsUp()) {
        Agent* ag = dynamic_cast<Agent*>(a);
        if (ag != nullptr) {
            ag->addGold();
            setVisible(false);
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
        }
        }
    
}

void OilBarrel::doSomething()
{
   if (!isAlive())
            return;

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

void Protester::moveInDirection(GraphObject::Direction dir) {
    if (!isAlive()) return;
        setVisible(true);

    IceMan* iceman = getWorld()->getIceMan();
    if (iceman && isAlive()) {
        int dx = iceman->getX() - getX();
        int dy = iceman->getY() - getY();
        double dist = sqrt(dx * dx + dy * dy);

        if (dist <= 4.0) {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            iceman->annoy(2); // Yell does 2 HP damage (if your spec uses this)
            return; // end tick here if needed
        }
    }
    
        m_ticksAlive++;

    int R = std::max(100, 500 - 10 * getWorld()->getCurrentGameLevel());
    
    if (m_ticksAlive >= R && !isLeaving()) {
        getWorld()->determineFirstMoveToExit(60, 0);
        return;
    }
}

void RegularProtester::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
    
    IceMan* iceman = getWorld()->getIceMan();
    const vector<RegularProtester*>& regPro = getWorld()->getRegProtester();
    for (auto protester : regPro) {
        if (iceman && isAlive()) {
            // Use 'protester' pointer, not 'regPro'
            getWorld()->determineFirstMoveToIceMan(protester->getX(), protester->getY());
            
            int dx = iceman->getX() - getX();
            int dy = iceman->getY() - getY();
            double dist = sqrt(dx * dx + dy * dy);
            
            if (dist <= 4.0) {
                
                getWorld()->playSound(SOUND_PROTESTER_YELL);
                getWorld()->annoyIceMan(5);; // Yell does 2 HP damage (if your spec uses this)
                return; // end tick here if needed */
            }
        }
    }
       // The max allowed ticks before leaving:
        int R = std::max(100, 500 - 10 * getWorld()->getCurrentGameLevel());

        if (m_ticksAlive >= R) {
            // Time to leave, call determineFirstMoveToExit to move towards exit
            GraphObject::Direction dir = getWorld()->determineFirstMoveToExit(getX(), getY());

            if (dir != GraphObject::none) {
                moveInDirection(dir);  // Assuming you have a moveInDirection() or similar
            }
            else {
                // Already at exit or nowhere to go, maybe setAlive(false) or similar
                !isAlive();
            }
            return;  // Skip normal behavior since it's leaving
        }
}


void HardcoreProtester::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
    
    
    IceMan* iceman = getWorld()->getIceMan();
    const vector<HardcoreProtester*>& hardPro = getWorld()->getHardProtester();
    
    for (auto protester : hardPro) {
        if (iceman && isAlive()) {
            // Use 'protester' pointer, not 'regPro'
            getWorld()->determineFirstMoveToIceMan(protester->getX(), protester->getY());
            
            int dx = iceman->getX() - getX();
            int dy = iceman->getY() - getY();
            double dist = sqrt(dx * dx + dy * dy);
            
            if (dist <= 4.0) {
                getWorld()->playSound(SOUND_PROTESTER_YELL);
                getWorld()->annoyIceMan(15); // Yell does 2 HP damage (if your spec uses this)
                return; // end tick here if needed
            }
        }
        
    }
}
void RegularProtester::move(){;}
void RegularProtester:: addGold(){;}

void RegularProtester::giveUp(){m_leaving = true;}

bool RegularProtester::annoy(unsigned int amount) {
        bool dead = Protester::annoy(amount);
        if (!dead) {
            setDead(); // one hit = gone
        }
        return true;
    }

void RegularProtester::receiveBribe() {
        setDead();
}

void RegularProtester::setLeaving() { m_leaving = true; }
//end of Actor.cpp
