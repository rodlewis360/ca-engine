#include <random>
#include <SDL3/SDL.h>
#include <string>
#include <bitset>
#include <set>
#include <numeric>
#include <random>
#include "evo_rule.h"
#include "base.h"

using namespace std;

EvoRule::EvoRule(int num_allowed) {
    this->num_allowed = num_allowed;
}

bool EvoState::operator==(const EvoState &rhs) {
    return rhs.survive == survive && rhs.create_new == create_new;
}

bool EvoState::operator!=(const EvoState &rhs) {
    return !(*this == rhs);
}

EvoState::EvoState(bool random) {
    if (!random || 1.0 * rand() / RAND_MAX < 0.995) {
        survive = 0;
        create_new = 0;
        mutate_chance = 0;
        alive = false;
    } else {
        survive = (1.0 * rand() / RAND_MAX) * 512;
        create_new = (1.0 * rand() / RAND_MAX) * 512;
        double mutate_chance_1 = 1.0 * rand() / RAND_MAX;
        mutate_chance = (mutate_chance_1 * mutate_chance_1 * mutate_chance_1);
        alive = true;
    }
    life_length = 1;
    refractory = false;
}

EvoState::EvoState(const EvoState &to_copy) {
    this->survive = to_copy.survive;
    this->create_new = to_copy.create_new;
    this->mutate_chance = to_copy.mutate_chance;
    this->refractory = to_copy.refractory;
    this->alive = to_copy.alive;
    this->life_length = to_copy.life_length;
}

bool EvoState::survives(int count) {
    switch (count) {
        case 0:
            return survive & 0b1;
        case 1:
            return survive & 0b10;
        case 2:
            return survive & 0b100;
        case 3:
            return survive & 0b1000;
        case 4:
            return survive & 0b10000;
        case 5:
            return survive & 0b100000;
        case 6:
            return survive & 0b1000000;
        case 7:
            return survive & 0b10000000;
        case 8:
            return survive & 0b100000000;
        default:
            return false;
    }
}

bool EvoState::is_born(int count) {
    switch (count) {
        case 0:
            return create_new & 0b1;
        case 1:
            return create_new & 0b10;
        case 2:
            return create_new & 0b100;
        case 3:
            return create_new & 0b1000;
        case 4:
            return create_new & 0b10000;
        case 5:
            return create_new & 0b100000;
        case 6:
            return create_new & 0b1000000;
        case 7:
            return create_new & 0b10000000;
        case 8:
            return create_new & 0b100000000;
        default:
            return false;
    }
}

EvoState EvoState::mutate(bool reset) {
    EvoState output = *this;
    if (1.0 * rand() / RAND_MAX < mutate_chance) {
        if (rand() % 3) {
            if (rand() % 2) {
                output.survive += (1.0 * (rand() - (RAND_MAX / 2)) / (RAND_MAX / 2)) * 512;
            } else {
                output.create_new += (1.0 * (rand() - (RAND_MAX / 2)) / (RAND_MAX / 2)) * 512;
            }
        } else {
            output.mutate_chance += (1.0 * (rand() - (RAND_MAX / 2)) / (RAND_MAX / 2));
            if (output.mutate_chance > 1.0) {
                output.mutate_chance = 1.0;
            }
        }
    }
    if (reset) output.life_length = 1;
    output.alive = true;
    return output;
}

EvoState EvoRule::process(const Cell<EvoState>& cell) {
    EvoState old = *(cell.m);

    if (cell.m->refractory) return EvoState();

    EvoState neighbors[8] = {*(cell.nw), *(cell.n), *(cell.ne), *(cell.w), *(cell.e), *(cell.sw), *(cell.s), *(cell.se)};

    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (neighbors[i].alive) count++;
    }

    vector<EvoState> valid_neighbors;
    vector<short> neighbor_weights; 
    if (cell.m->survives(count) && !cell.m->refractory && cell.m->alive && __popcnt(cell.m->survive) + __popcnt(cell.m->create_new) <= num_allowed) {
        valid_neighbors.push_back(old);
    } else if (!cell.m->survives(count) && cell.m->alive) {
        old.refractory = true;
        old.alive = false;
        old.life_length++;
        valid_neighbors.push_back(old);
    }

    for (int i = 0; i < 8; i++) {
        if (neighbors[i].is_born(count) && !neighbors[i].refractory && !(neighbors[i] == *(cell.m)) && neighbors[i].alive && __popcnt(neighbors[i].survive) + __popcnt(neighbors[i].create_new) <= num_allowed) {
            valid_neighbors.push_back(neighbors[i]);
            neighbor_weights.push_back(neighbors[i].life_length);
        }
    }

    if (valid_neighbors.size() > 1) {
        discrete_distribution<int> dist = discrete_distribution<int>(neighbor_weights.begin(), neighbor_weights.end());
        EvoState output = valid_neighbors[dist(gen)];
        return output.mutate(output == old);
    } else if (valid_neighbors.size() == 1) {
        EvoState output = valid_neighbors[0];
        return output.mutate(output == old);
    } else {
        return EvoState();
    }
}