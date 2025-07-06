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
                if(getY() + 1 < 60)
                {
                    setDirection(up);
                    
                    if(getWorld()->canActorMoveTo(this, getX(), getY()+1))
                    {
                        moveTo(getX(), getY() + 1);
                        getWorld()->clearIce(getX(), getY());
                        getWorld()->playSound(SOUND_DIG);
                    }
                }
                break;
            case 'w':
                if(getY() + 1 < 60)
                {
                    setDirection(up);
                    
                   if(getWorld()->canActorMoveTo(this, getX(), getY()+1))
                    {
                        moveTo(getX(), getY() + 1);
                        getWorld()->clearIce(getX(), getY());
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
                        getWorld()->clearIce(getX(), getY());
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
                        getWorld()->clearIce(getX(), getY());
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
                        getWorld()->clearIce(getX(), getY());
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
                        getWorld()->clearIce(getX(), getY());
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
                        getWorld()->clearIce(getX(), getY());
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
                        getWorld()->clearIce(getX(), getY());
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
                setDead(); // hit bottom
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
    
    setVisible(true);
    
    Actor* a = getWorld()->findNearbyPickerUpper(this, 3);
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
    if (!isAlive()) return;
    
    setVisible(true);
    
    Actor* a = getWorld()->findNearbyPickerUpper(this, 3);
        if (a != nullptr && a->canPickThingsUp()) {
            ActivatingObject* ag = dynamic_cast<ActivatingObject*>(a);
            if (ag != nullptr) {
                ag->addBarrels();
                setVisible(false);
                setDead();                
                getWorld()->playSound(SOUND_FOUND_OIL);
                getWorld()->increaseScore(1000);
            }
        }
}

SonarKit::SonarKit(int startX, int startY, StudentWorld* sw, bool isVisible, bool canPick): ActivatingObject(sw, startX, startY, IID_SONAR, SOUND_SONAR, false, true, true)
{
    setVisible(isVisible);
    m_ticksToLive = max(100, 300 - 10 * sw->getCurrentGameLevel());
}

WaterPool::WaterPool(int startX, int startY, StudentWorld* sw, bool isVisible, bool canPick): ActivatingObject(sw, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE, false, true, true)
{
    setVisible(isVisible);
    m_ticksToLive = max(100, 300 - 10 * sw->getCurrentGameLevel());
}

void SonarKit::doSomething() {
    if (!isAlive()) return;
    
    setVisible(true);
}

void WaterPool::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
}

bool Agent::canPickThingsUp() const{
    
    
    return false;
}

//end of Actor.cpp


