#ifndef __GAMEOBJECT_H_
#define __GAMEOBJECT_H_

#include <time.h>
#include "GROTTO.h"

#include "patterns.h"
#include "enemyobject.h"

class GameObject
{
private:

    int     m_iWinW;
    int     m_iWinH;
    SDL *   m_sdl;

    ShotController *    m_shotController;
    EnemyController *   m_enemyController;
    StageController *   m_stageController;

    Player *    m_player;
    int         m_score;
    bool        m_run;
    vec2        m_movementDir;

    std::vector<Enemy*> m_destroyed;

    const Uint8 *   m_keyboardState;
    SDL_Event       m_ev;
    Uint32          m_frameStart;
    int             m_frameTime;

public:
    GameObject();
    ~GameObject();

    bool is_running() const { return m_run; };
    void update();
    void handle_events();
    void delay_frame();

    void enemy_shots();
    void check_collision();
    void set_movementDir();
    void enemy_explode();
};


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

    m_player = new Player(vec2(400, 500), vec2(32, 32), 5);
    m_player->setSprite(m_sdl->loadTexture("sprites/ship.png"), 32, 32);
    m_sdl->addSpriteLayer(m_player->getSprite());

    m_movementDir = GROTTO_DIR_LEFT;
    SDL_Texture * enemyTex = m_sdl->loadTexture("sprites/enemy_ship.png");
    for (int i=1; i<=10; ++i)
    {
        for (int j=1; j<=4; ++j)
        {
            EnemyNormal * en = new EnemyNormal(enemyTex, 64 * i, 64 * j, 0);
            m_enemyController->add(en);
        }
    }

    m_sdl->addText("Score: %s", std::to_string(m_score), "sample.ttf", 20, 10, 10, FC_MakeColor(255, 255, 255, 255));
    m_run = true;
    m_score = 0;

    srand(time(NULL));
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
    set_movementDir();


    if (m_enemyController->getNumOfSpawned() == 0)
    {
        m_run = false;
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
    if (grotto::playerHit(m_player, m_enemyController->getSpawned(), m_shotController->getEnemyBullets()))
        m_run = false; // TODO

    int iNewScore = m_score;
    BulletEnemyPair enbl;
    enbl = grotto::enemyHit(m_enemyController->getSpawned(), m_shotController->getFriendlyBullets());
    for (Enemy * e : enbl.m_ven)
    {
        e->setHP(e->getHP() - 1);
        if (e->getHP() <= 0)
        {
            iNewScore += 1;

            e->getSprite()->nextIter();
            e->resetTimer();
            m_enemyController->removeEnemy(e);
            m_destroyed.push_back(e);
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

void GameObject::set_movementDir()
{
    if (m_movementDir == GROTTO_DIR_LEFT)
    {
        int tresh = m_iWinW;
        for (Enemy * en : m_enemyController->getSpawned())
        {
            int pos = en->getSprite()->getPos().x;
            if (pos < tresh)
            {
                tresh = pos;
            }
        }
        if (tresh <= 32)
        {
            for (Enemy * en : m_enemyController->getSpawned())
                en->addVar("m_movementDirection", 1);
            m_movementDir = GROTTO_DIR_RIGHT;
        }
    }
    else
    {
        int tresh = 0;
        for (Enemy * en : m_enemyController->getSpawned())
        {
            int pos = en->getSprite()->getPos().x;
            if (pos > tresh)
                tresh = pos;
        }
        if (tresh >= m_iWinW - 64)
        {
            for (Enemy * en : m_enemyController->getSpawned())
                en->addVar("m_movementDirection", -1);
            m_movementDir = GROTTO_DIR_LEFT;
        }
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

#endif // __GAMEOBJECT_H_
