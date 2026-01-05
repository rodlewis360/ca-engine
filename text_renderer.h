#ifndef D4903DFA_9401_47BD_80D5_20FBC5749185
#define D4903DFA_9401_47BD_80D5_20FBC5749185

#include "user_defined.h"
#include "base.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <algorithm>

using namespace std;

template <class T>
class TextRenderer : public Renderer<T> {
    private:
        function<string(T)> conversion;

    public:
        TextRenderer(function<string(T)> conversion) {
            this->conversion = conversion;
        };

        virtual void render(Grid<T> &grid) {
            for (int i = 0; i < grid.ysize(); i++) {
                for (int j = 0; j < grid.xsize(); j++) {
                    cout << conversion(grid.get_value(j, i)) << "  ";
                }
                cout << "|" << endl;
            }
            cout << "---------------------------" << endl;
        };
};

#endif /* D4903DFA_9401_47BD_80D5_20FBC5749185 */
