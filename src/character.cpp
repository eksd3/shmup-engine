#include "character.h"

Player::Player(vec2 pos, vec2 dim, float vel)
{
    init(vel, vel, true);
    hitbox = Hitbox(vec4(pos.x, pos.y, dim.x, dim.y));
}

void Player::movePlayer(vec2 vec, int iWinW, int iWinH)
{
    moveCharacter(vec);

    if (hitbox.rect.x + hitbox.rect.w > iWinW)
        hitbox.rect.x = iWinW - hitbox.rect.w;
    else if (hitbox.rect.x < 0)
        hitbox.rect.x = 0;

    if (hitbox.rect.y + hitbox.rect.h > iWinH)
        hitbox.rect.y = iWinH - hitbox.rect.h;
    else if (hitbox.rect.y < 0)
        hitbox.rect.y = 0;

    sprite->setCenter(hitbox.getCenter());
}


// -----------------------------------------------------------------------


void Enemy::initEnemy(float xvel, float yvel, int _hp, int spawn)
{
    init(xvel, yvel, false);

    hp              = _hp;
    spawnTimer      = spawn;
    movementScript  = nullptr;
    shotPattern     = nullptr;
}


void Enemy::execMovementScript()
{
    if (movementScript != nullptr)
        movementScript(this);
}

void Enemy::execShotScript(Character * t, ShotController * shc)
{
    if (shotPattern != nullptr)
        shotPattern(this, t, shc);
}

void Enemy::addVar(std::string key, int val)
{
    m_vars[key] = val;
}

int Enemy::getVar(std::string key)
{
    for (auto var : m_vars)
    {
        if (var.first == key)
            return var.second;
    }
    return INT_MAX;
}


// -----------------------------------------------------------------------


void Character::init(float xvel, float yvel, bool fr)
{
    xvelocity   = xvel;
    yvelocity   = yvel;
    friendly    = fr;

    vulnerable  = true;
    timer       = 0;
}


void Character::setSprite(SDL_Texture * tex, int w, int h)
{
    int x = hitbox.rect.x + hitbox.rect.w / 2 - w / 2;
    int y = hitbox.rect.y + hitbox.rect.h / 2 - h / 2;
    Sprite * spr = new Sprite(tex, x, y, w, h);
    sprite = spr;
    // delete spr;
}

void Character::moveCharacter(vec2 vec)
{
    auto mv = [&](float velocity, char dir)
    {
        if (velocity > 0.0f && velocity < 1.0f)
        {
            float fQuotient = 1 / velocity;
            int iQuotient = (int)fQuotient;
            if ( (fQuotient - iQuotient) > 0.5f)
                iQuotient += 1;

            if (timer % (FRAME_DELAY * iQuotient) == 0)
            {
                switch (dir)
                {
                case 'h':
                    hitbox.rect.x += vec.x;
                    break;
                case 'v':
                    hitbox.rect.y += vec.y;
                    break;
                }
            }
            return;
        }
        switch (dir)
        {
        case 'h':
            hitbox.rect.x += vec.x * velocity;
            break;
        case 'v':
            hitbox.rect.y += vec.y * velocity;
            break;
        }
    };

    mv(xvelocity, 'h');
    mv(yvelocity, 'v');
    sprite->setCenter(hitbox.getCenter());
}


bool Character::isOutOfBounds(int iWinW, int iWinH, int iOffset)
{
    return (    hitbox.rect.x <= -iOffset || hitbox.rect.x >= iWinW + iOffset ||
                hitbox.rect.y <= -iOffset || hitbox.rect.y >= iWinH + iOffset );
}

void Character::incrementTimer(int n)
{
    timer += n;
}

void Character::resetTimer()
{
    timer = 0;
}


// -----------------------------------------------------------------------


EnemyController::EnemyController(SDL * sdl)
{
    m_sdl = sdl;
    timer = 0;
    m_boundary = 64;
}

EnemyController::~EnemyController()
{
    delete m_sdl;
}

void EnemyController::spawnFirstEnemy()
{
    if (m_enemyVector.size() > 0)
    {
        Enemy * first = m_enemyVector[0];
        if (first->getSpawnTimer() <= timer)
        {
            spawn(first);
            m_enemyVector.erase(m_enemyVector.begin());
            spawnFirstEnemy();
        }
    }
}

void EnemyController::update()
{
    spawnFirstEnemy();

    std::vector<Enemy*>::iterator iter;
    for (iter = m_spawnedEnemies.begin(); iter != m_spawnedEnemies.end(); ++iter)
    {
        if ( (*iter)->isOutOfBounds(m_sdl->getWindowWidth(), m_sdl->getWindowHeight(), m_boundary) )
        {
            m_spawnedEnemies.erase(m_spawnedEnemies.begin() + (iter - m_spawnedEnemies.end()));
            m_sdl->destroySpriteLayer((*iter)->getSprite());
            --iter;
        }

        (*iter)->execMovementScript();

        (*iter)->incrementTimer(FRAME_DELAY);
        (*iter)->incrementShotTimer(FRAME_DELAY);
    }

    timer += FRAME_DELAY;
}

void EnemyController::add(Enemy * en)
{
    if (m_enemyVector.size() == 0)
    {
        m_enemyVector.push_back(en);
        return;
    }

    else
    {
        for (std::vector<Enemy*>::iterator it = m_enemyVector.begin(); it != m_enemyVector.end(); ++it)
        {
            if ( en->getSpawnTimer() < (*it)->getSpawnTimer() )
            {
                m_enemyVector.insert(m_enemyVector.begin() + (it - m_enemyVector.begin()), en);
                return;
            }
        }
        m_enemyVector.push_back(en);
    }
}

void EnemyController::spawn(Enemy * en)
{
    m_spawnedEnemies.push_back( en );
    m_sdl->addSpriteLayer(en->getSprite());
}

void EnemyController::removeEnemy(Enemy * en)
{
    grotto::removeFromSTDVector(m_spawnedEnemies, en);
    //m_spawnedEnemies.erase(std::remove(m_spawnedEnemies.begin(), m_spawnedEnemies.end(), en), m_spawnedEnemies.end());
}

void EnemyController::destroyDead()
{
    for (std::vector<Enemy*>::iterator it = m_spawnedEnemies.begin(); it != m_spawnedEnemies.end(); ++it)
    {
        if ( (*it)->getHP() <= 0 )
        {
            m_sdl->destroySpriteLayer((*it)->getSprite());
            m_spawnedEnemies.erase(m_spawnedEnemies.begin() + (it - m_spawnedEnemies.begin()));
            --it;
        }
    }
}
