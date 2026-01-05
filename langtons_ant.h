#ifndef CB2B7446_183F_43A4_9EE1_56D3170A46B8
#define CB2B7446_183F_43A4_9EE1_56D3170A46B8

#include "base.h"
#include "ant.h"

struct LangtonsState {
    int state;
    bool is_ant;

    LangtonsState() {
        this->state = 0;
        this->is_ant = false;
    }
};

class LangtonsAnt : public Ant<LangtonsState> {
    private:
        int old_pos[4];
    public:
        virtual int* process(Grid<LangtonsState>& grid);
        LangtonsAnt(int start_x, int start_y, int starting_direction) : Ant<LangtonsState>(start_x, start_y, starting_direction) {
            this->dead = false;
        }
};

class LangtonsExtensibleAnt : public Ant<LangtonsState> {
    private:
        int old_pos[4];
        int* direction_rules;
        int direction_rules_size;
    public:
        virtual int* process(Grid<LangtonsState>& grid);
        virtual std::pair<std::pair<int, int>, std::pair<int,int>> processPair(Grid<LangtonsState>& grid);
        LangtonsExtensibleAnt(int start_x, int start_y, int starting_direction, int* direction_rules, int direction_rules_size) : Ant<LangtonsState>(start_x, start_y, starting_direction) {
            this->dead = false;
            this->direction_rules = direction_rules;
            this->direction_rules_size = direction_rules_size;
        }

        LangtonsExtensibleAnt(LangtonsExtensibleAnt& other) : Ant<LangtonsState>(other.x, other.y, other.direction) {
            delete[] direction_rules;
            this->dead = other.dead;
            this->direction_rules_size = other.direction_rules_size;
            this->direction_rules = new int[direction_rules_size];
            for (int i = 0; i < direction_rules_size; i++) {
                this->direction_rules[i] = other.direction_rules[i];
            }
        }

        LangtonsExtensibleAnt() : Ant<LangtonsState>(0, 0, 0) {
            this->dead = false;
            this->direction_rules = new int[2] {1, 3}; // Default to Langton's Ant rules
            this->direction_rules_size = 2;
        }

        ~LangtonsExtensibleAnt() {
            delete[] direction_rules;
        }

        void operator=(LangtonsExtensibleAnt& other) {
            delete[] direction_rules;
            this->dead = other.dead;
            this->x = other.x;
            this->y = other.y;
            this->direction = other.direction;
            this->direction_rules_size = other.direction_rules_size;
            this->direction_rules = new int[direction_rules_size];
            for (int i = 0; i < direction_rules_size; i++) {
                this->direction_rules[i] = other.direction_rules[i];
            }
        }
};

#endif /* CB2B7446_183F_43A4_9EE1_56D3170A46B8 */
