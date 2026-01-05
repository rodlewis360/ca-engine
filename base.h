#ifndef FAAFD2BA_67C0_4E75_94B8_9BFFA3594785
#define FAAFD2BA_67C0_4E75_94B8_9BFFA3594785

#include <ostream>
#include <SDL3/SDL.h>

#define DEBUG 0

template <class T>
struct Cell {
    T *nw, *n, *ne, 
      *w,  *m, *e, 
      *sw, *s, *se;

    Cell(T *nw, T *n, T *ne, T *w, T *m, T *e, T *sw, T *s, T *se) {
        this->nw = nw;
        this->n = n;
        this->ne = ne;
        this->w = w;
        this->m = m;
        this->e = e;
        this->sw = sw;
        this->s = s;
        this->se = se;
    }

    Cell() {
        this->nw = nullptr;
        this->n = nullptr;
        this->ne = nullptr;
        this->w = nullptr;
        this->m = nullptr;
        this->e = nullptr;
        this->sw = nullptr;
        this->s = nullptr;
        this->se = nullptr;
    }
};

template <class T>
class Grid {
    private:
        int x_size, y_size;
        T *values;
        Cell<T> *cells;
        T dummy;
        bool wrap;

        virtual Cell<T>* generate_cells() {
            Cell<T> *processing_cells = new Cell<T>[size()];
            T *processing_array[9];
            dummy = T();
            
            for (int j = 0; j < y_size; j++) {
                for (int i = 0; i < x_size; i++) {
                    for (int k = 0; k < 9; k++) {
                        processing_array[k] = nullptr;
                    }

                    // Handle edge cells
                    if (wrap) {
                        // TODO wrapping doesn't work yet
                        if (i == 0) {
                            if (j == 0) {
                                #if DEBUG
                                    cout << "top left " << endl;
                                #endif DEBUG
                                processing_array[0] = get_value_pointer(x_size - 1, y_size - 1);
                                processing_array[3] = get_value_pointer(x_size - 1, j);
                                processing_array[6] = get_value_pointer(x_size - 1, j + 1);
                                processing_array[1] = get_value_pointer(i    , y_size - 1);
                                processing_array[2] = get_value_pointer(i + 1, y_size - 1);
                            } else if (j == y_size - 1) {
                                #if DEBUG
                                    cout << "bottom left " << endl;
                                #endif DEBUG
                                processing_array[0] = get_value_pointer(x_size - 1, j - 1);
                                processing_array[3] = get_value_pointer(x_size - 1, j);
                                processing_array[6] = get_value_pointer(x_size - 1, 0);
                                processing_array[7] = get_value_pointer(i    , 0);
                                processing_array[8] = get_value_pointer(i + 1, 0);
                            } else {
                                #if DEBUG
                                    cout << "left edge " << endl;
                                #endif DEBUG
                                processing_array[0] = get_value_pointer(x_size - 1, j - 1);
                                processing_array[3] = get_value_pointer(x_size - 1, j);
                                processing_array[6] = get_value_pointer(x_size - 1, j + 1);
                            }
                        } else if (i == x_size - 1) {
                            if (j == 0) {
                                #if DEBUG
                                    cout << "top right " << endl;
                                #endif DEBUG
                                processing_array[0] = get_value_pointer(i - 1, y_size - 1);
                                processing_array[1] = get_value_pointer(i    , y_size - 1);
                                processing_array[2] = get_value_pointer(0, y_size - 1);
                                processing_array[5] = get_value_pointer(0, j);
                                processing_array[8] = get_value_pointer(0, j + 1);
                            } else if (j == y_size - 1) {
                                #if DEBUG
                                    cout << "bottom right " << endl;
                                #endif DEBUG
                                processing_array[2] = get_value_pointer(0, j - 1);
                                processing_array[5] = get_value_pointer(0, j);
                                processing_array[6] = get_value_pointer(i - 1, 0);
                                processing_array[7] = get_value_pointer(i    , 0);
                                processing_array[8] = get_value_pointer(0, 0);
                            } else {
                                #if DEBUG
                                    cout << "right edge " << endl;
                                #endif DEBUG
                                processing_array[2] = get_value_pointer(0, j - 1);
                                processing_array[5] = get_value_pointer(0, j);
                                processing_array[8] = get_value_pointer(0, j + 1);
                            }
                        } else if (j == 0) {
                            #if DEBUG
                                cout << "top edge " << endl;
                            #endif DEBUG
                            processing_array[0] = get_value_pointer(i - 1, y_size - 1);
                            processing_array[1] = get_value_pointer(i    , y_size - 1);
                            processing_array[2] = get_value_pointer(i + 1, y_size - 1);
                        } else if (j == y_size - 1) {
                            #if DEBUG
                                cout << "bottom edge " << endl;
                            #endif DEBUG
                            processing_array[6] = get_value_pointer(i - 1, 0);
                            processing_array[7] = get_value_pointer(i    , 0);
                            processing_array[8] = get_value_pointer(i + 1, 0);
                        }
                    } else {
                        if (i == 0) {
                            processing_array[0] = &dummy;
                            processing_array[3] = &dummy;
                            processing_array[6] = &dummy;
                        }

                        if (i == x_size - 1) {
                            processing_array[2] = &dummy;
                            processing_array[5] = &dummy;
                            processing_array[8] = &dummy;
                        }

                        if (j == 0) {
                            processing_array[0] = &dummy;
                            processing_array[1] = &dummy;
                            processing_array[2] = &dummy;
                        }

                        if (j == y_size - 1) {
                            processing_array[6] = &dummy;
                            processing_array[7] = &dummy;
                            processing_array[8] = &dummy;
                        }
                    }

                    for (int k = 0; k < 9; k++) {
                        if (processing_array[k] == nullptr) {
                            // This is a fancy pair of math equations, but it gets the nw, n, ne, w, m, e, sw, s, se pointers in that order.
                            processing_array[k] = get_value_pointer(
                                i + ((k % 3) - 1), 
                                j + ((k / 3) - 1)
                            );
                        }
                        #if DEBUG > 1
                            cout << *(processing_array[k]) << " ";
                        #endif // DEBUG > 1
                    }

                    processing_cells[j*x_size + i] = Cell<T>(
                        processing_array[0],
                        processing_array[1],
                        processing_array[2],
                        processing_array[3],
                        processing_array[4],
                        processing_array[5],
                        processing_array[6],
                        processing_array[7],
                        processing_array[8]
                    );

                    #if DEBUG
                        Cell<T> cell = processing_cells[j*x_size + i];
                        cout << "(" << i << ", " << j << ") ->";
                        cout << " nw: " << *(cell.nw);
                        cout << " n: " << *(cell.n);
                        cout << " ne: " << *(cell.ne);
                        cout << " w: " << *(cell.w);
                        cout << " m: " << *(cell.m);
                        cout << " e: " << *(cell.e);
                        cout << " sw: " << *(cell.sw);
                        cout << " s: " << *(cell.s);
                        cout << " se: " << *(cell.se);
                        cout << endl;
                    #endif // DEBUG
                }
            }
            return processing_cells;
        }

    public:
        Grid(int x_size, int y_size, bool wrap = true) {
            this->x_size = x_size;
            this->y_size = y_size;
            this->wrap = wrap;

            this->values = new T[x_size * y_size];
            this->cells = this->generate_cells();
        }

        Grid(Grid &to_copy) {
            this->x_size = to_copy.x_size;
            this->y_size = to_copy.y_size;
            this->wrap = to_copy.wrap;
            this->values = new T[this->x_size * this->y_size];

            for (int i = 0; i < this->size(); i++) {
                this->values[i] = to_copy.values[i];
            }

            this->cells = this->generate_cells();
        }

        Cell<T>& operator[](int x) const {
            return cells[x];
        }

        T* get_value_row(int x) const {
            return &(values[y * x_size]);
        }

        Cell<T>& get_cell(int x, int y) const {
            return cells[y * x_size + x];
        }

        T& get_value(int x, int y) const {
            return values[y * x_size + x];
        }

        T* get_value_pointer(int x, int y) const {
            return &(values[y * x_size + x]);
        }

        Cell<T>* begin() {
            return &(cells[0]);
        }

        Cell<T>* end() {
            return &(cells[(x_size * y_size) - 1]) + 1; // We do the fancy math here to avoid accessing inaccesible memory
        }

        int xsize() {
            return x_size;
        }

        int ysize() {
            return y_size;
        }

        int size() {
            return x_size * y_size;
        }

        void set_edges(T edge_value) {
            dummy = edge_value;
        }

        virtual ~Grid() {
            delete[] values;
            delete[] cells;
        }
};

// Grid<EvoState>::Grid<EvoState>(Grid<EvoState>& to_copy) {
//     this->x_size = to_copy.x_size;
//         this->y_size = to_copy.y_size;
//         this->wrap = to_copy.wrap;
//         this->values = new EvoState[this->x_size * this->y_size];

//         for (int i = 0; i < this->size(); i++) {
//             this->values[i] = to_copy.values[i];
//             if (to_copy.values[i].alive) SDL_Log("alive in to copy");
//         }

//         this->cells = this->generate_cells();
// }

#endif /* FAAFD2BA_67C0_4E75_94B8_9BFFA3594785 */