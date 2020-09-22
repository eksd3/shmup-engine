//


#include "gameobject.h"


GameObject::GameObject()
{
    m_iWinW = 800;
    m_iWinH = 600;
    m_sdl = new SDL( m_iWinW, m_iWinH, SDL_INIT_VIDEO );

    m_shotController = new ShotController(m_sdl);
    m_shotController->setMaxBullets(1000);
    m_shotController->setbSmallTex(m_sdl->loadTexture("sprites/bullet.png"));

    m_enemyController = new EnemyController(m_sdl);

    m_stageController = new StageController(m_sdl, m_sdl->loadTexture("sprites/bg.png"), false, false, 3, 0);

    m_keyboardState = SDL_GetKeyboardState(NULL);

    m_player = new Player(vec2(400, 500), vec2(32, 32), 4);
    m_player->setSprite(m_sdl->loadTexture("sprites/ship.png"), 32, 32);
    m_sdl->addSpriteLayer(m_player->getSprite());

    m_sdl->addText("Score: %s", std::to_string(m_score), "sample.ttf", 20, 10, m_iWinH-40, FC_MakeColor(0, 255, 255, 255));
    m_sdl->addText("Level: %s", std::to_string(m_level), "sample.ttf", 20, m_iWinW-100, m_iWinH-40, FC_MakeColor(255, 0, 0, 255));
    m_run = true;
    m_score = 0;
    m_level = 0;

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
    SDL_RenderClear(m_sdl->getRenderer());
    m_sdl->draw();
    SDL_RenderPresent(m_sdl->getRenderer());

    m_stageController->update();
    m_shotController->update();
    m_enemyController->update();

    m_player->incrementTimer(FRAME_DELAY);
    m_player->getSprite()->gotoIter(0, 0);

    enemy_explode();


    if (m_enemyController->getNumOfUnspawned() == 0 && m_enemyController->getNumOfSpawned() == 0)
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

    if (m_keyboardState[SDL_SCANCODE_J])
        patterns::shot_pattern(m_player, m_shotController);
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

    int iNewScore = m_score;
    BulletEnemyPair enbl;
    enbl = grotto::enemyHit(m_enemyController->getSpawned(), m_shotController->getFriendlyBullets());
    for (Enemy * e : enbl.m_ven)
    {
        if (!e->isVulnerable())
            continue;

        e->setHP(e->getHP() - 1);
        if (e->getHP() <= 0)
        {
            iNewScore += 1;

            e->getSprite()->gotoIter(0, 1);
            e->resetTimer();
            m_enemyController->removeEnemy(e);
            m_destroyed.push_back(e);
        }
        else
        {
            e->getSprite()->gotoIter(3 - e->getHP(), 0);
        }
    }

    for (Bullet * b : enbl.m_vbl)
        m_shotController->destroyFriendlyBullet(b);

    if (iNewScore != m_score)
    {
        m_score = iNewScore;
        m_sdl->getOverlayByFormattedText("Score: %s")->setText(std::to_string(iNewScore));
    }
}

void GameObject::enemy_explode()
{
    for (Enemy * en : m_destroyed)
    {
        if (en->getTimer() > 500)
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
