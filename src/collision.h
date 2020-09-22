#ifndef __GRT_COLLISION_H_
#define __GRT_COLLISION_H_

#include "character.h"
#include "shot.h"
#include "grotto_def.h"

namespace grotto
{


struct BulletEnemyPair
{
    // Holds bullet and enemy vectors
    // Used for enemyHit
    std::vector<Bullet*>    m_vbl;
    std::vector<Enemy*>     m_ven;
};


bool vec4_intersect(vec4 v1, vec4 v2);

// Returns true if the player's hitbox intersects with an enemy's sprite or a
// bullet's sprite
bool playerHit(Player * p, std::vector<Enemy*> ven, std::vector<Bullet*> vbl);

// Returns a type containing a vector of bullets that made contact and
// another vector of enemies that were hit
BulletEnemyPair enemyHit(const std::vector<Enemy*>& ven, const std::vector<Bullet*>& vbl);

}

#endif // __GRT_COLLISION_H_
