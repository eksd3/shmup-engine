#include "character.h"

RendererError::RendererError() :
    exception(),
    msg( SDL_GetError() ) { };

RendererError::RendererError( const std::string & m ) :
    exception(),
    msg( m ) { };

RendererError::~RendererError() throw() { };

const char * RendererError::what() const throw()
{
    return msg.c_str();
}

// -------------------------------------------------------------------------------------

SDL::SDL( int iWinW, int iWinH, Uint32 flags )
{
    if ( SDL_Init( flags ) != 0 )
        throw RendererError();

    m_winWidth = iWinW;
    m_winHeight = iWinH;

    m_window = SDL_CreateWindow("GROTTO", 100, 100, iWinW, iWinH, SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
        throw RendererError();

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr)
        throw RendererError();
}

SDL::~SDL()
{
    clear();
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}

void SDL::addSpriteLayer(Sprite * s)
{
    m_spriteLayer.push_back(s);
}

void SDL::addBulletLayer(Sprite * s)
{
    m_bulletLayer.push_back(s);
}

void SDL::addOverlayLayer(TextSprite * t)
{
    m_overlayLayer.push_back(t);
}

void SDL::destroySpriteLayer(Sprite * spr)
{
    grotto::removeFromSTDVector(m_spriteLayer, spr);
}

void SDL::destroyBulletLayer(Sprite * spr)
{
    grotto::removeFromSTDVector(m_bulletLayer, spr);
}

TextSprite * SDL::getOverlayByFormattedText(const std::string& query) const
{
    for (TextSprite * ts : m_overlayLayer)
    {
        if (ts->formatted_text == query)
            return ts;
    }
    return nullptr;
}

TextSprite * SDL::getOverlayByText(const std::string& query) const
{
    for (TextSprite * ts : m_overlayLayer)
    {
        if (ts->text == query)
            return ts;
    }
    return nullptr;
}

void SDL::draw()
{
    drawBackgroundLayer();
    drawVectorLayer(m_spriteLayer);
    drawVectorLayer(m_bulletLayer);
    drawOverlayLayer();
}

void SDL::drawBackgroundLayer()
{
    SDL_Rect src;
    src.x = 0;
    src.y = m_backgroundClip;
    src.w = m_winWidth;
    src.h = m_winHeight;

    SDL_Rect dst;
    dst.x = 0;
    dst.y = 0;
    dst.w = m_winWidth;
    dst.h = m_winHeight;

    SDL_RenderCopy(m_renderer, m_backgroundLayer, &src, &dst);
}

void SDL::drawVectorLayer(std::vector<Sprite*>& layer)
{
    for (Sprite * s : layer)
    {
        renderSpriteSheet(s);
    }
}

void SDL::drawOverlayLayer()
{
    for (TextSprite * s : m_overlayLayer)
    {
        s->draw(m_renderer);
    }
}

SDL_Texture * SDL::loadTexture(const std::string& file)
{
    SDL_Texture * texture = IMG_LoadTexture(m_renderer, file.c_str());
    if (texture == nullptr)
    {
        SDL_DestroyTexture(texture);
        IMG_Quit();
        SDL_Quit();
    }
    return texture;
}

void SDL::addText(      const std::string& formattedText,
                        const std::string& text,
                        const std::string& fontfile,
                        Uint32 fontsize,
                        float x,
                        float y,
                        SDL_Color cl)
{
    TextSprite * spr = new TextSprite(m_renderer, fontfile, fontsize, formattedText, text, x, y, cl);
    m_overlayLayer.push_back(spr);
}

void SDL::renderSpriteSheet(Sprite * spr)
{
    SDL_Rect src;
    src.x = spr->getIter().x;
    src.y = spr->getIter().y;
    src.w = spr->rect.w;
    src.h = spr->rect.h;

    SDL_Rect dst;
    dst.x = spr->rect.x;
    dst.y = spr->rect.y;
    dst.w = spr->rect.w;
    dst.h = spr->rect.h;

    SDL_RenderCopyEx(   m_renderer,
                        spr->texture,
                        &src,
                        &dst,
                        spr->rotationAngle,
                        NULL,
                        SDL_FLIP_NONE);
}

void SDL::renderTexture(SDL_Texture * texture, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(m_renderer, texture, NULL, &dst);
}

void SDL::clear()
{
    m_spriteLayer.clear();
    m_bulletLayer.clear();
}
