#ifndef D3B5177F_3566_4C3A_BFE2_DA9C2B4E282F
#define D3B5177F_3566_4C3A_BFE2_DA9C2B4E282F

#include "user_defined.h"
#include "base.h"
#include <random>

struct EvoState {
    short survive; // flags from 0 to 8 for allowed neighbors to survive
    short create_new; // flags from 0 to 8 for allowed neighbors to be born
    double mutate_chance; // percent chance of mutations
    short life_length;
    bool refractory;
    bool alive;

    EvoState(bool random=false);
    EvoState(const EvoState &to_copy);

    bool survives(int count);
    bool is_born(int count);
    bool is_alive();
    bool operator==(const EvoState& rhs);
    bool operator!=(const EvoState& rhs);
    EvoState mutate(bool reset=true);
};

class EvoRule : public Rule<EvoState> {
    private:
        std::mt19937 gen;
        int num_allowed;
    public:
        EvoRule(int num_allowed);
        virtual EvoState process(const Cell<EvoState>& cell);
};

#endif /* D3B5177F_3566_4C3A_BFE2_DA9C2B4E282F */
