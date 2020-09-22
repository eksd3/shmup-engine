#ifndef _PATTERNS_H_
#define _PATTERNS_H_

#include "character.h"

namespace patterns {

    void enemyMovementScript(Enemy * en);

    void shot_pattern(Character * src, ShotController * shc);

    void enemyShotPattern(Enemy * src, Character * t, ShotController * shc);

}

#endif // _PATTERNS_H
