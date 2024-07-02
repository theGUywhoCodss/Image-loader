#include "raylib.h"
#include "defines.h"
#include "game.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

static int mode = 1;

static void UpdateDrawFrame(void);          // Update and draw one frame
int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template");
    loadGame();
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif
    unloadGame();
    CloseWindow();                  // Close window and OpenGL context
    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void){
    if(mode == 1){
        mode = inGame();
    }else if(mode == 2){
        mode = gameOver();
    }
}