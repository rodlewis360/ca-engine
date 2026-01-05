#include "ant.h"
#include "langtons_ant.h"
#include "base.h"
#include <string>

// Directions - n:0, e:1, s:2, w:3
// Remember negative y is up
int* LangtonsAnt::process(Grid<LangtonsState>& grid) {
    if (dead) return old_pos;
    old_pos[0] = x;
    old_pos[1] = y;
    Cell<LangtonsState> current_cell = grid.get_cell(x, y);

    if (current_cell.m->state == 0) {
        direction = (direction + 1) % 4;
    } else {
        direction -= 1;
        if (direction < 0) {
            direction += 4;
        }
    }

    // Swap the current cell
    current_cell.m->state = (current_cell.m->state + 1) % 2;
    current_cell.m->is_ant = false;

    // Move in the current direction
    switch(direction) {
        case 0:
            y -= 1;
            break;
        case 1:
            x += 1;
            break;
        case 2:
            y += 1;
            break;
        case 3:
            x -= 1;
            break;
    }

    if (x < 0 || y < 0 || x >= grid.xsize() || y >= grid.ysize()) {
        dead = true;
        return old_pos;
    }

    // Set new cell as ant
    grid.get_cell(x, y).m->is_ant = true;

    old_pos[2] = x;
    old_pos[3] = y;

    return old_pos;
}

int* LangtonsExtensibleAnt::process(Grid<LangtonsState>& grid) {
    if (dead) return old_pos;
    old_pos[0] = x;
    old_pos[1] = y;
    Cell<LangtonsState> current_cell = grid.get_cell(x, y);

    if (current_cell.m->state < 0 || current_cell.m->state >= direction_rules_size) {
        throw std::out_of_range("Cell state out of range for LangtonsExtensibleAnt");
    }

    direction = (direction + direction_rules[current_cell.m->state]) % 4;

    // Swap the current cell
    current_cell.m->state = (current_cell.m->state + 1) % direction_rules_size;
    current_cell.m->is_ant = false;

    // Move in the current direction
    switch(direction) {
        case 0:
            y -= 1;
            break;
        case 1:
            x += 1;
            break;
        case 2:
            y += 1;
            break;
        case 3:
            x -= 1;
            break;
    }

    if (x < 0 || y < 0 || x >= grid.xsize() || y >= grid.ysize()) {
        dead = true;
        return old_pos;
    }

    // Set new cell as ant
    grid.get_cell(x, y).m->is_ant = true;

    old_pos[2] = x;
    old_pos[3] = y;

    return old_pos;
}

std::pair<std::pair<int, int>, std::pair<int,int>> LangtonsExtensibleAnt::processPair(Grid<LangtonsState>& grid) {
    if (dead) return std::make_pair(std::make_pair(x, y), std::make_pair(x, y));
    old_pos[0] = x;
    old_pos[1] = y;
    Cell<LangtonsState> current_cell = grid.get_cell(x, y);

    if (current_cell.m->state < 0 || current_cell.m->state >= direction_rules_size) {
        throw std::out_of_range("Cell state out of range for LangtonsExtensibleAnt");
    }

    direction = (direction + direction_rules[current_cell.m->state]) % 4;

    // Swap the current cell
    current_cell.m->state = (current_cell.m->state + 1) % direction_rules_size;
    current_cell.m->is_ant = false;

    // Move in the current direction
    switch(direction) {
        case 0:
            y -= 1;
            break;
        case 1:
            x += 1;
            break;
        case 2:
            y += 1;
            break;
        case 3:
            x -= 1;
            break;
    }

    if (x < 0 || y < 0 || x >= grid.xsize() || y >= grid.ysize()) {
        dead = true;
        return std::make_pair(std::make_pair(old_pos[0], old_pos[1]), std::make_pair(old_pos[2], old_pos[3]));
    }

    // Set new cell as ant
    grid.get_cell(x, y).m->is_ant = true;

    old_pos[2] = x;
    old_pos[3] = y;

    return std::make_pair(std::make_pair(old_pos[0], old_pos[1]), std::make_pair(old_pos[2], old_pos[3]));
}