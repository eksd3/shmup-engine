#ifndef __ENEMYOBJECT_H_
#define __ENEMYOBJECT_H_

#include "GROTTO.h"
#include "patterns.h"


class EnemyNormal : public Enemy
{

public:
    EnemyNormal( SDL_Texture * tex, int xstart, int ystart, int spawn )
    {
        initEnemy(0.0f, 1.0f, 3, spawn);

        state = -1;
        setSprite(tex, 32, 32);
        sprite->rotationAngle = 180.0f;
        hitbox = Hitbox(vec4(xstart, ystart, 32, 32));
        movementScript = &patterns::enemyMovementScript;
        shotPattern = &patterns::enemyShotPattern;
    };

    ~EnemyNormal()
    {
        delete sprite;
    };

};

#endif // __ENEMYOBJECT_H_
