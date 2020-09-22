#ifndef __GROTTO_DEF_H_
#define __GROTTO_DEF_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_FontCache.h"

#include <string>
#include <vector>
#include <exception>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <time.h>
#include <unordered_map>

#define FRAME_RATE 60
#define FRAME_DELAY 1000/FRAME_RATE

#define GROTTO_DIR_UP vec2(0, -1)
#define GROTTO_DIR_LEFT vec2(-1, 0)
#define GROTTO_DIR_DOWN vec2(0, 1)
#define GROTTO_DIR_RIGHT vec2(1, 0)

#define MATH_PI 3.14159

namespace grotto
{

// Struct to hold x and y coords
struct vec2
{
    int x;
    int y;
    vec2(int xx, int yy) : x(xx), y(yy) { };
    vec2() : x(), y() { };

    bool operator==(vec2 c) { return (x == c.x && y == c.y); };
};


// --------------------------------------------------------------------


// Struct to hold x and y coords as well as
// width and height
struct vec4
{
    int x;
    int y;
    int w;
    int h;

    vec4(vec2 p, vec2 d) :
        x(p.x), y(p.y), w(d.x), h(d.y) { };

    vec4(int sx, int sy, int sw, int sh) :
        x(sx), y(sy), w(sw), h(sh) { };

    vec4() :
        x(), y(), w(), h() { };

    bool operator==(vec4 c) { return (x == c.x && y == c.y && w == c.w && h == c.h); };
};


// --------------------------------------------------------------------


// The sprite struct also supports spritesheets
struct Sprite
{

    SDL_Texture * texture;

    // x y w h
    vec4    rect;

    // Spritesheet stuff
    vec2    current_iter;
    vec2    iter_max;

    double  rotationAngle;


    Sprite(SDL_Texture * t, vec4 r) :
        texture(t), rect(r), current_iter(), rotationAngle(0)
        {
            setIter(t);
        };

    Sprite(SDL_Texture * t, int x, int y, int w, int h) :
        texture(t), rect(vec4(x, y, w, h)), current_iter(), rotationAngle(0)
        {
            setIter(t);
        };

    ~Sprite() { };


    // Note that nextIter and prevIter move the iterator by
    // the sprite's width/height and not the texture's w/h
    int nextIter()
    {
        current_iter.x += rect.w;
        if (current_iter.x >= iter_max.x)
        {
            current_iter.x = 0;
            return 1;
        }
        return 0;
    };

    int prevIter()
    {
        current_iter.x -= rect.w;
        if (current_iter.x <= 0)
        {
            current_iter.x = iter_max.x - rect.w;
            return 1;
        }
        return 0;
    }

    // x=0, y=0 : first iter
    void gotoIter(int x, int y)
    {
        if (x < 0 || x > iter_max.x / rect.w)
        {
            // TODO error message
            return;
        }
        if (y < 0 || y > iter_max.y / rect.h)
        {
            // TODO error
            return;
        }
        current_iter.x = x * rect.w;
        current_iter.y = y * rect.h;
    }

    // Used in initialization
    void setIter(SDL_Texture * t)
    {
         SDL_QueryTexture(t, NULL, NULL, &iter_max.x, &iter_max.y);
    };

    vec2 getCenter() const { return vec2(rect.x + rect.w / 2, rect.y + rect.h / 2); };
    vec4 getRect() const { return rect; };
    vec2 getPos() const { return vec2(rect.x, rect.y); };
    vec2 getIter() const { return current_iter; };
    double getRotationAngle() const { return rotationAngle; };

    void setCenter(vec2 c) { rect.x = c.x - rect.w / 2; rect.y = c.y - rect.h / 2; };
    void setRotationAngle(double ang)
    {
        rotationAngle = ang;
        if (rotationAngle > 360.0) rotationAngle -= 360.0;
    };
};


// --------------------------------------------------------------------


struct Hitbox
{
    vec4 rect;

    Hitbox(vec4 r) :
        rect(r) { };

    Hitbox(vec2 p, vec2 d) :
        rect(vec4(p.x, p.y, d.x, d.y)) { };

    Hitbox() :
        rect() { };

    vec4 getRect() const { return rect; };
    vec2 getCenter() const { return vec2(rect.x + rect.w / 2, rect.y + rect.h / 2); };

    void setCenter(vec2 c) { rect.x = c.x - rect.w / 2; rect.y = c.y - rect.h / 2; };
    void setDimensions(vec2 d) { rect.w = d.x; rect.h = d.y; };
};


// --------------------------------------------------------------------


// struct to display text easier
struct TextSprite
{

    FC_Font *   font;

    // x and y positions on the screen
    float       x;
    float       y;

    std::string formatted_text;
    std::string text;

    TextSprite( SDL_Renderer * ren,
                const std::string& font_file,
                Uint32 font_size,
                const std::string& formattedtext,
                const std::string& text_,
                float x_,
                float y_,
                SDL_Color cl
                )
    {
        font = FC_CreateFont();
        FC_LoadFont(font, ren, font_file.c_str(), font_size, cl, TTF_STYLE_NORMAL);
        formatted_text = formattedtext;
        text = text_;
        x = x_;
        y = y_;
    };

    ~TextSprite() { FC_FreeFont(font); };

    void draw(SDL_Renderer * ren)
    {
        FC_Draw(font, ren, x, y, formatted_text.c_str(), text.c_str());
    };

    void setFormattedText(const std::string& t) { formatted_text = t; };
    void setText(const std::string& t) { text = t; };
};


// --------------------------------------------------------------------


template <class T>
void removeFromSTDVector(std::vector<T*>& vec, T* val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

}

#endif // __GROTTO_DEF_H_
