#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "error.h"
#include "vis_struct.h"
#include "text.h"
#include <math.h>
#include <string.h>
#include "store.h"

#define NAME_WIDTH 200
#define VALUE_WIDTH 300
#define CELL_HEIGHT 20
#define POPUP_BORDER_WIDTH 7
#define POPUP_TEXT_PADDING 5

#define SYM_WIDTH 8
#define SYM_HEIGHT 10
#define TEXT_VERT_SPACE 5
#define TEXT_POINT_SIZE 13

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;

// Colors
static const SDL_Color grid_color           = { 0, 0, 0, 255 };
static const SDL_Color bg_color             = { 200, 200, 200, 255 };
static const SDL_Color text_color           = { 0, 0, 0, 255 };
static const SDL_Color border_color         = { 0, 0, 0, 255 };
static const SDL_Color popup_bg_color       = { 0xFF, 0xFF, 0xCC, 0xFF };
static const SDL_Color popup_border_color   = { 0xFF, 0x99, 0x33, 0xFF };

typedef struct
{
    vis_struct_t *vis;
    int x, y;
} gfx_struct_t;

static store_t gfx_structs;

void gfx_init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO)) {
        set_error("Failed to init SDL");
        return;
    }

    // Initialize SDL_ttf
    if (TTF_Init()) {
        set_error("Failed to init SDL_ttf");
        return;
    }

    // Create main window and renderer for it
    window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);
    if (!window) {
        set_error("Failed to create SDL Window");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        set_error("Failed to create SDL renderer");
        return;
    }

    // Load fonts
    font = TTF_OpenFont("../fonts/DejaVuSansMono.ttf", TEXT_POINT_SIZE);

    // Initialize gfx structures store
    store_create(&gfx_structs, gfx_struct_t);
}

void gfx_kill()
{
    // Unload fonts
    TTF_CloseFont(font);

    // Destroy window and its renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Release SDL libraries
    TTF_Quit();
    SDL_Quit();
}

void _set_color(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void _render_text(const char *text, int x, int y)
{
    SDL_Surface *text_surface = TTF_RenderUTF8_Solid(font, text, text_color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect target_rect = { x, y, text_surface->w, text_surface->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &target_rect);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

void _render_popup(int x, int y, const char *text)
{
    size_t text_len = strlen(text);
    int line_size = (int)sqrtf((float)text_len) * 2;
    int line_count = text_line_num(text, line_size);

    SDL_Rect outer_rect = {
        x,
        y,
        line_size * SYM_WIDTH + POPUP_TEXT_PADDING * 2 + POPUP_BORDER_WIDTH * 2,
        line_count * (SYM_HEIGHT + TEXT_VERT_SPACE) + POPUP_TEXT_PADDING * 2 + POPUP_BORDER_WIDTH * 2
    };

    SDL_Rect inner_rect = {
        outer_rect.x + POPUP_BORDER_WIDTH - 1,
        outer_rect.y + POPUP_BORDER_WIDTH - 1,
        outer_rect.w - 2 * (POPUP_BORDER_WIDTH - 1),
        outer_rect.h - 2 * (POPUP_BORDER_WIDTH - 1),
    };

    _set_color(popup_border_color);
    SDL_RenderFillRect(renderer, &outer_rect);

    _set_color(popup_bg_color);
    SDL_RenderFillRect(renderer, &inner_rect);

    _set_color(border_color);
    SDL_RenderDrawRect(renderer, &outer_rect);
    SDL_RenderDrawRect(renderer, &inner_rect);

    text_line_t tl = text_line(text, line_size);
    for (int i = 0; tl.size > 0; i++) {
        char temp = tl.start[tl.size];
        tl.start[tl.size] = '\0';
        _render_text(tl.start, inner_rect.x + POPUP_TEXT_PADDING, inner_rect.y + POPUP_TEXT_PADDING + i * (SYM_HEIGHT + TEXT_VERT_SPACE));
        tl.start[tl.size] = temp;

        tl = text_line(tl.start + tl.size, line_size);
    }
    
}

void _render_vis_struct(vis_struct_t *st, int x, int y)
{
    SDL_Rect name_rect = { x, y, NAME_WIDTH, CELL_HEIGHT };
    SDL_Rect value_rect = { x + NAME_WIDTH - 1, y, VALUE_WIDTH, CELL_HEIGHT };
    for (int i = 0; i < st->fields.size; i++) {
        vis_field_t *field = store_pget(&st->fields, i);

        _set_color(grid_color);
        SDL_RenderDrawRect(renderer, &name_rect);
        SDL_RenderDrawRect(renderer, &value_rect);

        _render_text(field->name, name_rect.x + 1, name_rect.y);
        _render_text(vis_field_value_str(field), value_rect.x + 1, value_rect.y);

        name_rect.y += CELL_HEIGHT - 1;
        value_rect.y += CELL_HEIGHT - 1;
    }
}

void gfx_render()
{
    // Clear screen
    _set_color(bg_color);
    SDL_RenderClear(renderer);

    // Draw visual structures
    for (size_t i = 0; i < gfx_structs.size; i++) {
        gfx_struct_t *gs = store_pget(&gfx_structs, i);
        _render_vis_struct(gs->vis, gs->x, gs->y);
    }

    // Draw popup help, if cursor is in position
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    for (size_t i = 0; i < gfx_structs.size; i++) {
        gfx_struct_t *gs = store_pget(&gfx_structs, i);
        if (mouse_x >= gs->x && mouse_x < gs->x + NAME_WIDTH + VALUE_WIDTH &&
                mouse_y >= gs->y && mouse_y < gs->y + CELL_HEIGHT * gs->vis->fields.size) {
            size_t field_idx = (mouse_y - gs->y) / CELL_HEIGHT;
            vis_field_t *field = store_pget(&gs->vis->fields, field_idx);
            _render_popup(mouse_x, mouse_y, field->description);
            break;
        }
    }

    

    // Update screen
    SDL_RenderPresent(renderer);
}

void gfx_loop()
{
    vis_struct_t *coff_file_header = vis_find_struct("COFF File Header");
    gfx_struct_t *gs = store_alloc(&gfx_structs);
    gs->vis = coff_file_header;
    gs->x = 20;
    gs->y = 20;


    bool done = false;
    while (!done) {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            switch (evt.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;

                case SDL_KEYDOWN:
                    if ((evt.key.keysym.mod & KMOD_ALT) && evt.key.keysym.sym == SDLK_F4) {
                        done = true;
                    }
                    if ((evt.key.keysym.sym == SDLK_ESCAPE)) {
                        done = true;
                    }
                    break;
            }
        }


        gfx_render();
    }

}