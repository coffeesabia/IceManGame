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
            // Check if we can fall further
            if (y - 1 < 0) {
                setDead();
                setVisible(false);
                return;
            }

            // Check for boulder collision
            if (!getWorld()->canActorMoveTo(this, x, y - 1)) {
                setDead(); // hit another Boulder
                return;
            }

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

            // Safe to move down
            moveTo(x, y - 1);
            --y;

            // Optional: Annoy IceMan or Protesters within radius 3
            // You can add this later if needed

            break;
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

void RegularProtester::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
}

void HardcoreProtester::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
}

void Squirt::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
}

//end of Actor.cpp
