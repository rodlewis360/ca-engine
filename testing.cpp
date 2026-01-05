#include <string>
#include <random>
#include "base.h"
#include "user_defined.h"
#include "text_renderer.h"
#include "sample_rules.h"
#include "evo_rule.h"
#include "langtons_ant.h"
#include "engine.h"

using namespace std;

#define USING_SDL
#define TARGETED_RENDER
#include "rectangle_renderer.h"

#ifdef USING_SDL

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 1080

Grid<LangtonsState> grid = Grid<LangtonsState>(200, 200, false);

LangtonsAnt ant = LangtonsAnt(100, 100, 2);

int* ant_name_to_rules(std::string name) {
    int* rules = new int[name.size()];
    for (int i = 0; i < name.size(); i++) {
        switch (name[i]) {
            case 'R':
                rules[i] = 1;
                break;
            case 'L':
                rules[i] = 3;
                break;
            case 'N':
                rules[i] = 0;
                break;
            case 'U':
                rules[i] = 2;
                break;
            default:
                rules[i] = 0;
                break;
        }
    }

    return rules;
}

int* random_rules(int size) {
    int* rules = new int[size];
    for (int i = 0; i < size; i++) {
        rules[i] = rand() % 4;
    }
    return rules;
}

Color forest_fire_conversion(int value) {
    if (value > 0 && value <= 128) return Color(0, min(value + 100, 255), 0);
    if (value > 128 && value <= 254) return Color((value - 128) / 3, (value - 128) / 3, max(value - 200, 0) / 3);
    if (value == 255) return Color(255, 128, 0);
    return Color(0,0,0);
}

Color life_conversion(int value) {
    return (value > 0) ? Color(255,255,255) : Color(0,0,0);
}

Color brians_brain_conversion(int value) {
    return (value > 0) ? ((value > 1) ? Color(200,150,255) : Color(50, 50, 255)) : Color(0,0,0);
}

Color evo_rule_conversion(EvoState value) {
    if (value.refractory) return Color(25, 25, 25);
    // if (value.refractory) return Color(value.survive / 8, value.create_new / 8, (value.mutate_chance * 255) / 8);
    if (!value.alive) return Color(0,0,0);
    return Color(value.survive, value.create_new, value.mutate_chance * 255);
}

Color langtons_ant_conversion(LangtonsState state) {
    if (state.is_ant) {
        return Color(255, 0, 0);
    }
    if (state.state == 0) return Color (0, 0, 0);
    if (state.state == 1) return Color (100, 255, 255);
    if (state.state == 2) return Color (150, 200, 10);
    if (state.state == 3) return Color (50, 125, 125);
    if (state.state == 4) return Color (75, 175, 175);
    if (state.state == 5) return Color (25, 75, 75);
    if (state.state == 6) return Color (125, 125, 125);
    if (state.state == 7) return Color (0, 25, 25);
    if (state.state == 8) return Color (128, 0, 128);
    if (state.state == 9) return Color (0, 255, 255);
    if (state.state == 10) return Color (255, 192, 203);
    if (state.state == 11) return Color (165, 42, 42);
    if (state.state == 12) return Color (255, 215, 50);
    return Color(0,0,0);
}

void final_render(GraphicsCell *cells, Color bg_color) {
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(renderer);
    SDL_FRect to_render;
    for (int i = 0; i < grid.xsize() * grid.ysize(); i++) {
        SDL_SetRenderDrawColor(renderer, cells[i].color.r, cells[i].color.g, cells[i].color.b, cells[i].color.a);
        to_render.h = cells[i].height;
        to_render.w = cells[i].width;
        to_render.x = cells[i].pos_x;
        to_render.y = cells[i].pos_y;
        SDL_RenderFillRect(renderer, &to_render);
    }
    
    SDL_RenderPresent(renderer);  /* put it all on the screen! */
}

#ifdef TARGETED_RENDER

SDL_Texture* gridTex = nullptr;

void init_grid_texture() {
    gridTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!gridTex) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return;
    }

    // Fill with background color initially
    SDL_SetRenderTarget(renderer, gridTex);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}

void targeted_final_render(GraphicsCell *cells, int size) {
    SDL_SetRenderTarget(renderer, gridTex);

    SDL_FRect to_render;
    for (int i = 0; i < size; i++) {
        SDL_SetRenderDrawColor(renderer, cells[i].color.r, cells[i].color.g, cells[i].color.b, cells[i].color.a);
        to_render.h = cells[i].height;
        to_render.w = cells[i].width;
        to_render.x = cells[i].pos_x;
        to_render.y = cells[i].pos_y;
        SDL_RenderFillRect(renderer, &to_render);
    }
    
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, gridTex, nullptr, nullptr);
    SDL_RenderPresent(renderer);  /* put it all on the screen! */
}

#endif // TARGETED_RENDER

RectangleRenderer<LangtonsState> ca_renderer = RectangleRenderer<LangtonsState>(Color(0,0,0,255), WINDOW_WIDTH, WINDOW_HEIGHT, grid.xsize(), grid.ysize(), langtons_ant_conversion);
ForestFireRule forest_fire_rule;
LifeRule life_rule;
BriansBrainRule brians_brain_rule;
EvoRule evo_rule(5);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    int seed = time(0);
    if (argv[1]) {
        for (int i = 0; i < 2; i++) {
            if (argv[1][i] == '-' && argv[1][i+1] == 's') {
                seed = stoi(argv[2]);
            }
        }
    }

    srand(seed);
    SDL_srand(seed);

    SDL_Log(("Seed: " + to_string(seed)).c_str());

    SDL_SetAppMetadata("Cellular Automaton Engine", "1.0", "com.ca.engine");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("CA Engine", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    #ifdef TARGETED_RENDER
        init_grid_texture();
    #endif

    for_each(grid.begin(), grid.end(), [](Cell<LangtonsState> &cell) {cell.m->state = 0;});
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        float grid_x = (mouse_x * grid.xsize()) / WINDOW_WIDTH;
        float grid_y = (mouse_y * grid.ysize()) / WINDOW_HEIGHT;
        grid.get_cell(grid_x, grid_y).m->is_ant = true;
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint32 frame_start_time = SDL_GetTicks();

    // run_iter(grid, life_rule);

    int* old_pos = ant.process(grid);

    // ca_renderer.render(grid);

    ca_renderer.targeted_render(grid, set<pair<int,int>>{
        {old_pos[0], old_pos[1]}, 
        {old_pos[2], old_pos[3]}
    });

    #ifdef FPS
        Uint32 frame_end_time = SDL_GetTicks();
        int delay_time = (1000 / FPS) - (frame_end_time - frame_start_time);
        if (delay_time > 0) {
            SDL_Delay(delay_time);
        }
    #endif // FPS

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

#endif // USING_SDL


#ifndef USING_SDL
int main(int argc, char *argv[]) {
    double start_time = time(0);

    srand(start_time);

    function<string(int)> brians_brain_converter = [](int value) {return ((value > 0) ? ((value == 2) ? "#" : "O") : " ");};
    function<string(int)> life_converter = [](int value) {return ((value > 0) ? "#" : " ");};
    function<string(int)> forest_fire_converter = [](int value) {
        if (value > 0 && value <= 128) return "\033[32m^\033[0m";
        if (value > 128 && value <= 254) return "\033[90m~\033[0m";
        if (value == 255) return "\033[31mM\033[0m";
        return " ";
    };

    Grid<int> grid = Grid<int>(20, 20, false);

    for_each(grid.begin(), grid.end(), [](Cell<int> &cell) {*(cell.m) = (1.0 * rand() / RAND_MAX) > 0.85;});
    grid.set_edges(0);

    TextRenderer<int> renderer = TextRenderer<int>(forest_fire_converter);
    renderer.render(grid);
    cout << left;

    LifeRule rule;
    BriansBrainRule brain_rule;
    ForestFireRule forest_fire_rule;

    animate(grid, forest_fire_rule, renderer, 200);
}
#endif // not USING_SDL