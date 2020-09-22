#include "stage_controller.h"

StageController::StageController(SDL* sdl, SDL_Texture* tex, bool scr, bool l, int spd, int stage_length)
{
    m_sdl = sdl;
    m_sdl->setBackgroundLayer(tex);

    int width, height;
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
    float s_ratio = height / m_sdl->getWindowHeight();
    m_maxy = height - (m_sdl->getWindowHeight() * s_ratio);
    m_sdl->setBackgroundClip(m_maxy);

    m_currentY = 0;
    m_stageLength = stage_length;
    m_scroll = scr;
    m_loop = l;
    m_scrollSpeed = spd;
}

StageController::~StageController()
{

}

bool StageController::reachedEndOfTexture()
{
    return (m_sdl->getBackgroundClip() <= 0);
}

bool StageController::reachedEndOfStage()
{
    return (m_currentY >= m_stageLength);
}

void StageController::loopStage()
{
    m_sdl->setBackgroundClip(m_maxy);
}

void StageController::scrollStage()
{
    m_sdl->setBackgroundClip( m_sdl->getBackgroundClip() - m_scrollSpeed );
}

void StageController::update()
{
    if (m_scroll)
    {
        if (reachedEndOfTexture())
        {
            if (m_loop)
                loopStage();
        }

        else
            scrollStage();
    }

    m_currentY += FRAME_DELAY;
}
