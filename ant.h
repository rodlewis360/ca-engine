#ifndef CCB78913_9E42_4832_9C9E_C7A083E51DE2
#define CCB78913_9E42_4832_9C9E_C7A083E51DE2

#include "base.h"

template <class T>
class Ant {
    protected:
        int direction;
        bool dead;
    public:
        int x, y;

        Ant(int start_x, int start_y, int starting_direction) {
            this->x = start_x;
            this->y = start_y;
            this->direction = starting_direction;
        }

        virtual int* process(Grid<T>& grid) {
            return nullptr;
        }
        virtual void process_n(Grid<T>& grid, int n) {
            for (int i = 0; i < n; i++) {
                process(grid);
            }
        }
};

#endif /* CCB78913_9E42_4832_9C9E_C7A083E51DE2 */
