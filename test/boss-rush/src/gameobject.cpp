/*

Game object class

*/


#include "gameobject.h"


GameObject::GameObject()
{
    // Window size
    m_iWinW = 800;
    m_iWinH = 600;

    // Init SDL
    m_sdl = new SDL( m_iWinW, m_iWinH, SDL_INIT_VIDEO );

    m_shotController = new ShotController(m_sdl);
    m_shotController->setMaxBullets(1000);

    // Set bullet sprites
    m_shotController->setbSmallTex(m_sdl->loadTexture("sprites/bullet.png"));
    m_shotController->setbMediumTex(m_sdl->loadTexture("sprites/bullet_med.png"));
    m_shotController->setbLargeTex(m_sdl->loadTexture("Sprites/bullet_large.png"));

    m_enemyController = new EnemyController(m_sdl);

    // Enemies despawn if they go 96 pixels off the screen
    m_enemyController->setBoundary(96);

    m_stageController = new StageController(m_sdl, m_sdl->loadTexture("sprites/bg.png"), false, false, 3, 0);

    m_keyboardState = SDL_GetKeyboardState(NULL);

    // Initialize the player character
    m_player = new Player(vec2(400, 500), vec2(32, 32), 7);
    m_player->setSprite(m_sdl->loadTexture("sprites/ship.png"), 32, 32);

    // Add player sprite to sdl's vector
    m_sdl->addSpriteLayer(m_player->getSprite());

    m_time = 0;
    m_level = 0;
    m_sdl->addText("Time: %s", std::to_string(m_time), "sample.ttf", 15, 10, m_iWinH-40, FC_MakeColor(0, 255, 255, 255));
    m_sdl->addText("Level: %s", std::to_string(m_level), "sample.ttf", 15, 10, m_iWinH-60, FC_MakeColor(255, 0, 0, 255));
    m_run = true;

    next_level();
}



GameObject::~GameObject()
{
    delete m_sdl;
    delete m_shotController;
    delete m_enemyController;
    delete m_stageController;
    delete m_player;
}


void GameObject::update()
{
    // Increment the timer
    m_time += FRAME_DELAY;

    SDL_RenderClear(m_sdl->getRenderer());

    m_sdl->draw();

    // Draw the boss hp rectangle
    int iMaxw = m_sdl->getWindowWidth() - 10;
    float fRatio = 0.0f;

    // Prevent index error
    if ( m_enemyController->getNumOfSpawned() != 0 )
    {
        // Calculate the percentage of health left
        float fHP = m_enemyController->getSpawned()[0]->getHP();
        float fMaxhp = m_enemyController->getSpawned()[0]->getVar("m_maxhp");
        fRatio = fHP / fMaxhp;
    }

    SDL_Rect r;
    r.x = 5;
    r.y = 5;
    r.w = fRatio * iMaxw;
    r.h = 10;

    // Draw green rectangle
    SDL_SetRenderDrawColor(m_sdl->getRenderer(), 0, 255, 0, 255);
    SDL_RenderFillRect(m_sdl->getRenderer(), &r);

    SDL_RenderPresent(m_sdl->getRenderer());

    m_stageController->update();
    m_shotController->update();
    m_enemyController->update();

    m_player->incrementTimer(FRAME_DELAY);

    // Set player sprite to 0. 0 (static sprite)
    m_player->getSprite()->gotoIter(0, 0);

    enemy_explode();

    // Update Time: text
    m_sdl->getOverlayByFormattedText("Time: %s")->setText(std::to_string(m_time));

    // Level is cleared when there's no more enemies left
    if (    m_enemyController->getNumOfUnspawned() == 0 &&
            m_enemyController->getNumOfSpawned() == 0 &&
            m_destroyed.size() == 0 )
    {
        next_level();
    }
}

void GameObject::handle_events()
{
    m_frameStart = SDL_GetTicks();

    SDL_PollEvent(&m_ev);
    if (m_ev.type == SDL_QUIT)
        m_run = false;

    SDL_PumpEvents();
    if (m_keyboardState[SDL_SCANCODE_ESCAPE])
        m_run = false;

    if (m_keyboardState[SDL_SCANCODE_S])
    {
        m_player->movePlayer(GROTTO_DIR_LEFT, m_iWinW, m_iWinH);
        m_player->getSprite()->gotoIter(2, 0);
    }

    if (m_keyboardState[SDL_SCANCODE_F])
    {
        m_player->movePlayer(GROTTO_DIR_RIGHT, m_iWinW, m_iWinH);
        m_player->getSprite()->gotoIter(1, 0);
    }

    if (m_keyboardState[SDL_SCANCODE_E])
    {
        m_player->movePlayer(GROTTO_DIR_UP, m_iWinW, m_iWinH);
    }

    if (m_keyboardState[SDL_SCANCODE_D])
    {
        m_player->movePlayer(GROTTO_DIR_DOWN, m_iWinW, m_iWinH);
    }

    if (m_keyboardState[SDL_SCANCODE_J])
    {
        patterns::shot_pattern(m_player, m_shotController);
    }
}

void GameObject::delay_frame()
{
    m_frameTime = SDL_GetTicks() - m_frameStart;
    if (FRAME_DELAY > m_frameTime)
    {
        SDL_Delay(FRAME_DELAY - m_frameTime);
    }
}

void GameObject::enemy_shots()
{
    for (Enemy * en : m_enemyController->getSpawned())
    {
        en->execShotScript(m_player, m_shotController);
    }
}

void GameObject::check_collision()
{
    // The player is always vulnerable so there's no need to perform a check
    if (grotto::playerHit(m_player, m_enemyController->getSpawned(), m_shotController->getEnemyBullets()))
        m_run = false; // TODO

    BulletEnemyPair enbl;
    enbl = grotto::enemyHit(m_enemyController->getSpawned(), m_shotController->getFriendlyBullets());
    for (Enemy * e : enbl.m_ven)
    {
        if (!e->isVulnerable())
            continue;

        e->setHP(e->getHP() - 1);
        if (e->getHP() <= 0)
        {
            e->getSprite()->gotoIter(0, 1);
            e->resetTimer();
            m_enemyController->removeEnemy(e);
            m_destroyed.push_back(e);
        }
        else
        {
            // Set boss' sprite based off of how much health he has left
            if (e->getHP() > e->getVar("m_HPThreshold_1"))
            {
                e->getSprite()->gotoIter(0, 0);
            }
            else if (e->getHP() > e->getVar("m_HPThreshold_2"))
            {
                e->getSprite()->gotoIter(1, 0);
            }
            else
            {
                e->getSprite()->gotoIter(2, 0);
            }
        }
    }

    // Destroy all bullets that have collided with an enemy
    for (Bullet * b : enbl.m_vbl)
        m_shotController->destroyFriendlyBullet(b);
}


void GameObject::enemy_explode()
{
    // Set the enemy's sprite to the explosion strip
    // After the strip reaches the end remove the sprite from sdl's vector
    for (Enemy * en : m_destroyed)
    {
        // Change the sprite after 1 second
        if (en->getTimer() > 1000)
        {
            if (en->getSprite()->nextIter() != 0)
            {
                m_sdl->destroySpriteLayer(en->getSprite());
                grotto::removeFromSTDVector(m_destroyed, en);
            }
            en->resetTimer();
        }
        en->incrementTimer(FRAME_DELAY);
    }
}
