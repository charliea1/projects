Evolution Simulator:

compiling: "make" to compile and "make clean" to clean bin directory (clean does not change executable)
running: $./simulator runs the simulation as is; $./simulator -name- creates unique output file "SIM-name"

This simulator works with "entities" that move on a simple 2D board. Entities move randomly, searching for food. If they find a specified amount of food, they will reproduce with a specified chance of mutation. If 2+ entities find food at the same time, all but one will yield, or some may "fight" for the food. Entities are helped/limited by these stats:

aggression: willingness to fight for food

attack: chance of success in fight

defense: chance of surviving lost fight

The simulator runs in stages of adjustable size, broken into turns, iterations, and generations.

A turn consists of movement, eating/confrontations, and reproduction.

An iteration is a specified amount of turns to deem a "generation." This may seem unintuitive, but consider that to emulate evolution, the chance of mutation must be kept low. This means that many more generations (in the evolutionary sense) are required to run to see actual changes. Breaking simulation generations into iterations allows for these changes to be viewed without sifting through as much text.

A generation contains a specified number of iterations; at the end of a generation, the stats of living entities will be printed to an output file.

Classes:

-Entities are the "players" that can move, eat, and reproduce
-GameState creates the board and manages entity v entity interaction and entity v board interaction
