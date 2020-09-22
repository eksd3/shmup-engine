#ifndef __ENEMYOBJECT_H_
#define __ENEMYOBJECT_H_

#include "GROTTO.h"
#include "patterns.h"

class EnemyNormal : public Enemy
{
public:
    EnemyNormal( SDL_Texture * tex, int xstart, int ystart, int spawn);
    ~EnemyNormal();
};

EnemyNormal::EnemyNormal(SDL_Texture * tex, int xstart, int ystart, int spawn)
{
    initEnemy(1.0f, 1.0f, 1, spawn);

    setSprite(tex, 32, 32);
    sprite->rotationAngle = 180.0;
    hitbox = Hitbox(vec4(xstart, ystart, 32, 32));
    movementScript = &patterns::move_alternate;
    shotPattern = &patterns::enemy_shot_pattern;
    addVar("m_movementDirection", -1);

}

#endif // __ENEMYOBJECT_H_
