#include <time.h>
#include <stdlib.h>
#include "IOstreams.H"
#include "fieldTypes.H"
#include "Field.H"

using namespace Foam;

#define LIMIT 1000000000

int main() {

    Info << "fibonacci2::main()" << endl;

    Field<label> fib(2, 1);      // creates a list with two elements with value 1
    clock_t start, end = 0;     // used to measure computation time

    Info << "fib list: " << fib << endl; // will return 2{1} meaning a list with two elements with value 1

    start = clock();
    while (fib.last() < LIMIT) {
        fib.append( fib[fib.size() - 2] + fib.last());
    }
    end = clock();

    scalar dt = 1000.0*(end - start) / CLOCKS_PER_SEC;

    Info << "calculated " << fib.size() << " elements in " << dt << " seconds" << endl;
    Info << "fibonacci series: " << fib << endl;

    // doubling values
    forAll(fib, i){
        fib[i] *= 2;
    }

    // more intelligent approach: use openfoam field algebra
    fib *= 2;

    return EXIT_SUCCESS;
}
