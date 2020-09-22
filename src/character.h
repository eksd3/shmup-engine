#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include "grotto_def.h"
#include "renderer.h"
#include "shot.h"



class Character
{
protected:
    Sprite *    sprite;
    Hitbox      hitbox;
    float       xvelocity;
    float       yvelocity;
    bool        friendly;
    bool        vulnerable;
    int         timer;

public:

    // Used for inheritance
    void init(float xvel, float yvel, bool fr);

    Hitbox * getHitbox()        { return &hitbox; };
    Sprite * getSprite() const  { return sprite; };
    float getXVelocity() const  { return xvelocity; };
    float getYVelocity() const  { return yvelocity; };
    bool getFriendly() const    { return friendly; };
    int getTimer() const        { return timer; };
    bool isVulnerable() const   { return vulnerable; };

    // Takes a unit vector as the arg
    void moveCharacter(vec2 vec);
    void incrementTimer(int n);
    void resetTimer();

    void setSprite(SDL_Texture * tex, int w, int h);
    void setXVelocity(float v)      { xvelocity = v; };
    void setYVelocity(float v)      { yvelocity = v; };
    void setVelocity(float v)       { setXVelocity(v); setYVelocity(v); };
    void setVulnerability(bool v)   { vulnerable = v; };

    // iWinW - window width     iWinH - window height   iOffset - leeway
    bool isOutOfBounds(int iWinW, int iWinH, int iOffset);
};



class Player : public Character
{
    // Player character

public:
    Player(vec2 pos, vec2 dim, float vel);
    Player();

    virtual ~Player() { delete sprite; };

    // iWinW - window width
    // iWinH - window height
    void movePlayer(vec2 vec, int iWinW, int iWinH);
};



class Enemy : public Character
{
protected:
    int     hp;

    // How long until the enemy spawns (in ms), from
    // staring the stage
    int     spawnTimer;

    // Delay between the executions of shotPattern()
    int     shotTimer;

    // Used for scripting
    int     state;
    std::unordered_map<std::string, int> m_vars;

    // Shot pattern and movement script
    void(*movementScript)(Enemy * en);
    void(*shotPattern)(Enemy * src, Character * t, ShotController * shc);

public:

    // Used for inheritance
    void initEnemy(float xvel, float yvel, int _hp, int spawn);

    void incrementShotTimer(int n)  { shotTimer += n; };
    void resetShotTimer()           { shotTimer = 0; };
    void addVar(std::string key, int val);

    int getHP() const               { return hp; };
    int getSpawnTimer() const       { return spawnTimer; };
    int getTimer() const            { return timer; };
    int getShotTimer() const        { return shotTimer; };
    int getState() const            { return state;} ;
    int getVar(std::string key);

    void setHP(int h)               { hp = h; };
    void setState(int s)            { state = s; };

    void execMovementScript();
    void execShotScript(Character* t, ShotController* shc);
};


class EnemyController
{
    // Handles enemy spawns stuff

private:

    // Actually only need the renderer
    SDL *   m_sdl;

    // Local timer
    int     timer;

    // How much an enemy can go off screen before
    // it is considered out of bounds
    int     m_boundary;

    // Enemies that haven't yet spawned
    std::vector<Enemy*> m_enemyVector;

    std::vector<Enemy*> m_spawnedEnemies;
    std::vector<Enemy*> m_destroyedEnemies;

public:
    EnemyController(SDL * sdl);
    ~EnemyController();

    // Add to m_enemyVector
    void add(Enemy * en);

    // Remove from m_enemyVector
    void removeEnemy(Enemy * en);

    // Remove from m_enemyVector and add to m_spawnedEnemies
    void spawn(Enemy * en);

    // Checks if m_enemyVector[0] should spawn and spawns it if true
    // recursive
    void spawnFirstEnemy();

    // Update enemy positions etc.
    void update();

    // Loops through m_spawnedEnemies and removes them if HP <= 0
    void destroyDead();

    void setBoundary(int n)     { m_boundary = n; };

    int getNumOfUnspawned() const   { return m_enemyVector.size(); };
    int getNumOfSpawned() const     { return m_spawnedEnemies.size(); };

    std::vector<Enemy*> getSpawned() const      { return m_spawnedEnemies; };
    std::vector<Enemy*> getUnspawned() const    { return m_enemyVector; };

    int getTimer() const { return timer; };
    void resetTimer() { timer = 0; };
};



#endif // __CHARACTER_H_
