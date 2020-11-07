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
    int         m_time;
    bool        m_run;
    vec2        m_movementDir;
    int         m_level;
    int         m_maxLevel;
    bool        m_paused;

    std::vector<Enemy*> m_destroyed;

    const Uint8 *   m_keyboardState;
    SDL_Event       m_ev;
    Uint32          m_frameStart;
    int             m_frameTime;

public:
    GameObject();
    ~GameObject();

    int MainMenu(bool gameClear = false);

    bool is_running() const { return m_run; };
    void update();
    void handle_events();
    void delay_frame();

    void enemy_shots();
    void check_collision();
    void set_movementDir();
    void enemy_explode();

    void next_level();
    void init_level(int n);
    void reset_level();
};

#endif // __GAMEOBJECT_H_
