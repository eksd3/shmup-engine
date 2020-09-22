#include "shot.h"


ShotController::ShotController(SDL * sdl)
{
    m_sdl = sdl;

    m_boundary      = 10;
    m_bSmallTex     = nullptr;
    m_bMediumTex    = nullptr;
    m_bLargeTex     = nullptr;
}

ShotController::~ShotController()
{
    SDL_DestroyTexture(m_bSmallTex);
    SDL_DestroyTexture(m_bMediumTex);
    SDL_DestroyTexture(m_bLargeTex);
    for (SDL_Texture * t : m_bTextures)
        SDL_DestroyTexture(t);
}

void ShotController::clear()
{
    bullets.clear();
    friendlyBullets.clear();
}

bool ShotController::exceededSpriteLimit()
{
    return ( getNumOfprites() >= m_maxBullets && m_maxBullets != 0 );
}

SDL_Texture * ShotController::getbSmallTex()
{
    return m_bSmallTex;
}

SDL_Texture * ShotController::getbMediumTex()
{
    return m_bMediumTex;
}

SDL_Texture * ShotController::getbLargeTex()
{
    return m_bLargeTex;
}

SDL_Texture * ShotController::getTexture(int i)
{
    return m_bTextures[i];
}

void ShotController::createBullet(vec2 pos, vec2 targ, int r, int v, bool f, SDL_Texture * t)
{
    Bullet * bullet = new Bullet(pos, targ, r, v, t, f);
    if (f)
        friendlyBullets.push_back(bullet);
    else
        bullets.push_back(bullet);
    m_sdl->addBulletLayer(bullet->getSprite());
}

void ShotController::fireStraightArray(vec2 start, vec2 dir, int width, int num, int v, int r, bool f, SDL_Texture * t)
{
    if (exceededSpriteLimit())
        return;

    vec2 pos;
    pos.y = start.y;
    vec2 targ = start;

    int offset = 10;
    if (dir.y == 1)         targ.y = m_sdl->getWindowHeight() + offset;
    else if (dir.y == -1)   targ.y = -offset;
    if (dir.x == 1)         targ.x = m_sdl->getWindowWidth() + offset;
    else if (dir.x == -1)   targ.x = -offset;

    int delta = width / (num + 1);
    for (int i = 1; i <= num; ++i)
    {
        pos.x = start.x + i * delta;
        targ.x = pos.x;
        createBullet(pos, targ, r, v, f, t);
    }
}

void ShotController::fireTargeted(vec2 start, vec2 targ, int v, int r, bool f, SDL_Texture * t)
{
    if (exceededSpriteLimit())
        return;

    vec2 pos;
    pos.y = start.y;

    pos.x = start.x - r * 0.5f;
    createBullet(pos, targ, r, v, f, t);
}

void ShotController::fireRandomArc(vec2 start, vec2 target, float angle, int num, int v, int r, bool f, SDL_Texture * t)
{
    if (exceededSpriteLimit())
        return;

    for (int i = 1; i <= num; ++i)
    {
        int random = rand() % (int)angle;
        target.x += random;
        createBullet(start, target, r, v, f, t);
    }
}

void ShotController::fireNonrandomArc(vec2 start, vec2 target, float angle, int num, int v, int r, bool f, SDL_Texture * t)
{
    if (exceededSpriteLimit())
        return;

    for (int i = 0; i <= num - 1; ++i)
    {
        float fCurrentAng = (angle / (2 * num)) * i;
        float fHalfAng = fCurrentAng * 0.5f;
        float fBeta = 90.0f - fHalfAng;
        float fGamma = 90.0f - fBeta;

        int iDeltaX = target.x + sin(fHalfAng * MATH_PI / 180.0f);
        int iDeltaY = target.y + sin(fGamma * MATH_PI / 180.0f);

        target.x = iDeltaX;
        target.y = iDeltaY;

        createBullet(start, target, r, v, f, t);

        /*
        target = vSaveTarget;
        iDeltaX = target.x + sin(fHalfAng * MATH_PI / 180.0f) * -1.0f;
        iDeltaY = target.y + sin(fGamma * MATH_PI / 180.0f) * -1.0f;

        target.x = iDeltaX;
        target.y = iDeltaY;

        createBullet(start, target, r, v, f, t);
        */
    }
}

void ShotController::update()
{
    // Move all bullets in both m_friendlyBullets and bullets
    // Any bullet that leaves the screen (with a little bit of leeway) is destroyed

    int iWinW = m_sdl->getWindowWidth();
    int iWinH = m_sdl->getWindowHeight();

    for (std::vector<Bullet*>::iterator b = bullets.begin(); b != bullets.end(); ++b)
    {
        (*b)->moveBullet();
        if ( (*b)->isOutOfBounds(iWinW, iWinH, m_boundary) )
        {
            destroyEnemyBullet( *b );
            --b;
        }
    }
    for (std::vector<Bullet*>::iterator b = friendlyBullets.begin(); b != friendlyBullets.end(); ++b)
    {
        (*b)->moveBullet();
        if ( (*b)->isOutOfBounds(iWinW, iWinH, m_boundary) )
        {
            destroyFriendlyBullet( *b );
            --b;
        }
    }
}

void ShotController::destroyFriendlyBullet(Bullet * bl)
{
    grotto::removeFromSTDVector(friendlyBullets, bl);
    m_sdl->destroyBulletLayer(bl->getSprite());
}

void ShotController::destroyEnemyBullet(Bullet * bl)
{
    grotto::removeFromSTDVector(bullets, bl);
    m_sdl->destroyBulletLayer(bl->getSprite());
}

void ShotController::addTexture(SDL_Texture * t)
{
    m_bTextures.push_back(t);
}

void ShotController::setbSmallTex(SDL_Texture * t)
{
    m_bSmallTex = t;
}

void ShotController::setbMediumTex(SDL_Texture * t)
{
    m_bMediumTex = t;
}

void ShotController::setbLargeTex(SDL_Texture * t)
{
    m_bLargeTex = t;
}

void ShotController::setBoundary(int n)
{
    m_boundary = n;
}


// ---------------------------------------------------------------------



Bullet::Bullet(vec2 from, vec2 to, int r, int v, SDL_Texture * tex, bool f)
{
    init(r, v, f);

    start = from;
    current = from;
    target = to;

    Sprite * s = new Sprite(tex, from.x, from.y, radius, radius);
    sprite = s;
}

Bullet::~Bullet()
{
    delete sprite;
}

void Bullet::init(int r, int v, bool f)
{
    radius = r;
    velocity = v;
    friendly = f;
}

void Bullet::moveBullet()
{
    int dx = target.x - start.x;
    int dy = target.y - start.y;
    float hip = std::sqrt(dx*dx + dy*dy);
    float sine = dx / hip;
    float cosine = dy / hip;

    current.x += (velocity * sine);
    current.y += (velocity * cosine);

    sprite->rect.x = current.x;
    sprite->rect.y = current.y;
}

bool Bullet::isOutOfBounds(int iWinW, int iWinH, int iOffset)
{
    return (    ( current.x <= -iOffset || current.x >= iWinW + iOffset ) ||
                ( current.y <= -iOffset || current.y >= iWinH + iOffset ) );
}


// ---------------------------------------------------------------------
