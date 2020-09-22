//


#include "gameobject.h"


void GameObject::next_level()
{
    // Clear all sprites
    m_sdl->clear();
    m_shotController->clear();

    // Add player sprite that I just cleared
    m_sdl->addSpriteLayer(m_player->getSprite());

    m_level++;

    // Update level text
    m_sdl->getOverlayByFormattedText("Level: %s")->setText(std::to_string(m_level));

    init_level(m_level);
}


void GameObject::init_level(int n)
{
    SDL_Texture * enemyTex = m_sdl->loadTexture("sprites/enemy_ship.png");

    switch (n)
    {

    case 1:

        // Level 1 is 10 enemies
        for (int i = 1; i < 10; ++i)
        {
            EnemyNormal * en = new EnemyNormal(enemyTex, 100 + i * 48, -32, 1000 + (i * 15 * FRAME_DELAY));
            m_enemyController->add(en);
        }
        break;


    case 2:

        // Level 2 is 15 enemies
        for (int i = 1; i < 10; ++i)
        {
            EnemyNormal * en = new EnemyNormal(enemyTex, 100 + i * 48, -32, 1000 + (i * 15 * FRAME_DELAY));
            m_enemyController->add(en);
        }

        for (int i = 1; i < 5; ++i)
        {
            EnemyNormal * en = new EnemyNormal(enemyTex, 200 + i * 48, -32, 5000 + (i * 30 * FRAME_DELAY));
            m_enemyController->add(en);
        }
        break;


    case 3:

        // Reaching level 3 closes the game
        m_run = false;
    }

}

//void init_stage2();
//void init_stage3();

