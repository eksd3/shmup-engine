//


#include "gameobject.h"


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


void GameObject::init_level(int n)
{
    // Load the texture for the enemy
    SDL_Texture * enemyTex = m_sdl->loadTexture("sprites/boss_ship.png");

    switch (n)
    {

    case 1:
        EnemyNormal * en = new EnemyNormal(enemyTex, 300, -64, 1000);
        en->setState(-1);
        m_enemyController->add(en);
        return;

    }

    m_run = false;
}

