#include "IOstreams.H"
#include "fieldTypes.H"

using namespace Foam;

int main() {

    scalar b = 3.1415;

    vector u(1,2,3);
    vector v(2,3,1);
    tensor T(1,2,3,4,5,6,7,8,9);

    Info << "sqr(b) = " << sqr(b) << endl;
    Info << u << endl;
    Info << v << endl;
    Info << T << endl;

    Info << "b * u = " << b * u << nl
         << "u + v = " << u * v << nl
         << "u ^ v = " << (u ^ v) << nl
         << "u & v = " << (u & v) << nl
         << "T & v = " << (T & u) << nl
         << "u * v = " << (u * v) << endl;

    tensor oP = (u * v);
    Info << oP << endl;
}
