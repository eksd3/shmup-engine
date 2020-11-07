#ifndef _PATTERNS_H_
#define _PATTERNS_H_

#include "character.h"

namespace patterns {

    void enemyMovementScript(Enemy * en);

    void shot_pattern(Character * src, ShotController * shc);

    void stage1ShotPattern(Enemy * src, Character * t, ShotController * shc);

    void stage2ShotPattern(Enemy * src, Character * t, ShotController * shc);

    void stage3ShotPattern(Enemy * src, Character * t, ShotController * shc);

    void stage4ShotPattern(Enemy * src, Character * t, ShotController * shc);
}

#endif // _PATTERNS_H
