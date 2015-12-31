#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "error.h"
#include "vis_struct.h"
#include "text.h"

#define NAME_WIDTH 200
#define VALUE_WIDTH 300
#define CELL_HEIGHT 20
#define POPUP_BORDER_WIDTH 7

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
    font = TTF_OpenFont("../fonts/DejaVuSansMono.ttf", 15);
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
    SDL_Rect outer_rect = { x, y, 200, 100 };
    SDL_Rect inner_rect = {
        x + POPUP_BORDER_WIDTH - 1,
        y + POPUP_BORDER_WIDTH - 1,
        200 - 2 * (POPUP_BORDER_WIDTH - 1),
        100 - 2 * (POPUP_BORDER_WIDTH - 1),
    };

    _set_color(popup_border_color);
    SDL_RenderFillRect(renderer, &outer_rect);

    _set_color(popup_bg_color);
    SDL_RenderFillRect(renderer, &inner_rect);

    _set_color(border_color);
    SDL_RenderDrawRect(renderer, &outer_rect);
    SDL_RenderDrawRect(renderer, &inner_rect);

    //size_t i = text_get_line(text, 0, 10);
    //*((char *)text + i) = '\0';
    _render_text(text, x + 12, y + 12);
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
    vis_struct_t *coff_file_header = vis_find_struct("COFF File Header");
    _render_vis_struct(coff_file_header, 20, 20);

    _render_popup(150, 200,
        "Long description for test: "
        "The preferred address of the first byte of image when loaded into memory; "
        "must be a multiple of 64 K. The default for DLLs is 0x10000000. The default "
        "for Windows CE EXEs is 0x00010000. The default for Windows NT, Windows 2000, "
        "Windows XP, Windows 95, Windows 98, and Windows Me is 0x00400000. "
        "The alignment (in bytes) of sections when they are loaded into memory. It "
        "must be greater than or equal to FileAlignment. The default is the page size "
        "for the architecture. "
        "The alignment factor (in bytes) that is used to align the raw data of "
        "sections in the image file. The value should be a power of 2 between 512 and "
        "64 K, inclusive. The default is 512. If the SectionAlignment is less than the "
        "architecture’s page size, then FileAlignment must match SectionAlignment. "
    );

    // Update screen
    SDL_RenderPresent(renderer);
}

void gfx_loop()
{
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