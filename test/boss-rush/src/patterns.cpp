//


#include "patterns.h"


void patterns::enemyMovementScript(Enemy * en)
{
    float fMaxVel = 2.0f;

    switch (en->getState())
    {

    case -1:

        // Enemy is invulnerable while entering the scene
        en->setVulnerability(false);
        en->moveCharacter(GROTTO_DIR_DOWN);

        if (en->getSprite()->rect.y >= 96)
        {
            en->setState(0);
            en->resetTimer();
        }

        break;

    case 0:

        // Stays still for 1 second and starts moving left
        if (en->getTimer() > 1000)
        {
            en->setVulnerability(true);
            en->setXVelocity(-fMaxVel);
            en->setYVelocity(0.0f);
            en->setState(1);
        }

        break;

    case 1:

        // Moves horizontally and when it reaches a certain point on the screen
        // starts accelerating towards the other direction
        en->moveCharacter(GROTTO_DIR_RIGHT);

        float fDistx = en->getHitbox()->getCenter().x;
        float fXvel = en->getXVelocity();
        int iHorizDistMargin = 200;
        float fVelIncrement = 0.05f;

        if ( fDistx < iHorizDistMargin )
        {
            if ( fXvel <= fMaxVel )
            {
                en->setXVelocity( fXvel + fVelIncrement );
            }

            break;
        }

        else if ( fDistx > 800 - iHorizDistMargin )
        {
            if ( fXvel >= -fMaxVel )
            {
                en->setXVelocity( fXvel - fVelIncrement );
            }

            break;
        }

        else
        {
            if ( fXvel > 0.0f )
            {
                en->setXVelocity( fMaxVel );
            }
            else if ( fXvel < 0.0f )
            {
                en->setXVelocity( -fMaxVel );
            }
        }

        break;
    }
}

void patterns::shot_pattern(Character * src, ShotController * shc)
{
    // Fires a spray of bullets in an arc once every 150 milliseconds
    if (shc->exceededSpriteLimit())
        return;

    if (src->getTimer() > 150)
    {
        // Source is the center of the player's sprite
        vec2 vSrcPos = src->getSprite()->getCenter();

        // Target is the top of the screen
        vec2 vTarg = vec2(vSrcPos.x, 0);

        // Move target's x coordinate by 150 times the percentage of the
        // players y position relative to the bottom of the screen
        vTarg.x -= 150 * (vSrcPos.y / 600.0f);

        // Fire 5 bullets
        for (int i = 1; i <= 5; ++i)
        {
            vTarg.x += 60 * (vSrcPos.y / 600.0f);
            shc->createBullet(vSrcPos, vTarg, 3, 10, true, shc->getbSmallTex());
        }

        src->resetTimer();
    }
}

void patterns::enemyShotPattern(Enemy * src, Character * t, ShotController * shc)
{
    int iTimer = src->getShotTimer();

    // Delay between shots
    int iDelayFirst = 3;
    int iDelaySecond = 5;

    // Bullet velocity
    int iVelocityTargeted = 6;
    int iVelocityStraight = 10;

    // Bullet radius
    int iRadiusTargeted = 3;
    int iRadiusStright = 6;

    // Number of bullets fired (applies to the straight shot (blue-red))
    int iNumStraight = 3;

    bool bTresh2 =  src->getHP() < src->getVar("m_HPThreshold_1");
    bool bTresh3 =  src->getHP() < src->getVar("m_HPThreshold_2");

    if (bTresh2)
    {
        // If the boss has 2/3 hp or less
        iNumStraight = 4;
    }

    if (bTresh3)
    {
        // If the boss has 1/3 hp or less
        iDelayFirst = 2;
        iDelaySecond = 3;
        iVelocityTargeted = 7;
        iVelocityStraight = 12;
    }

    // Targeted shots
    if (iTimer % (iDelayFirst * FRAME_DELAY) == 0)
    {
        vec2 vTarg = t->getHitbox()->getCenter();
        vec2 vSrc;

        // If hp is below 2/3 fire another shot directly at the player
        if (bTresh2)
        {
            vSrc = src->getHitbox()->getCenter();
            vSrc.y += (int) (src->getHitbox()->rect.h * 0.5f);
            shc->fireTargeted(vSrc, vTarg, iVelocityTargeted + 2, iRadiusTargeted + 3, false, shc->getbMediumTex());
        }

        // Fire a shot a 20 units to the left of the player
        // from the top-left of the sprite
        vSrc = src->getSprite()->getPos();
        vTarg.x -= 20;
        shc->fireTargeted(vSrc, vTarg, iVelocityTargeted, iRadiusTargeted, false, shc->getbSmallTex());

        // Fire a shot 20 units to the right of the player
        // from the top-right of the sprite
        vSrc.x += src->getSprite()->rect.w;
        vTarg.x += 40;
        shc->fireTargeted(vSrc, vTarg, iVelocityTargeted, iRadiusTargeted, false, shc->getbSmallTex());
    }

    // Straight shots
    if (iTimer % (iDelaySecond * FRAME_DELAY) == 0)
    {
        vec2 vSrc = src->getSprite()->getPos();
        vec4 vSprDim = src->getSprite()->rect;

        // Source is the bottom-middle of the sprite
        vSrc.y += vSprDim.h;

        // If hp is less than 2/3 the sprite is red (bLargeTex), else it's blue (bMediumTex)
        SDL_Texture * tBulletTex = shc->getbMediumTex();
        if (bTresh2)
        {
            tBulletTex = shc->getbLargeTex();
        }
        shc->fireStraightArray( vSrc, GROTTO_DIR_DOWN, vSprDim.w, iNumStraight, iVelocityStraight, iRadiusStright, false, tBulletTex);
    }

    if (iTimer >= INT_MAX - FRAME_DELAY)
    {
        src->resetTimer();
    }
}
