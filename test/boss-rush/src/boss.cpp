/*
A boss fight

Only 1 level
*/

#include "gameobject.h"


int WinMain( int argc, char * argv[] )
{
    GameObject * game = new GameObject();
    srand((unsigned) time(NULL));

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