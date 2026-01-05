#include "sample_rules.h"
#include <random>

int LifeRule::process(const Cell<int>& cell) {
    int total = *(cell.nw) + *(cell.n) + *(cell.ne) 
                + *(cell.w)              + *(cell.e) 
                + *(cell.sw) + *(cell.s) + *(cell.se);
    
    if (*(cell.m)) {
        return total == 2 || total == 3;
    }
    return total == 3;
}

int BriansBrainRule::process(const Cell<int>& cell) {
    int total;
    switch (*(cell.m)) {
        case 0:
            total =   (*(cell.nw) == 1) + (*(cell.n) == 1) + (*(cell.ne) == 1) 
                    + (*(cell.w)  == 1)                    + (*(cell.e)  == 1) 
                    + (*(cell.sw) == 1) + (*(cell.s) == 1) + (*(cell.se) == 1);
            return total == 2;
        case 1:
            return 2;
        case 2:
            return 0;
        default:
            return 0;
    }
}


int treeCount(int value) {
    if (value > 0 && value <= 128) {
        return value;
    }
    return 0;
}

int ForestFireRule::process(const Cell<int>& cell) {
    if (*(cell.m) == 255) return 254; // fire dying
    // Fire state is 255, Tree states are 1-128, Dead ground states are 129-254, and empty ground state is 0.
    int total_fire =   (*(cell.nw) == 255) + (*(cell.n) == 255) + (*(cell.ne) == 255) 
                     + (*(cell.w)  == 255)                      + (*(cell.e)  == 255) 
                     + (*(cell.sw) == 255) + (*(cell.s) == 255) + (*(cell.se) == 255);

    int total_tree = treeCount(*(cell.nw)) + treeCount(*(cell.n)) + treeCount(*(cell.ne)) 
                   + treeCount(*(cell.w))                         + treeCount(*(cell.e))
                   + treeCount(*(cell.sw)) + treeCount(*(cell.s)) + treeCount(*(cell.se));

    if (*(cell.m) > 0 && *(cell.m) <= 128) {
        if ((1.0 * rand() / RAND_MAX > 0.99995 || total_fire > 0) && 1.0 * rand() / RAND_MAX < 0.8) {
            return 255; // lightning strike (starts fire)
        }
        if (*(cell.m) == 128) return 128; // full grown tree
        else if (*(cell.m) < 128) return *(cell.m) + 1;
    }
    else if (*(cell.m) == 129) return 0; // burned ground to ground
    else if (*(cell.m) > 128 && *(cell.m) < 255) {
        // Formula for chance of growing new tree: total of ages of trees + clearing ground status of current cell, scaled between 0 and 1 and 
        if (0.75 * rand() / RAND_MAX > (0.833 - ((total_tree + *(cell.m) - 128) / (128.0 * 9.0)))) return 1; // new tree
        return max(*(cell.m) - (int)((1.0 * rand() / RAND_MAX) * 10), 129); // clearing ground
    }

    if (0.75 * rand() / RAND_MAX > (0.833 - (total_tree / (128.0 * 8.0)))) return 1; // new tree
    
    return 0;
}