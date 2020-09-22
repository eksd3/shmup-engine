#ifndef __ENEMYOBJECT_H_
#define __ENEMYOBJECT_H_

#include "GROTTO.h"
#include "patterns.h"

class EnemyNormal : public Enemy
{

public:

    EnemyNormal( SDL_Texture * tex, int xstart, int ystart, int spawn )
    {
        int maxhp = 400;
        initEnemy(0.0f, 1.0f, maxhp, spawn);

        // Hp threshold is 1/3 of max hp
        int hpt = (int)(maxhp * 0.3f);

        addVar("m_HPThreshold_1", 2 * hpt);
        addVar("m_HPThreshold_2", hpt);
        addVar("m_maxhp", maxhp);

        setSprite(tex, 96, 96);
        sprite->rotationAngle = 180.0f;
        hitbox = Hitbox(vec4(xstart, ystart, 96, 96));
        movementScript = &patterns::enemyMovementScript;
        shotPattern = &patterns::enemyShotPattern;
    };

    ~EnemyNormal()
    {
        delete sprite;
    };

};

#endif // __ENEMYOBJECT_H_
