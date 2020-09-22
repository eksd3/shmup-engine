#ifndef __SHOT_H_
#define __SHOT_H_

#include "grotto_def.h"
#include "renderer.h"

class Bullet
{
protected:
    int     radius;
    int     velocity;

    // The starting position of the bullet
    vec2    start;

    // The bullet's target coordinates
    vec2    target;

    // The current position
    vec2    current;

    Sprite* sprite;
    bool    friendly;

public:
    Bullet(vec2 from, vec2 to, int r, int v, SDL_Texture * t, bool f);
    ~Bullet();

    // Init a bullet; used for inheritance
    // r - radius   v - velocity    f - friendly
    void init(int r, int v, bool f);

    void moveBullet();

    int getRadius() const       { return radius; };
    Sprite * getSprite() const  { return sprite; };

    // Returns true if the bullet is outside of the visible screen
    // iWinW - windows width    iWinH - window height
    // iOffset -    the distance a bullet can go off the screen before it is
    //              considered out of bounds
    bool isOutOfBounds(int iWinW, int iWinH, int iOffset);
};


// -------------------------------------------------------------------------


class ShotController
{
private:
    SDL * m_sdl;

    // Naming mishap - bullets is only for enemy bullets
    std::vector<Bullet*> bullets;
    std::vector<Bullet*> friendlyBullets;
    int m_maxBullets;

    // Used for out of bounds checks, passed to isOutOfBounds()
    int m_boundary;

    SDL_Texture * m_bSmallTex;
    SDL_Texture * m_bMediumTex;
    SDL_Texture * m_bLargeTex;

    // Other textures
    std::vector<SDL_Texture*> m_bTextures;

public:
    ShotController(SDL * sdl);
    ~ShotController();

    // Clear bullets and friendlyBullets
    void clear();

    int getNumOfprites() const                      { return (bullets.size() + friendlyBullets.size()); };
    std::vector<Bullet*> getEnemyBullets() const    { return bullets; };
    std::vector<Bullet*> getFriendlyBullets() const { return friendlyBullets; };

    // Returns true if getNumOfSprites() > m_maxBullets
    bool exceededSpriteLimit();

    // Move all bullets in both m_friendlyBullets and bullets
    // Any bullet that leaves the screen (with a little bit of leeway) is destroyed
    void update();

    // Add an SDL_Texture to m_bTextures
    void addTexture(SDL_Texture * t);

    // Remove bullet from m_friendlyBullets and also remove
    // it's sprite from m_sdl->bulletLayer
    void destroyFriendlyBullet(Bullet * bl);

    // Same but for an enemy bullet
    void destroyEnemyBullet(Bullet * bl);

    void setMaxBullets(int n) { m_maxBullets = n; };
    void setbSmallTex(SDL_Texture * t);
    void setbMediumTex(SDL_Texture * t);
    void setbLargeTex(SDL_Texture * t);
    void setBoundary(int n);

    SDL_Texture * getbSmallTex();
    SDL_Texture * getbMediumTex();
    SDL_Texture * getbLargeTex();
    SDL_Texture * getTexture(int i);

    // Helper function to create a bullet
    void createBullet(vec2 pos, vec2 targ, int r, int v, bool f, SDL_Texture * t);

    // Fires an array of num bullets in dir direction
    // start    - starting position
    // dir      - unit vector for direction
    // width    - the width of the object firing the bullets (mostly pass sprite->rect.w here)
    // num      - number of bullets to fire
    // v        - the velocity of the bullets
    // r        - the radius
    // f        - is the bullet friendly (coming from the player or an enemy)
    // t        - texture for bullet's sprite
    void fireStraightArray(vec2 start, vec2 dir, int width, int num, int v, int r, bool f, SDL_Texture * t);

    // Fires ONE bullet from start to targ
    // targ     - coords of the bullet's target
    void fireTargeted(vec2 start, vec2 targ, int v, int r, bool f, SDL_Texture * t);

    // Fires an array of num bullets at random directions around the target
    // alngle   - deviation from the target's position (0.0f fires directly at the target)
    void fireRandomArc(vec2 start, vec2 target, float angle, int num, int v, int r, bool f, SDL_Texture * t);

    // Shotgun
    void fireNonrandomArc(vec2 start, vec2 target, float angle, int num, int v, int r, bool f, SDL_Texture * t);
};


#endif // __SHOT_H_
