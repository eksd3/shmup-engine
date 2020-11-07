// Shot and movement patterns
//
// The first boss has a base shot pattern and every boss after
// the first has the previous boss' shot pattern with some added on
// top of it so I just copied the previous boss' code for each subsequent
// boss it's dumb but idgaf


#include "patterns.h"


// ---------------------------------------------------------------------------------------------------------


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


// ---------------------------------------------------------------------------------------------------------


void patterns::shot_pattern(Character * src, ShotController * shc)
{
    // Fires a spray of bullets in an arc once every 150 milliseconds
    if (shc->exceededSpriteLimit())
        return;

    int iDelayShot = 150;

    if (src->getTimer() >= iDelayShot)
    {
        int iNumBullets = 5;
        int iAngleIncrement = 60;

        // Source is the center of the player's sprite
        vec2 vSrcPos = src->getSprite()->getCenter();

        // Target is the top of the screen
        vec2 vTarg = vec2(vSrcPos.x, 0);

        // Move target's x coordinate by 150 times the percentage of the
        // players y position relative to the bottom of the screen
        vTarg.x -= ((int)(iNumBullets * iAngleIncrement) * 0.5f) * (vSrcPos.y / 600.0f);

        // Fire bullets
        for (int i = 1; i <= iNumBullets; ++i)
        {
            vTarg.x += iAngleIncrement * (vSrcPos.y / 600.0f);
            shc->createBullet(vSrcPos, vTarg, 3, 10, true, shc->getbSmallTex());
        }

        src->resetTimer();
    }
}


// ---------------------------------------------------------------------------------------------------------


void patterns::stage1ShotPattern(Enemy * src, Character * t, ShotController * shc)
{
    if (src->getState() != 1)
        return;

    int iTimerTarg = src->getVar("m_shotTimerTargeted");
    int iTimerStraight = src->getVar("m_shotTimerStraightArray");

    // Delay between shots
    int iDelayFirst = 750;
    int iDelaySecond = 1000;

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
        iDelayFirst = 450;
        iDelaySecond = 800;
        iVelocityTargeted = 7;
        iVelocityStraight = 12;
    }

    // Targeted shots
    if (iTimerTarg >= iDelayFirst)
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

        // Reset the timer
        src->setVar("m_shotTimerTargeted", 0);
    }

    // Straight shots
    if (iTimerStraight >= iDelaySecond)
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

        src->setVar("m_shotTimerStraightArray", 0);
    }

    // Increment both timers
    iTimerTarg = src->getVar("m_shotTimerTargeted");
    iTimerStraight = src->getVar("m_shotTimerStraightArray");
    src->setVar("m_shotTimerTargeted", iTimerTarg + FRAME_DELAY);
    src->setVar("m_shotTimerStraightArray", iTimerStraight + FRAME_DELAY);
}



// ---------------------------------------------------------------------------------------------------------


void patterns::stage2ShotPattern(Enemy * src, Character * t, ShotController * shc)
{
    if (src->getState() != 1)
        return;

    int iTimerTarg = src->getVar("m_shotTimerTargeted");
    int iTimerStraight = src->getVar("m_shotTimerStraightArray");
    int iTimerDelayed = src->getVar("m_shotTimerDelayed");

    // Delay between shots
    int iDelayTarg = 800;
    int iDelayArray = 1200;
    int iDelayDelayed = 500;

    // Bullet velocity
    int iVelocityTargeted = 6;
    int iVelocityStraight = 10;
    int iVelocityDelayedFirst = 2;

    // Bullet radius
    int iRadiusTargeted = 3;
    int iRadiusStright = 6;
    int iRadiusDelayed = 7;

    // Number of bullets fired (applies to the straight shot (blue-red))
    int iNumStraight = 1;

    bool bTresh2 =  src->getHP() < src->getVar("m_HPThreshold_1");
    bool bTresh3 =  src->getHP() < src->getVar("m_HPThreshold_2");

    std::vector<Bullet*> DelayedBullets;

    if (bTresh2)
    {
        // If the boss has 2/3 hp or less
        iNumStraight = 2;
        iDelayDelayed = 400;
    }

    if (bTresh3)
    {
        // If the boss has 1/3 hp or less
        iDelayTarg = 700;
        iDelayArray = 1000;
        iVelocityTargeted = 7;
        iVelocityStraight = 12;
    }

    // Targeted shots
    if (iTimerTarg >= iDelayTarg)
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

        // Reset the timer
        src->setVar("m_shotTimerTargeted", 0);
    }

    // Straight shots
    if (iTimerStraight >= iDelayArray)
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

        src->setVar("m_shotTimerStraightArray", 0);
    }

    // Delayed shots
    if (iTimerDelayed >= iDelayDelayed)
    {
        vec2 vSrc = src->getSprite()->getCenter();
        vec2 vTarg = vSrc;
        vTarg.y += src->getSprite()->rect.h + 50;

        int direction = rand() % 2;

        if (direction == 0) vTarg.x = vSrc.x - (src->getSprite()->rect.w + 50);
        else vTarg.x = vSrc.x + (src->getSprite()->rect.w + 50);

        SDL_Texture * tBulletTex = shc->getbLargeTex();

        shc->fireTargeted(vSrc, vTarg, iVelocityDelayedFirst, iRadiusDelayed, false, tBulletTex);

        src->setVar("m_shotTimerDelayed", 0);
    }

    // Increment both timers
    iTimerTarg = src->getVar("m_shotTimerTargeted");
    iTimerStraight = src->getVar("m_shotTimerStraightArray");
    iTimerDelayed = src->getVar("m_shotTimerDelayed");

    src->setVar("m_shotTimerTargeted", iTimerTarg + FRAME_DELAY);
    src->setVar("m_shotTimerStraightArray", iTimerStraight + FRAME_DELAY);
    src->setVar("m_shotTimerDelayed", iTimerDelayed + FRAME_DELAY);
}



// ---------------------------------------------------------------------------------------------------------



void patterns::stage3ShotPattern(Enemy * src, Character * t, ShotController * shc)
{
    if (src->getState() != 1)
        return;

    int iTimerTarg = src->getVar("m_shotTimerTargeted");
    int iTimerStraight = src->getVar("m_shotTimerStraightArray");
    int iTimerDelayed = src->getVar("m_shotTimerDelayed");
    int iTimerRandom = src->getVar("m_shotTimerRandom");

    // Delay between shots
    int iDelayTarg = 800;
    int iDelayArray = 1200;
    int iDelayDelayed = 500;
    int iDelayRandom = 100;

    // Bullet velocity
    int iVelocityTargeted = 6;
    int iVelocityStraight = 10;
    int iVelocityDelayedFirst = 2;

    int iMinVelocityRandom = 5;
    int iMaxVelocityRandom = 7;

    // Bullet radius
    int iRadiusTargeted = 3;
    int iRadiusStright = 6;
    int iRadiusDelayed = 7;

    // Number of bullets fired (applies to the straight shot (blue-red))
    int iNumStraight = 1;

    bool bTresh2 =  src->getHP() < src->getVar("m_HPThreshold_1");
    bool bTresh3 =  src->getHP() < src->getVar("m_HPThreshold_2");

    std::vector<Bullet*> DelayedBullets;

    if (bTresh2)
    {
        // If the boss has 2/3 hp or less
        iNumStraight = 2;
        iDelayDelayed = 400;
        iVelocityDelayedFirst = 3;

        iMinVelocityRandom = 6;
        iMaxVelocityRandom = 8;
    }

    if (bTresh3)
    {
        // If the boss has 1/3 hp or less
        iDelayTarg = 700;
        iDelayArray = 1000;
        iDelayDelayed = 350;
        iVelocityTargeted = 7;
        iVelocityStraight = 12;
    }

    // Targeted shots
    if (iTimerTarg >= iDelayTarg)
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

        // Reset the timer
        src->setVar("m_shotTimerTargeted", 0);
    }

    // Straight shots
    if (iTimerStraight >= iDelayArray)
    {
        vec2 vSrc = src->getSprite()->getPos();
        vec4 vSprDim = src->getSprite()->rect;

        // Source is the bottom-middle of the sprite
        vSrc.y += vSprDim.h;

        // If hp is less than 2/3 the sprite is red (bLargeTex), else it's blue (bMediumTex)
        SDL_Texture * tBulletTex = shc->getbMediumTex();
        if (bTresh2)
            tBulletTex = shc->getbLargeTex();

        shc->fireStraightArray( vSrc, GROTTO_DIR_DOWN, vSprDim.w, iNumStraight, iVelocityStraight, iRadiusStright, false, tBulletTex);

        src->setVar("m_shotTimerStraightArray", 0);
    }

    // Delayed shots
    if (iTimerDelayed >= iDelayDelayed)
    {
        vec2 vSrc = src->getSprite()->getCenter();
        vec2 vTarg = vSrc;
        vTarg.y += src->getSprite()->rect.h + 50;

        int direction = rand() % 2;

        if (direction == 0) vTarg.x = vSrc.x - (src->getSprite()->rect.w + 50);
        else vTarg.x = vSrc.x + (src->getSprite()->rect.w + 50);

        SDL_Texture * tBulletTex = shc->getbLargeTex();

        shc->fireTargeted(vSrc, vTarg, iVelocityDelayedFirst, iRadiusDelayed, false, tBulletTex);


        src->setVar("m_shotTimerDelayed", 0);
    }

    // Random shots
    if (iTimerRandom >= iDelayRandom)
    {
        vec2 vSrc = src->getSprite()->getCenter();
        vec2 vTarg = vec2(800, vSrc.y);

        // Random angle int multiples of 30
        int iRand = rand() % 12;
        iRand *= 30;
        iRand = iRand * MATH_PI / 180.0f;

        vTarg.x *= cos(iRand);
        vTarg.y *= sin(iRand);

        // fire half the bullets downward
        iRand = rand() % 2;
        if (iRand == 0) vTarg.y = 600;

        // Determine velocity
        int iVel;
        iVel = rand() % (iMinVelocityRandom - iMaxVelocityRandom) + 1;
        iVel += iMinVelocityRandom;

        SDL_Texture * tBulletTex = shc->getbLargeTex();

        shc->fireTargeted(vSrc, vTarg, iVel, iRadiusDelayed, false, tBulletTex);

        src->setVar("m_shotTimerRandom", 0);
    }

    // Increment both timers
    iTimerTarg = src->getVar("m_shotTimerTargeted");
    iTimerStraight = src->getVar("m_shotTimerStraightArray");
    iTimerDelayed = src->getVar("m_shotTimerDelayed");
    iTimerRandom = src->getVar("m_shotTimerRandom");

    src->setVar("m_shotTimerTargeted", iTimerTarg + FRAME_DELAY);
    src->setVar("m_shotTimerStraightArray", iTimerStraight + FRAME_DELAY);
    src->setVar("m_shotTimerDelayed", iTimerDelayed + FRAME_DELAY);
    src->setVar("m_shotTimerRandom", iTimerRandom + FRAME_DELAY);
}



// ---------------------------------------------------------------------------------------------------------



void patterns::stage4ShotPattern(Enemy * src, Character * t, ShotController * shc)
{
    if (src->getState() != 1)
        return;

    int iTimerTarg = src->getVar("m_shotTimerTargeted");
    int iTimerStraight = src->getVar("m_shotTimerStraightArray");
    int iTimerDelayed = src->getVar("m_shotTimerDelayed");
    int iTimerRandom = src->getVar("m_shotTimerRandom");

    // Delay between shots
    int iDelayTarg = 800;
    int iDelayArray = 1200;
    int iDelayDelayed = 500;
    int iDelayRandom = 100;

    // Bullet velocity
    int iVelocityTargeted = 6;
    int iVelocityStraight = 10;
    int iVelocityDelayedFirst = 2;

    int iMinVelocityRandom = 5;
    int iMaxVelocityRandom = 7;

    // Bullet radius
    int iRadiusTargeted = 3;
    int iRadiusStright = 6;
    int iRadiusDelayed = 7;

    // Number of bullets fired (applies to the straight shot (blue-red))
    int iNumStraight = 3;

    bool bTresh2 =  src->getHP() < src->getVar("m_HPThreshold_1");
    bool bTresh3 =  src->getHP() < src->getVar("m_HPThreshold_2");

    if (bTresh2)
    {
        // If the boss has 2/3 hp or less
        iNumStraight = 4;
        iDelayDelayed = 250;
        iDelayRandom = 50;

        iVelocityDelayedFirst = 3;

        iMinVelocityRandom = 6;
        iMaxVelocityRandom = 8;
    }

    if (bTresh3)
    {
        // If the boss has 1/3 hp or less
        iDelayTarg = 670;
        iDelayArray = 1000;
        iDelayDelayed = 200;
        iDelayRandom = FRAME_DELAY;

        iVelocityTargeted = 7;
        iVelocityStraight = 12;
    }

    // Targeted shots
    if (iTimerTarg >= iDelayTarg)
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

        // Reset the timer
        src->setVar("m_shotTimerTargeted", 0);
    }

    // Straight shots
    if (iTimerStraight >= iDelayArray)
    {
        vec2 vSrc = src->getSprite()->getPos();
        vec4 vSprDim = src->getSprite()->rect;

        // Source is the bottom-center of the sprite
        vSrc.y += vSprDim.h;

        // If hp is less than 2/3 the sprite is red (bLargeTex), else it's blue (bMediumTex)
        if (bTresh2)
           shc->fireStraightArray(
                vSrc, GROTTO_DIR_DOWN, vSprDim.w, iNumStraight, iVelocityStraight, iRadiusStright, false, shc->getbMediumTex());
        else
            shc->fireStraightArray(
                vSrc, GROTTO_DIR_DOWN, vSprDim.w, iNumStraight, iVelocityStraight, iRadiusStright, false, shc->getbLargeTex());

        src->setVar("m_shotTimerStraightArray", 0);
    }

    // Delayed shots
    if (iTimerDelayed >= iDelayDelayed)
    {
        vec2 vSrc = src->getSprite()->getCenter();
        vec2 vTarg = vSrc;
        vTarg.y += src->getSprite()->rect.h + 50;

        int direction = rand() % 2;

        if (direction == 0) vTarg.x = vSrc.x - (src->getSprite()->rect.w + 50);
        else vTarg.x = vSrc.x + (src->getSprite()->rect.w + 50);

        shc->fireTargeted(vSrc, vTarg, iVelocityDelayedFirst, iRadiusDelayed, false, shc->getbMediumTex());


        src->setVar("m_shotTimerDelayed", 0);
    }

    // Random shots
    if (iTimerRandom >= iDelayRandom)
    {
        vec2 vSrc = src->getSprite()->getCenter();
        vec2 vTarg = vec2(800, vSrc.y);

        // Random angle in multiples of 30
        int iRand = rand() % 12;
        iRand *= 30;
        iRand = iRand * MATH_PI / 180.0f;

        vTarg.x *= cos(iRand);
        vTarg.y *= sin(iRand);

        // fire half the bullets downward
        iRand = rand() % 2;
        if (iRand == 0) vTarg.y = 600;

        // Determine velocity
        int iVel;
        iVel = rand() % (iMinVelocityRandom - iMaxVelocityRandom) + 1;
        iVel += iMinVelocityRandom;

        shc->fireTargeted(vSrc, vTarg, iVel, iRadiusDelayed, false, shc->getbLargeTex());

        src->setVar("m_shotTimerRandom", 0);
    }

    // Increment both timers
    iTimerTarg = src->getVar("m_shotTimerTargeted");
    iTimerStraight = src->getVar("m_shotTimerStraightArray");
    iTimerDelayed = src->getVar("m_shotTimerDelayed");
    iTimerRandom = src->getVar("m_shotTimerRandom");

    src->setVar("m_shotTimerTargeted", iTimerTarg + FRAME_DELAY);
    src->setVar("m_shotTimerStraightArray", iTimerStraight + FRAME_DELAY);
    src->setVar("m_shotTimerDelayed", iTimerDelayed + FRAME_DELAY);
    src->setVar("m_shotTimerRandom", iTimerRandom + FRAME_DELAY);
}
