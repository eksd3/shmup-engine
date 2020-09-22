/*
A Space Invaders clone.
*/

#include "gameobject.h"


int WinMain( int argc, char * argv[] )
{
    GameObject * game = new GameObject();

    while (game->is_running())
    {
        game->handle_events();
        game->update();
        game->enemy_shots();
        game->check_collision();
        game->delay_frame();
    }

    return 0;
}
