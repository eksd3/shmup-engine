#ifndef _PATTERNS_H_
#define _PATTERNS_H_

#include "character.h"


namespace patterns {

void move_alternate(Enemy * en)
{
    if (en->getVar("m_movementDirection") == -1)
    {
        en->moveCharacter(GROTTO_DIR_LEFT);
    }
    else
    {
        en->moveCharacter(GROTTO_DIR_RIGHT);
    }
}

void shot_pattern(Character * src, ShotController * shc)
{
    if (src->getTimer() > 200)
    {
        vec2 vSrcPos = src->getSprite()->getPos();
        int iSrcW = src->getSprite()->getRect().w;

        shc->fireStraightArray(vSrcPos, GROTTO_DIR_UP, iSrcW, 1, 5, 3, true, shc->getbSmallTex());
        src->resetTimer();
    }
}

void enemy_shot_pattern(Enemy * src, Character * t, ShotController * shc)
{
    if (rand() % 400 == 0)
    {
        shc->fireRandomArc(src->getHitbox()->getCenter(), t->getHitbox()->getCenter(), 60.0f, 5, 3, 3, false, shc->getbSmallTex());
    }

}

}
#endif // _PATTERNS_H
