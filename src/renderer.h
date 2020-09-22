#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "grotto_def.h"

using namespace grotto;


class SDL
{

    // Window width and height
    int m_winWidth;
    int m_winHeight;

    SDL_Window * m_window;
    SDL_Renderer * m_renderer;

    // StageController vars
    int m_backgroundClip;
    SDL_Texture * m_backgroundLayer;

    // Sprite vectors
    std::vector<Sprite*> m_spriteLayer;
    std::vector<Sprite*> m_bulletLayer;
    std::vector<TextSprite*> m_overlayLayer;

public:

    // iWinW, iWinH : window width and height
    SDL( int iWinW, int iWinH, Uint32 flags = 0 );
    virtual ~SDL();

    // Clears sprite vectors
    void clear();

    // Takes x and y positions of the texture as well as width and height
    void renderTexture(SDL_Texture * tex, int x, int y, int w, int h);

    // Same thing but only with positional args
    // a little bit slower since it has to determine the width and height
    void renderTexture(SDL_Texture * tex, int x, int y);

    void renderSpriteSheet(Sprite * s);

    // Loads texture from file
    SDL_Texture * loadTexture(const std::string& file);

    // Adds text to m_overlayLayer
    // cl : FC_MakeColor(r, g, b, a);
    void addText(   const std::string& formattedText,
                    const std::string& text,
                    const std::string& fontfile,
                    Uint32 fontsize,
                    float x,
                    float y,
                    SDL_Color cl);

    // Adds to sprite vectors
    void addSpriteLayer(Sprite * s);
    void addBulletLayer(Sprite * s);
    void addOverlayLayer(TextSprite * t);

    // Removes a sprite from vector
    void destroySpriteLayer(Sprite * spr);
    void destroyBulletLayer(Sprite * spr);

    void draw();
    void drawBackgroundLayer();
    void drawVectorLayer(std::vector<Sprite*>& layer);
    void drawOverlayLayer();

    // Returns total number of sprites
    int getNumOfSprites() const;

    // Returns a TextSprite whose formattedText == query
    TextSprite * getOverlayByFormattedText(const std::string& query) const;

    // Same but for text
    TextSprite * getOverlayByText(const std::string& query) const;

    TextSprite * getOverlayByIndex(int i) const     { return m_overlayLayer[i]; };
    SDL_Renderer * getRenderer() const              { return m_renderer; };
    SDL_Window * getWindow() const                  { return m_window; };
    int getBackgroundClip() const                   { return m_backgroundClip; };
    int getWindowWidth() const                      { return m_winWidth; };
    int getWindowHeight() const                     { return m_winHeight; };

    void setBackgroundLayer(SDL_Texture * t)    { m_backgroundLayer = t; };
    void setBackgroundClip(int n)               { m_backgroundClip = n; };
};

class RendererError : public std::exception
{
    std::string msg;
public:
    RendererError();
    RendererError( const std::string & );
    virtual ~RendererError() throw();
    virtual const char * what() const throw();
};

#endif // __RENDERER_H
