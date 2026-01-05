#ifndef CF345173_79D8_4D73_A67B_76ACD47A1966
#define CF345173_79D8_4D73_A67B_76ACD47A1966

#include "base.h"
#include "user_defined.h"
#include "evo_rule.h"
#include <SDL3/SDL.h>

template <class T>
void run_iter(Grid<T>& grid, Rule<T>& rule) {
    Grid<T> copy = grid;

    for (int i = 0; i < grid.size(); i++) {
        *(grid[i].m) = rule.process(copy[i]);
    }
}

template <class T>
void animate(Grid<T>& grid, Rule<T>& rule, Renderer<T>& renderer, int n) {
    for (int i = 0; i < n; i++) {
        run_iter(grid, rule);
        renderer.render(grid);
    }
}

template <class T>
void run_iter_n(Grid<T>& grid, Rule<T>& rule, int n) {
    for (int i = 0; i < n; i++) {
        run_iter(grid, rule);
    }
}

#endif /* CF345173_79D8_4D73_A67B_76ACD47A1966 */
