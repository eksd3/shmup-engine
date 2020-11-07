#ifndef __ENEMYOBJECT_H_
#define __ENEMYOBJECT_H_

#include "GROTTO.h"
#include "patterns.h"



// ---------------------------------------------------------------------------------------------------------



class EnemyStage1 : public Enemy
{

public:

    EnemyStage1( SDL_Texture * tex, int xstart, int ystart, int spawn )
    {
        int maxhp = 200;
        initEnemy(0.0f, 1.0f, maxhp, spawn);

        // Hp threshold is 1/3 of max hp
        int hpt = (int)(maxhp * 0.3f);

        addVar("m_HPThreshold_1", 2 * hpt);
        addVar("m_HPThreshold_2", hpt);
        addVar("m_maxhp", maxhp);

        addVar("m_shotTimerTargeted", 0);
        addVar("m_shotTimerStraightArray", 0);

        setSprite(tex, 96, 96);
        sprite->rotationAngle = 180.0f;
        hitbox = Hitbox(vec4(xstart, ystart, 96, 96));
        movementScript = &patterns::enemyMovementScript;
        shotPattern = &patterns::stage1ShotPattern;
    };

    ~EnemyStage1()
    {
        delete sprite;
    };
};



// ---------------------------------------------------------------------------------------------------------



class EnemyStage2 : public Enemy
{

public:

    EnemyStage2( SDL_Texture * tex, int xstart, int ystart, int spawn )
    {
        int maxhp = 250;
        initEnemy(0.0f, 1.0f, maxhp, spawn);

        // Hp threshold is 1/3 of max hp
        int hpt = (int)(maxhp * 0.3f);

        addVar("m_HPThreshold_1", 2 * hpt);
        addVar("m_HPThreshold_2", hpt);
        addVar("m_maxhp", maxhp);

        addVar("m_shotTimerTargeted", 0);
        addVar("m_shotTimerStraightArray", 0);
        addVar("m_shotTimerDelayed", 0);

        setSprite(tex, 96, 96);
        sprite->rotationAngle = 180.0f;
        hitbox = Hitbox(vec4(xstart, ystart, 96, 96));
        movementScript = &patterns::enemyMovementScript;
        shotPattern = &patterns::stage2ShotPattern;
    };

    ~EnemyStage2()
    {
        delete sprite;
    };
};



// ---------------------------------------------------------------------------------------------------------



class EnemyStage3 : public Enemy
{

public:

    EnemyStage3( SDL_Texture * tex, int xstart, int ystart, int spawn )
    {
        int maxhp = 300;
        initEnemy(0.0f, 1.0f, maxhp, spawn);

        // Hp threshold is 1/3 of max hp
        int hpt = (int)(maxhp * 0.3f);

        addVar("m_HPThreshold_1", 2 * hpt);
        addVar("m_HPThreshold_2", hpt);
        addVar("m_maxhp", maxhp);

        addVar("m_shotTimerTargeted", 0);
        addVar("m_shotTimerStraightArray", 0);
        addVar("m_shotTimerDelayed", 0);
        addVar("m_shotTimerRandom", 0);

        setSprite(tex, 96, 96);
        sprite->rotationAngle = 180.0f;
        hitbox = Hitbox(vec4(xstart, ystart, 96, 96));
        movementScript = &patterns::enemyMovementScript;
        shotPattern = &patterns::stage3ShotPattern;
    };

    ~EnemyStage3()
    {
        delete sprite;
    };
};



// ---------------------------------------------------------------------------------------------------------



class EnemyStage4 : public Enemy
{

public:

    EnemyStage4( SDL_Texture * tex, int xstart, int ystart, int spawn )
    {
        int maxhp = 300;
        initEnemy(0.0f, 1.0f, maxhp, spawn);

        // Hp threshold is 1/3 of max hp
        int hpt = (int)(maxhp * 0.3f);

        addVar("m_HPThreshold_1", 2 * hpt);
        addVar("m_HPThreshold_2", hpt);
        addVar("m_maxhp", maxhp);

        addVar("m_shotTimerTargeted", 0);
        addVar("m_shotTimerStraightArray", 0);
        addVar("m_shotTimerDelayed", 0);
        addVar("m_shotTimerRandom", 0);

        setSprite(tex, 96, 96);
        sprite->rotationAngle = 180.0f;
        hitbox = Hitbox(vec4(xstart, ystart, 96, 96));
        movementScript = &patterns::enemyMovementScript;
        shotPattern = &patterns::stage4ShotPattern;
    };

    ~EnemyStage4()
    {
        delete sprite;
    };
};

#endif // __ENEMYOBJECT_H_
