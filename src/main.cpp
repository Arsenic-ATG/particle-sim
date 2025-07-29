/**@file main.cpp
 * @brief entery point of the program
 *
 * This contains all the SDL fucntion and mainly responsible for all the
 * rendering and UI stuff.
 *
 * The program uses SDL3's main callback fucntion which takes care of different
 * entry points for different platforms (especially usefull when using
 * emscripten to build this project)
 */

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/**
 * Defaults (ignore later when the window resizes)
 */
constexpr int default_win_width = 650;
constexpr int default_win_height = 650;

/**
 * App Context
 *
 * pointer to this structure would be made avaialable by SDL throught the
 * rendering pipeline, a pointer to it would be passed around to all the SDL
 * functions as "appstate". If there is anything that is needed by all the SDL
 * functions, it would most probabbly go here instead of existing as a global
 * variable
 */
typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} app_ctx_t;

/**
 * SDL_APPInit
 *
 * Intial entry point of for SDL3, this runs first at the begining of the
 * program, so put stuff that is related any kind of initialisation here.
 *
 * Learn more about it here: https://wiki.libsdl.org/SDL3/SDL_AppInit
 */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("part", "v.1.0", "com.part.test");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize : %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  /* === Handle Window === */
  SDL_Window *window =
      SDL_CreateWindow("part", default_win_width, default_win_height,
                       SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);
  if (!window) {
    SDL_Log("Couldn't Create Window : %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  /* on some displays the maximisation happens asyncornously, so try to sync
   * window before taking in the new dimentions */
  if (!SDL_SyncWindow(window))
    SDL_Log("Warning: Windows sync failed !");

  /* ===  Handle Renderer === */
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    SDL_Log("Couldn't Create Renderer : %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Change the blening mode to blend (for alpha value of colors to have an
  // effect when rending stuff)
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  *appstate = new app_ctx_t{.window = window, .renderer = renderer};

  return SDL_APP_CONTINUE;
}

/**
 * SDL_APPEvent
 *
 * This handles AppEvents (at this point these are user inputs or keystrokes)
 *
 * Learn more about it here: https://wiki.libsdl.org/SDL3/SDL_AppEvent
 */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  app_ctx_t *app_ctx = static_cast<app_ctx_t *>(appstate);
  if (event->type == SDL_EVENT_QUIT)
    return SDL_APP_SUCCESS;

  else if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.scancode) {
    case SDL_SCANCODE_ESCAPE:
      return SDL_APP_SUCCESS;
      break;
    default: {
    }
    }
  }
  return SDL_APP_CONTINUE;
}

/**
 * SDL_APPIterate
 *
 * This function is equivalant to game loop (or update loop) in other
 * game/rendering engines, this will be called over and over responsible for
 * rendering and updating the frame of the application.
 *
 * Learn more about it here: https://wiki.libsdl.org/SDL3/SDL_AppIteratet
 */
SDL_AppResult SDL_AppIterate(void *appstate) {
  app_ctx_t *app_ctx = static_cast<app_ctx_t *>(appstate);

  /* draw background*/
  SDL_SetRenderDrawColor(app_ctx->renderer, 46, 52, 64,
                         SDL_ALPHA_OPAQUE); /* #2e3440 */
  SDL_RenderClear(app_ctx->renderer);
  SDL_RenderPresent(app_ctx->renderer);
  return SDL_APP_CONTINUE;
}

/**
 * SDL_APPQuit
 *
 * This function is called before the program terminates, so if there is some
 * cleanup which you want to do, this is the function to use.
 *
 * Learn more about it here: https://wiki.libsdl.org/SDL3/SDL_AppQuit
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
