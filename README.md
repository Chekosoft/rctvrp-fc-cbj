# Forward Checking with Constraint Backwards Jumping algorithm for the RCTVRP

This is an implementation of one of the algorithms proposed by Prosser (1993) to solve
the Risk-constrained Cash-in Vehicle Routing Problem.

# How to Compile / Requirements

Executing `make` should be fine. This program was developed using the C11 standard and compiled
using the GNU C Compiler (gcc) version 7.2.1 (20171128). It also uses the pthread library (because
the used logger library depends on, but it's not used for resolution purposes).

There's also `make clean`, which cleans all compilation artifacts.
Also, there is `make watch`, which watches source files for changes and auto
`make watch` requires having the [`entr(1)`](http://entrproject.org/) program installed, otherwise it will fail.

# How to execute
After compiling, you can execute with the following command:

`./out/rctvrp -f [sample/instance.txt]`

Where the -f flag determines the path of the instance file to solve.
Various examples are in the `./tests/` folder.
