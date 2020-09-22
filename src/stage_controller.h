#ifndef __STAGE_CONTROLLER_H_
#define __STAGE_CONTROLLER_H_

#include "grotto_def.h"
#include "renderer.h"


// Handles background scrolling mostly
class StageController
{
private:

    // Only need the renderer
    SDL *   m_sdl;

    // Maximum y coordinate of the stage
    int     m_maxy;

    // How fast the background scrolls
    int     m_scrollSpeed;

    int     m_stageLength;

    // Current position on the stage
    int     m_currentY;

    // Flag to determine if the stage scrolls
    bool    m_scroll;

    // Flag to determine if the stage loops
    bool    m_loop;

public:

    // scr  sets m_scroll
    // l    sets m_loop
    // spd  sets m_scrollSpeed
    StageController(SDL*            sdl,
                    SDL_Texture*    tex,
                    bool            scr,
                    bool            l,
                    int             spd,
                    int             stage_length
                    );
    ~StageController();

    void update();
    void scrollStage();
    void loopStage();

    bool reachedEndOfTexture();
    bool reachedEndOfStage();

    void setScoll(bool f) { m_scroll = f; };
    void setScrollSpeed(int s) { m_scrollSpeed = s; };
};


#endif // __STAGE_CONTROLLER_H_
