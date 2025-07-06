//start of StudentWorld.cpp
#include "StudentWorld.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>


using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

/*bool StudentWorld::NearIceman(int x, int y, int amount)
{
    int square = pow(m_iceman -> getX() - x, 2) + pow(m_iceman -> getY() - y, 2);
    if(sqrt(square) <= amount)
        return true;
    
    return false;
}

void StudentWorld::setPositions(int x, int y, char objtype)
{
    for (int i = x; i != x + 4; i++)
        for (int j = y; j != y + 4; j++)
            ObjPositions[i][j] = objtype;
}*/



int StudentWorld::init()
{
     for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 60; y++) {
                if (x >= 30 && x <= 33 && y > 3) {
                    IcePointers[x][y] = nullptr;  // tunnel area
                } else {
                    IcePointers[x][y] = new Ice(x, y, this);
                }
            }
        }
    
    m_iceman = new IceMan(30, 60, this);
    
    auto isFarEnough = [&](int x, int y) {
            for (Boulder* b : m_boulder) {
                int dx = b->getX() - x;
                int dy = b->getY() - y;
                if (sqrt(dx * dx + dy * dy) < 6.0) return false;
            }
            for (GoldNugget* g : m_gold) {
                int dx = g->getX() - x;
                int dy = g->getY() - y;
                if (sqrt(dx * dx + dy * dy) < 6.0) return false;
            }
            for (OilBarrel* o : m_barrel) {
                int dx = o->getX() - x;
                int dy = o->getY() - y;
                if (sqrt(dx * dx + dy * dy) < 6.0) return false;
            }
            return true;
        };

        int numberOfBoulders = min((getCurrentGameLevel() / 2) + 2, 9);
        int placed = 0;
        while (placed < numberOfBoulders) {
            int x = rand() % 61;
            int y = rand() % 37 + 20;

            if (x >= 27 && x <= 33 && y > 20) continue;

            if (!isFarEnough(x, y)) continue;

            Boulder* b = new Boulder(x, y, this);
            m_boulder.push_back(b);
            clearIce(x, y);
            placed++;
        }

        int numberOfBarrels = min((2 + getCurrentGameLevel()), 21);
        placed = 0;
        while (placed < numberOfBarrels) {
            int x = rand() % 61;
            int y = rand() % 37 + 20;

            if (x >= 27 && x <= 33 && y > 20) continue;

            if (!isFarEnough(x, y)) continue;

            OilBarrel* barrel = new OilBarrel(x, y, this, true, true);
            m_barrel.push_back(barrel);
            placed++;
        }

        int numberOfNuggets = max((5 - getCurrentGameLevel()) / 2, 2);
        placed = 0;
        while (placed < numberOfNuggets) {
            int x = rand() % 61;
            int y = rand() % 37 + 20;

            if (x >= 27 && x <= 33 && y > 20) continue;

            if (!isFarEnough(x, y)) continue;

            GoldNugget* gold = new GoldNugget(x, y, this, true, true);
            m_gold.push_back(gold);
            placed++;
        }
    
    int T = max(100, 300 - 10 * getCurrentGameLevel());

    
    
   return GWSTATUS_CONTINUE_GAME;
    
}

int StudentWorld::move()
{
    if (m_iceman -> isAlive())
        m_iceman -> doSomething();
    
    
    //iterate through all boulders
    for (int i = 0; i < m_boulder.size(); ++i)
    {
        if (m_boulder[i] -> isAlive())
            m_boulder[i] -> doSomething();
    }
    
    /*for (int i = 0; i < m_gold.size(); ++i)
    {
        if (m_gold[i] -> isAlive())
            m_gold[i] -> doSomething();
        if (!m_gold[i] -> isAlive())
            delete m_gold[i];
    }*/
    
    for (int i = 0; i < m_gold.size(); ) {
        if (m_gold[i]->isAlive()) {
            m_gold[i]->doSomething();
            ++i;
        } else {
            delete m_gold[i];
            m_gold.erase(m_gold.begin() + i); // don't increment i here
        }
    }
    
    for (int i = 0; i < m_barrel.size(); ) {
        if (m_barrel[i]->isAlive()) {
            m_barrel[i]->doSomething();
            ++i;
        } else {
            delete m_barrel[i];
            m_barrel.erase(m_barrel.begin() + i); // don't increment i here
        }
    }
    
    ostringstream oss;

    int level = getCurrentGameLevel();
    int lives = getNumLivesLeft();
    int health = getCurrentHealth();
    int squirts = getSquirtsLeftInSquirtGun();
    int gold = getPlayerGoldCount();
    int barrelsLeft = getNumberOfBarrelsRemainingToBePickedUp();
    int sonar = getPlayerSonarChargeCount();
    int score = getCurrentScore();
    
    oss << "Level: " << level << " ";
    oss << "Lives: " << lives << "  ";
    oss << "Health: " << setw(3) << health << "%  ";
    oss << "Squirts: " << squirts << "  ";
    oss << "Gold: " << gold << "  ";
    oss << "Barrels: " << barrelsLeft << " ";
    oss << "Sonar: " << sonar << " ";
    oss << "Score: " << setw(6) << setfill('0') << score;

    setGameStatText(oss.str());
    
    //if (getNumberOfBarrelsRemainingToBePickedUp() == 0)
        //return GWSTATUS_FINISHED_LEVEL;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 60; ++j) {
            if (IcePointers[i][j] != nullptr) {
                delete IcePointers[i][j];
                IcePointers[i][j] = nullptr;
            }
        }
    }
    
    if (m_iceman != nullptr) {
        delete m_iceman;
        m_iceman = nullptr;
    }
    
    /*if (m_Rprotestor != nullptr) {
        delete m_Rprotestor;
        m_Rprotestor = nullptr;
    }
    
    if (m_Hprotestor != nullptr) {
        delete m_Hprotestor;
        m_Hprotestor = nullptr;
    }*/
    
    for (Boulder* b : m_boulder) {
        delete b;
    }
    m_boulder.clear();

    
    for (GoldNugget* g : m_gold) {
        delete g;
    }
    m_gold.clear();
    
    for (OilBarrel* o : m_barrel) {
        delete o;
    }
    m_barrel.clear();
    
    for (SonarKit* kit : m_sonar) delete kit;
    m_sonar.clear();
    

    for (WaterPool* pool : m_pool) delete pool;
    m_pool.clear();
    
    /*for (Actor* obj : Objects) {
        delete obj;
    }
    
    Objects.clear();*/
}

// Can actor move to x,y?, is there a boulder, or protestor?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
    // Check if moving actor would overlap with any alive Boulder
    for (Boulder* b : m_boulder) {
        if (b == a)
            continue;
        if (b && b->isAlive()) {
            int bx = b->getX();
            int by = b->getY();
            // Check for 4x4 overlap between actor at (x, y) and Boulder at (bx, by)
            if (x + 3 >= bx && x <= bx + 3 &&
                y + 3 >= by && y <= by + 3) {
                return false; // Blocked by Boulder
            }
        }
    }

    // Add additional checks here for Protesters, if needed

    return true; // No Boulder or blocking object at the destination
}

//deleting ice when iceman runs into it
void StudentWorld::clearIce(int x, int y)
{
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 60; ++j) {
            if (IcePointers[i][j] != nullptr) {
                int ix = IcePointers[i][j]->getX();
                int iy = IcePointers[i][j]->getY();
                
                if (ix >= x && ix <= x + 3 && iy >= y && iy <= y + 3) {
                    delete IcePointers[i][j];
                    IcePointers[i][j] = nullptr;
                }
            }
        }
    }
}

/*bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
    int ax = a->getX();
    int ay = a->getY();

    for (int x = max(0, ax - radius); x <= min(63, ax + radius); ++x) {
        for (int y = max(0, ay - radius); y <= min(59, ay + radius); ++y) {
            if (IcePointers[x][y] != nullptr) {
                int dx = x - ax;
                int dy = y - ay;
                if (dx*dx + dy*dy <= radius * radius) {
                    return true; // Found ice within radius
                    }
                }
            }
        }
        return false;
    }*/

bool StudentWorld::iceAt(int x, int y) const {
   if (x < 0 || x >= 64 || y < 0 || y >= 60)
            return false;
        
    if(IcePointers[x][y] != nullptr)
        return true;
    else
        return false;
    
    
   /* for (int i = x; i < x + 4; i++) {
        for (int j = y; j < y + 4; j++) {
            if (i < 0 || i >= 64 || j < 0 || j >= 60)
                continue; // skip out-of-bounds but don’t return false
            if (IcePointers[i][j] != nullptr)
                return true; // found ice in the 4x4 region
        }
    }
    return false; // no ice in the 4x4 region*/
}

Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const{
    // First, check if IceMan is in range
       int ax = a->getX();
       int ay = a->getY();

       if (m_iceman != nullptr && m_iceman->isAlive()) {
           int ix = m_iceman->getX();
           int iy = m_iceman->getY();

           double dist = sqrt(pow(ix - ax, 2) + pow(iy - ay, 2));
           if (dist <= radius && m_iceman->canPickThingsUp()) {
               return m_iceman;
           }
       }

       // Later, you can add logic for Protesters here if needed
       return nullptr;
}


//end of StudentWorld.cpp
