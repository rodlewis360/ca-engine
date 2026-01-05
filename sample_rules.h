#ifndef A89C2F82_1620_4BF6_B6C9_70739BAF7464
#define A89C2F82_1620_4BF6_B6C9_70739BAF7464

#include "user_defined.h"
#include "base.h"

using namespace std;

class LifeRule : public Rule<int> {
    public:
        virtual int process(const Cell<int>& cell); 
};

class BriansBrainRule : public Rule<int> {
    public:
        virtual int process(const Cell<int>& cell); 
};

class ForestFireRule : public Rule<int> {
    public:
        virtual int process(const Cell<int>& cell);
};

#endif /* A89C2F82_1620_4BF6_B6C9_70739BAF7464 */