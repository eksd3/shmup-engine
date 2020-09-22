//


#include "patterns.h"


void patterns::enemyMovementScript(Enemy * en)
{

    switch (en->getState())
    {

    case -1:

        en->setVulnerability(false);
        en->moveCharacter(GROTTO_DIR_DOWN);
        if (en->getSprite()->rect.y >= 200)
        {
            en->setVulnerability(true);
            en->setState(0);
            en->resetTimer();
        }
        break;

    case 0:

        if (en->getTimer() > 1000)
        {
            en->setYVelocity(-4.0f);

            // If the enemy is positioned on the left side of the screen, jump right
            if (en->getHitbox()->getCenter().x < 400)       en->setXVelocity(0.5f);

            // Else jump left
            else if (en->getHitbox()->getCenter().x > 400)  en->setXVelocity(-0.5f);

            // Or, if in the center, jump a little bit to the right
            else en->setXVelocity(0.1f);

            en->setState(1);
        }
        break;

    case 1:

        en->moveCharacter(GROTTO_DIR_DOWN);
        en->moveCharacter(GROTTO_DIR_RIGHT);

        // If moving up
        if ( en->getYVelocity() <= 0.0f )
        {
            int mod = 1;

            // Accelerate in the horizontal direction you're moving
            if ( en->getXVelocity() < 0.0f ) mod = -1;
            en->setXVelocity( en->getXVelocity() + (0.05f * mod) );
        }

        // Accelerate downwards
        en->setYVelocity(en->getYVelocity() + 0.1f);

        // If you're off the screen
        if (en->getSprite()->rect.y > 600)
            en->setState(2);

        break;

    case 2:

        // If an enemy's off the screen warp it back up
        en->getHitbox()->setCenter( vec2(en->getHitbox()->getCenter().x, -32) );
        en->getSprite()->setCenter( vec2(en->getHitbox()->getCenter()) );
        en->setState(3);
        break;

    case 3:

        // Moving down into position after being warped to the
        // top of the screen
        en->moveCharacter(GROTTO_DIR_DOWN);

        // Can stop if it has reached at least 48 units down
        if (en->getHitbox()->rect.y > 48)
        {
            // 10% chance to stop (each frame) at any point between 48 and 200 units
            // Stops automatically when it reaches 200
            if (rand() % 10 == 0 || en->getHitbox()->rect.y >= 200)
            {
                en->setState(0);
                en->resetTimer();
            }
        }

        break;
    }
}

void patterns::shot_pattern(Character * src, ShotController * shc)
{
    // Fire once every 200 milliseconds
    if (src->getTimer() > 200)
    {
        vec2 vSrcPos = src->getSprite()->getPos();
        int iSrcW = src->getSprite()->getRect().w;

        shc->fireStraightArray(vSrcPos, GROTTO_DIR_UP, iSrcW, 1, 5, 3, true, shc->getbSmallTex());
        src->resetTimer();
    }
}

void patterns::enemyShotPattern(Enemy * src, Character * t, ShotController * shc)
{
    if (rand() % 400 == 0 && src->getHitbox()->getCenter().y < 300)
    {
        vec2 vSrc = src->getSprite()->getPos();
        vec2 vTarg = t->getHitbox()->getCenter();

        shc->fireTargeted(vSrc, vTarg, 5, 3, false, shc->getbSmallTex());
    }
}
