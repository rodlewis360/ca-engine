#ifndef B95A41EA_68B3_4FE3_8520_4BA0B8A5B5C4
#define B95A41EA_68B3_4FE3_8520_4BA0B8A5B5C4

#include "base.h"

template <class T>
class Rule {
    public:
        virtual T process(const Cell<T>& cell) = 0;
};

template <class T>
class Renderer {
    public:
        Renderer() {};
        virtual void render(Grid<T> &grid) = 0;
};

#endif /* B95A41EA_68B3_4FE3_8520_4BA0B8A5B5C4 */
