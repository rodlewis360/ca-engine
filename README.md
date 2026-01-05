This is a cellular automaton engine written in C++ using SDL for rendering.

NOTE - the vendored/ directory is not included in this repo because of the size of the SDL3 library. SDL3 must be added under the vendored/ directory before this code can be compiled.

## Example Builds
There are 5 example builds included in this project under the example_builds/ directory:

- `evo_rule.exe` - evolution of elementary 2D cellular automata (think variations on Conway's Game of Life) through natural selection. Starts in a random configuration each time, where each different color is a different "species" or elementary 2D CA ruleset.
- `life_rule.exe` - Conway's Game of Life example with random starting configuration.
- `forest_fire.exe` - Forest fire simulation example (modified from B. Drossl and F. Schwabl - "Self-Organized Critical Forest-Fire Model" (1992))
- `langtons_ant.exe` - Langton's Ant example
- `langtons_ant_variations.exe` - Variations on Langton's Ant with a random starting configuration (originally proposed by Greg Turk and Jim Propp)


## Architecture
There are 3 main components to this CA engine: `Grid`s (with `Cell`s), `Rule`s, and `Renderer`s.

### Grids
A `Grid` holds all of the Cells and connects them to their neighborhood. It is also what will be processed and rendered by the `Renderer`.

A `Grid` is defined with a class that is used as the state of all of the `Cells`. In many cases, this is an `int`, but when more complex states need to be stored, other classes and structs can be used (like `EvoState` for the `EvoRule`).

Besides the state class, each `Grid` has dimensions and an option to turn on wrapping (which is still under development).

Each `Cell` in the default `Grid` has a pointer to each of its neighbors' states in its Moore neighborhood, which are used by `Rule`s to determine the next state of the cell.

### Rules
A `Rule` is fairly simple - the only thing required of a rule object is a method that takes in a `Cell` that contains a state of type `T` and returns the next state of the `Cell`. It does this using the states of the cell's neighbors. Each `Rule` object should extend the `Rule` class from `user_defined.h`.

For example, the `LifeRule` (Conway's Game of Life) would add up the values of all the neighbors' states, then return 1 if its own state is 1 and the sum is 2 or 3, or if its own state is 0 and the sum is exactly 3, and otherwise would return 0.

### Renderers
A `Renderer` contains a function called `render` that accepts a Grid containing Cells with type T and renders the Grid in its current state. Each Renderer should extend the Renderer class from `user_defined.h`

Often, `Renderer`s will accept a conversion function in their constructors as well, to allow the user to have the same Renderer represent different `Rule`s differently.

### Running it All
The engine provides a method for animation with processes that don't require their own control loop (like text output) - just use `animate` from engine.h and plug in the `Grid`, the `Rule`, the `Renderer`, and the number of iterations/generations to animate.

If you need to make your own control loop, you can call `run_iter` instead with the Grid and the Rule, then use `renderer.render` to render the grid manually. This method is used for the SDL renderer used for all of the examples in this project, since it's useful to use the SDL callbacks for control loops.
