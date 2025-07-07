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

int StudentWorld::init()
{
    
    if (m_resetToLevelZero) {
        setLevel(0);
        m_resetToLevelZero = false;
    }
    
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 60; y++) {
            IcePointers[x][y] = nullptr;
        }
    }
    
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
        m_barrelsLeft = numberOfBarrels;

        int numberOfNuggets = max((5 - getCurrentGameLevel()) / 2, 2);
        placed = 0;
        while (placed < numberOfNuggets) {
            int x = rand() % 61;
            int y = rand() % 37 + 20;

            if (x >= 27 && x <= 33 && y > 20) continue;

            if (!isFarEnough(x, y)) continue;

            GoldNugget* gold = new GoldNugget(x, y, this, true, true, false);
            m_gold.push_back(gold);
            placed++;
        }
    
    m_itemTimer = 0;
    m_toggleState = 0; // Start with sonar
    
   return GWSTATUS_CONTINUE_GAME;
    
}

int StudentWorld::move()
{
    if (m_iceman && m_iceman -> isAlive())
        m_iceman -> doSomething();
    
    m_RegprotesterTimer++;
    m_HardprotesterTimer++;
    m_ticksSinceLevelStart++;

    static int tickCount = 0;
    tickCount++;

    for (int i = 0; i < m_Rprotester.size(); ) {
        if (m_Rprotester[i] && m_Rprotester[i]->isAlive()) {
            if (tickCount % 2 == 0) {
                m_Rprotester[i]->doSomething();
            }
            ++i;
        } else {
            delete m_Rprotester[i];
            m_Rprotester.erase(m_Rprotester.begin() + i);
        }
    }

    // Hardcore protester moves every tick
    for (int i = 0; i < m_Hprotester.size(); ) {
        if (m_Hprotester[i] && m_Hprotester[i]->isAlive()) {
            m_Hprotester[i]->doSomething();
            ++i;
        } else {
            delete m_Hprotester[i];
            m_Hprotester.erase(m_Hprotester.begin() + i);
        }
    }
    
    int R = std::max(100, 500 - 10 * getCurrentGameLevel());

    // Spawn RegularProtester every 10 ticks
    if (m_RegprotesterTimer >= 10) {
        m_RegprotesterTimer = 0;
        RegularProtester* rp = new RegularProtester(60, 60, this);
        m_Rprotester.push_back(rp);
    }

    // Spawn HardcoreProtester every R ticks
    if (m_HardprotesterTimer >= R) {
        m_HardprotesterTimer = 0;
        HardcoreProtester* hp = new HardcoreProtester(60, 60, this);
        m_Hprotester.push_back(hp);
    }
    
    //iterate through all boulders
    for (int i = 0; i < m_boulder.size(); ++i)
    {
        if (m_boulder[i] && m_boulder[i] -> isAlive())
            m_boulder[i] -> doSomething();
    }
    
   for (int i = 0; i < m_gold.size(); ) {
        if (m_gold[i] && m_gold[i]->isAlive()) {
            m_gold[i]->doSomething();
            ++i;
        } else {
            delete m_gold[i];
            m_gold.erase(m_gold.begin() + i);
        }
    }
    
    for (int i = 0; i < m_barrel.size(); ) {
        if (m_barrel[i] && m_barrel[i]->isAlive()) {
            m_barrel[i]->doSomething();
            ++i;
        } else {
            delete m_barrel[i];
            m_barrel.erase(m_barrel.begin() + i); //
        }
    }
    
    int T = max(100, 300 - 10 * getCurrentGameLevel());
    
    for (int i = 0; i < m_sonar.size(); ) {
            if (m_sonar[i] && m_sonar[i]->isAlive()) {
                m_sonar[i]->doSomething();
                ++i;
            } else {
                delete m_sonar[i];
                m_sonar.erase(m_sonar.begin() + i); //
            }
        }
    
    for (int i = 0; i < m_pool.size(); ) {
        if (m_pool[i] && m_pool[i]->isAlive()) {
            m_pool[i]->doSomething();
            ++i;
        } else {
            delete m_pool[i];
            m_pool.erase(m_pool.begin() + i); //
        }
    }
    
    m_itemTimer++;

    if (m_itemTimer >= T) {
        m_itemTimer = 0;

        if (m_toggleState == 0) {
            
            SonarKit* sonar = new SonarKit(0, 60, this, true, true);
            m_sonar.push_back(sonar);
        } else {
            
            int tries = 0;
            while (tries < 100) {
                int x = rand() % 61;
                int y = rand() % 57;

                bool isCleared = true;
                for (int i = x; i < x + 4 && i < 64; ++i) {
                    for (int j = y; j < y + 4 && j < 60; ++j) {
                        if (iceAt(i, j)) {
                            isCleared = false;
                            break;
                        }
                    }
                    if (!isCleared) break;
                }

                if (isCleared) {
                    WaterPool* pool = new WaterPool(x, y, this, true, true);
                    m_pool.push_back(pool);
                    break;
                }

                tries++;
            }
        }

        m_toggleState = 1 - m_toggleState;
    }
    
    for (int i = 0; i < m_squirt.size(); ) {
        if (m_squirt[i] && m_squirt[i]->isAlive()) {
            m_squirt[i]->doSomething();
            ++i;
        } else {
            delete m_squirt[i];
            m_squirt.erase(m_squirt.begin() + i);
        }
    }
    
    for (int i = 0; i < m_Rprotester.size(); ) {
        if (m_Rprotester[i] && m_Rprotester[i]->isAlive()) {
            m_Rprotester[i]->doSomething();
            ++i;
        } else {
            delete m_Rprotester[i];
            m_Rprotester.erase(m_Rprotester.begin() + i);
        }
    }

    for (int i = 0; i < m_Hprotester.size(); ) {
        if (m_Hprotester[i] && m_Hprotester[i]->isAlive()) {
            m_Hprotester[i]->doSomething();
            ++i;
        } else {
            delete m_Hprotester[i];
            m_Hprotester.erase(m_Hprotester.begin() + i);
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
    
    if (getNumberOfBarrelsRemainingToBePickedUp() == 0)
        return GWSTATUS_FINISHED_LEVEL;
    
    if (!m_iceman || !m_iceman->isAlive()) {
        m_livesLeft--;
        if (m_livesLeft <= 0) {
            setLevel(0);         
            m_livesLeft = 3;
            return GWSTATUS_PLAYER_DIED;  // Let GameController restart at level 0
        } else {
            return GWSTATUS_PLAYER_DIED;
        }
    }
    
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
    
    for (Actor* s : m_squirt)
            delete s;
    m_squirt.clear();
    
    for (Boulder* b : m_boulder)
            delete b;
        m_boulder.clear();

   for (auto g : m_gold)
        delete g;
    m_gold.clear();
    
    for (auto b : m_barrel) delete b;
    m_barrel.clear();

    for (auto s : m_sonar) delete s;
    m_sonar.clear();

    for (auto w : m_pool) delete w;
    m_pool.clear();
    
    for (size_t i = 0; i < m_Rprotester.size(); ++i) {
        if (m_Rprotester[i]) {
            delete m_Rprotester[i];
            m_Rprotester[i] = nullptr;
        }
    }
    m_Rprotester.clear();
    
    for (size_t i = 0; i < m_Hprotester.size(); ++i) {
        if (m_Hprotester[i]) {
            delete m_Hprotester[i];
            m_Hprotester[i] = nullptr;
        }
    }
    m_Hprotester.clear();
}

bool StudentWorld::clearIce(int x, int y)
{
    bool removedIce = false;

    for (int i = x; i < x + 4 && i < 64; ++i) {
        for (int j = y; j < y + 4 && j < 60; ++j) {
            if (IcePointers[i][j] != nullptr) {
                delete IcePointers[i][j];
                IcePointers[i][j] = nullptr;
                removedIce = true;
            }
        }
    }

    return removedIce;
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
     
     return true;
}

int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius){
    return 0;
}

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius){
    auto tryReveal = [&](auto& container) {
        for (ActivatingObject* a : container) {
            if (!a->isAlive()) continue;

            double dx = a->getX() - x;
            double dy = a->getY() - y;
            if (sqrt(dx*dx + dy*dy) <= radius)
                a->setVisible(true);
        }
    };

    tryReveal(m_barrel);
    tryReveal(m_gold);
}

Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const{
    return a;
}

Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const{
    // First, check if IceMan is in range
    if (!a) return nullptr;
    
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

void StudentWorld::annoyIceMan(int amount){
    if (m_iceman && m_iceman->isAlive()) {
            m_iceman->annoy(amount);
        }
}

bool StudentWorld::facingTowardIceMan(Actor* a) const{
    int ax=a->getX(), ay=a->getY(),
            ix=m_iceman->getX(), iy=m_iceman->getY();
        if (ax==ix)
            return ( (ay<iy && a->getDirection()==GraphObject::up   ) ||
                     (ay>iy && a->getDirection()==GraphObject::down ) );
        if (ay==iy)
            return ( (ax<ix && a->getDirection()==GraphObject::right) ||
                     (ax>ix && a->getDirection()==GraphObject::left ) );
        return false;
}

GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, int y){
    
    const int exitX = 60;
        const int exitY = 60;

        if (x == exitX && y== exitY)
            return ActivatingObject::none;  // Already at exit

        // Directions: up, down, left, right
        const int dx[4] = {0, 0, -1, 1};
        const int dy[4] = {1, -1, 0, 0};
        const GraphObject::Direction dir[4] = {GraphObject::up, GraphObject::down, GraphObject::left, GraphObject::right};

        // Distance grid: -1 means unvisited
        vector<vector<int>> dist(64, vector<int>(60, -1));
        dist[exitX][exitY] = 0;

        queue<pair<int,int>> q;
        q.push({exitX, exitY});

        // BFS from exit backwards
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < 64 && ny >= 0 && ny < 60) {
                    // Check if square is not blocked (no ice or boulder)
                    if (dist[nx][ny] == -1 && !iceAt(nx, ny) && canActorMoveTo(nullptr, nx, ny)) {
                        dist[nx][ny] = dist[x][y] + 1;
                        q.push({nx, ny});
                    }
                }
            }
        }

        // Now from Protester's position, find neighbor with smallest dist
        int minDist = dist[x][y];
        if (minDist == -1) return ActivatingObject::none; // No path

        // Check neighbors to find next step closer to exit
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < 64 && ny >= 0 && ny < 60) {
                if (dist[nx][ny] != -1 && dist[nx][ny] < minDist) {
                    return dir[i];  // Move toward neighbor closer to exit
                }
            }
        }

        return ActivatingObject::none;  // no valid move found
}

GraphObject::Direction StudentWorld::determineFirstMoveToIceMan(int x, int y){
    
    if (!m_iceman || !m_iceman->isAlive())
            return ActivatingObject::none;

        int ix = m_iceman->getX();
        int iy = m_iceman->getY();

        if (x == ix && y == iy)
            return ActivatingObject::none;

        const int dx[4] = {0, 0, -1, 1};
        const int dy[4] = {1, -1, 0, 0};
        const GraphObject::Direction dir[4] = {GraphObject::up, GraphObject::down, GraphObject::left, GraphObject::right};

        vector<vector<int>> dist(64, vector<int>(60, -1));
        dist[ix][iy] = 0;

        queue<pair<int,int>> q;
        q.push({ix, iy});

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < 64 && ny >= 0 && ny < 60) {
                    if (dist[nx][ny] == -1 && !iceAt(nx, ny) && canActorMoveTo(nullptr, nx, ny)) {
                        dist[nx][ny] = dist[x][y] + 1;
                        q.push({nx, ny});
                    }
                }
            }
        }

        int minDist = dist[x][y];
        if (minDist == -1) return ActivatingObject::none;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < 64 && ny >= 0 && ny < 60) {
                if (dist[nx][ny] != -1 && dist[nx][ny] < minDist) {
                    return dir[i];
                }
            }
        }

        return ActivatingObject::none;
}

bool StudentWorld::iceAt(int x, int y) const {
   if (x < 0 || x >= 64 || y < 0 || y >= 60)
            return false;
        
    if(IcePointers[x][y] != nullptr)
        return true;
    else
        return false;
}

//end of StudentWorld.cpp
