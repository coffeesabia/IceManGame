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
            if (m_waitTicks >= 30)
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
                setDead(); // hit ice
                return;
            }

            // Safe to move down
            moveTo(x, y - 1);

            // Optional: Annoy IceMan or Protesters within radius 3
            // You can add this later if needed

            break;
            
            
            
            /*If the Boulder is currently in the stable state, then it must check to see if there is
             any Ice in the 4 squares immediately below it. For example, if the Boulder is at
             location x=20,y=30, it would check squares (20,29), (21,29), (22,29) and (23,29)
             for Ice. If there is any Ice below the Boulder, it does nothing – after all it’s a
             dumb rock. However, if none of the 4 squares beneath the Boulder have any Ice,
             then the Boulder must transition into a waiting state:
             A. It must enter a waiting state for the next 30 ticks*/
             
             /*3. If the Boulder is in a waiting state and 30 ticks have elapsed, then it must
             transition into a falling state and play the sound SOUND_FALLING_ROCK.
             
             4. If the Boulder is in a falling state, then:
             A. It must continue to move downward one square during each tick until it
             either (a) hits the bottom of the oil field (i.e., it tries to move to y=-1), (b)
             runs into the top of another Boulder, or (c) it runs into Ice (i.e., by
             moving down a square, the Boulder would overlap over one or more Ice
             objects). When any of the above conditions are met the Boulder must set
             its state to dead so it can be removed from the game at the end of the
             current tick.
             B. If the Boulder comes within a radius of 3 (i.e. less than or equal to 3
             squares, so 2.99 would count) of any Protester(s) or the Iceman while
             falling, it must cause 100 points of annoyance to those actors (effectively
             completely annoying them in one fell swoop). The Boulder will continue
             falling down as descri
              bed above, even if it annoys one or more Protesters
             on its way down (i.e., it won’t stop falling if it runs into a Protester).
             However, if the Boulder annoys the Iceman, the player will instantly lose
             a life*/
            
        }
    }
}

void GoldNugget::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
}

void OilBarrel::doSomething()
{
    if (!isAlive()) return;
    
    setVisible(true);
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

//end of Actor.cpp
