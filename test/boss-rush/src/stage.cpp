//


#include "gameobject.h"



// ---------------------------------------------------------------------------------------------------------



void GameObject::next_level()
{
    // Clear all sprites
    m_sdl->clear();
    m_shotController->clear();

    // Including the player sprite, so add that
    m_sdl->addSpriteLayer(m_player->getSprite());

    m_level++;

    // Update the "Level: " text
    m_sdl->getOverlayByFormattedText("Level: %s")->setText(std::to_string(m_level));

    init_level(m_level);
}


// ---------------------------------------------------------------------------------------------------------


void GameObject::reset_level()
{
    // Clear all sprites
    m_sdl->clear();
    m_shotController->clear();
    m_enemyController->clear();

    // Including the player sprite, so add that
    m_sdl->addSpriteLayer(m_player->getSprite());

    // Reset player position to (400, 500)
    m_player->getHitbox()->setCenter(vec2(400, 500));
    m_player->getSprite()->setCenter(m_player->getHitbox()->getCenter());

    // Reset the timer
    // m_time = 0;

    init_level(m_level);
}


// ---------------------------------------------------------------------------------------------------------



void SetDefaultEnemyParams(Enemy * en, EnemyController * controller)
{
    en->setState(-1);
    controller->add(en);
    return;
}



// ---------------------------------------------------------------------------------------------------------




void GameObject::init_level(int n)
{
    // Load the texture for the enemy
    SDL_Texture * enemyTex;

    // Set player vulnerability
    m_player->setVulnerability(true);

    switch (n)
    {

    case 1:
        {
            enemyTex = m_sdl->loadTexture("sprites/boss_ship.png");
            EnemyStage1 * en = new EnemyStage1(enemyTex, 300, -64, 1000);
            SetDefaultEnemyParams(en, m_enemyController);
            return;
        }

    case 2:
        {
            enemyTex = m_sdl->loadTexture("sprites/boss_2.png");
            EnemyStage2 * en = new EnemyStage2(enemyTex, 300, -64, 1000);
            SetDefaultEnemyParams(en, m_enemyController);
            return;
        }

    case 3:
        {
            enemyTex = m_sdl->loadTexture("sprites/boss_3.png");
            EnemyStage3 * en = new EnemyStage3(enemyTex, 300, -64, 1000);
            SetDefaultEnemyParams(en, m_enemyController);
            return;
        }

    case 4:
        {
            enemyTex = m_sdl->loadTexture("sprites/boss_4.png");
            EnemyStage4 * en = new EnemyStage4(enemyTex, 300, -64, 1000);
            SetDefaultEnemyParams(en, m_enemyController);
            return;
        }
    }

    // After beating all stages
    m_paused = true;
    m_sdl->clear();

    m_level = MainMenu(true);

    if (!m_run)
        return;

    m_time = 0;
    m_sdl->clearOverlay();
    m_sdl->addText("Time: %s", std::to_string(m_time), "sample.ttf", 15, 10, m_iWinH-40, FC_MakeColor(0, 255, 255, 255));
    m_sdl->addText("Level: %s", std::to_string(m_level), "sample.ttf", 15, 10, m_iWinH-60, FC_MakeColor(255, 0, 0, 255));

    next_level();

}

