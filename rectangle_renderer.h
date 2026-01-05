#ifndef E3A02B11_7AD7_4E31_979A_84D9E95F6BA2
#define E3A02B11_7AD7_4E31_979A_84D9E95F6BA2

#include <SDL3/SDL.h>
#include <functional>
#include <string>
#include <set>
#include "user_defined.h"
#include "base.h"

#define USING_SDL

struct Color {
    int r, g, b, a;
    Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 0;
    }

    Color(int r, int g, int b, int a=255) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};

struct GraphicsCell {
    float pos_x, pos_y, width, height;
    Color color;
    GraphicsCell(float pos_x, float pos_y, float width, float height, Color color) {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
        this->width = width;
        this->height = height;
        this->color = color;
    }
    GraphicsCell() {
        this->pos_x = 0;
        this->pos_y = 0;
        this->width = 0;
        this->height = 0;
        this->color = Color();
    }
};

template <class T>
class RectangleRenderer : public Renderer<T> {
    private:
        Color bg_color;
        float cell_size;
        function<Color(T)> conversion;

    public:
        RectangleRenderer(Color bg_color, const int window_width, const int window_height, const int grid_width, const int grid_height, function<Color(T)> conversion) : Renderer() {
            this->bg_color = bg_color;
            this->cell_size= 1.0 * min(window_width, window_height) / grid_width;
            this->conversion = conversion;
        }

        virtual void render(Grid<T> &grid) {
            Color to_render;
            GraphicsCell *cells = new GraphicsCell[grid.xsize() * grid.ysize()];

            for (int i = 0; i < grid.ysize(); i++) {
                for (int j = 0; j < grid.xsize(); j++) {
                    to_render = conversion(grid.get_value(j,i));
                    cells[i * grid.ysize() + j] = GraphicsCell((i * cell_size),
                                                               (j * cell_size),
                                                               cell_size, cell_size, Color(to_render));
                }
            }

            final_render(cells, bg_color);
            delete[] cells;
        }

        #ifdef TARGETED_RENDER

        virtual void targeted_render(Grid<T> &grid, set<pair<int,int>> targets) {
            Color to_render;
            GraphicsCell *cells = new GraphicsCell[targets.size()];

            int index = 0;
            for (pair<int,int> target : targets) {
                to_render = conversion(grid.get_value(target.first, target.second));
                cells[index] = GraphicsCell((target.first * cell_size),
                                            (target.second * cell_size),
                                            cell_size, cell_size, Color(to_render));
                index++;
            }

            targeted_final_render(cells, targets.size());

            delete[] cells;
        }

        #endif // TARGETED_RENDER
};

#endif /* E3A02B11_7AD7_4E31_979A_84D9E95F6BA2 */
