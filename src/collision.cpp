#include "collision.h"


bool grotto::vec4_intersect(vec4 v1, vec4 v2)
{
    // returns true if x intersects with y
    return (    (v1.x < v2.x + v2.w && v1.y < v2.y + v2.w) &&
                (v1.x + v1.w > v2.x && v1.y + v1.h > v2.y) );
}

bool grotto::playerHit(Player * p, std::vector<Enemy*> ven, std::vector<Bullet*> vbl)
{
    // Returns true of the player is hit

    vec4 p_rect = p->getHitbox()->getRect();
    vec4 e_rect;

    for (Enemy * en : ven)
    {
        // Check for collisions with enemy sprites
        e_rect = en->getHitbox()->getRect();
        if (vec4_intersect(p_rect, e_rect))
            return true;
    }

    for (Bullet * bl : vbl)
    {
        // Check for collisions with enemy bullets
        e_rect = bl->getSprite()->getRect();
        if (vec4_intersect(p_rect, e_rect))
            return true;
    }

    return false;
}

BulletEnemyPair grotto::enemyHit(const std::vector<Enemy*>& ven, const std::vector<Bullet*>& vbl)
{
    // Returns a type containing a vector of bullets that made contact and
    // another vector of enemies that were hit

    vec4 e_rect;
    vec4 b_rect;
    BulletEnemyPair hit;

    for (Enemy * en : ven)
    {
        e_rect = en->getHitbox()->getRect();
        for (Bullet * bl : vbl)
        {
            b_rect = bl->getSprite()->getRect();
            if (vec4_intersect(e_rect, b_rect))
            {
                hit.m_ven.push_back(en);
                hit.m_vbl.push_back(bl);
            }
        }
    }
    return hit;
}
