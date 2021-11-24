/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2021 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    setBubble

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    Info << "received " << argc - 1 << " arguments" << endl;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            Info << "received argument: " << argv[i] << endl;
        }
    }

    #include "setRootCase.H"    // deals with the command line arguments and provides generic help functionality
                                // creates an openfoam Foam::argList object args

    // let's add an argument to switch of initialization of p_rgh
    argList::addBoolOption(
        "noWritePrgh",
        "do not initialise p_rgh"
    );

    #include "createTime.H"     // deals with all time related things but also writing stuff out
                                // creates an openfoam Foam::Time object runTime

    #include "createMesh.H"     // creates an openfoam Foam::fvMesh object of name mesh    

    #include "createFields.H"   // create fields p, p_rgh, alpha, T

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    // From the documentation: https://cpp.openfoam.org/v9/classFoam_1_1fvMesh.html#a0fbf3f470cb51bdbb754bf72e736ae12
    // returns a reference to the mesh cell centres (not a pointer!)
    const volVectorField& meshCellCentres = mesh.C();

    // create a dimensioned scalar from the p.dimensions() and pL and assign it to p
    // same goes for T
    p = dimensioned<scalar>("pL", p.dimensions(), pL);
    T = dimensioned<scalar>("TL", T.dimensions(), TL);
    
    p_rgh == p;    // use the == operator to make sure, the values are also set on the boundary faces
                   // this is because fixedValue boundary conditions do not implement a = operator

    forAll(meshCellCentres, celli){

        // iterate over all cell centres and if the distance between the cell
        // centre and the bubble radius is smaller than the bubble radius, 
        // we are inside the bubble and we can set our fields

        /*
        if (mag(meshCellCentres[celli] - bubbleCentre) < bubbleRadius) {
            alpha[celli] = 0;
            p[celli] = pBubble;
            T[celli] = TBubble;
        }

        if (meshCellCentres[celli].y() > freeSurfaceHeight) {
            alpha[celli] = 0;
        }
        */

        // more elegant iteration
        // first create a reference to the current cell
        const vector& Ci = meshCellCentres[celli];
                // inside the bubble?
        if ( mag(Ci - centre) < radius) {
            alpha[celli] = 0;       // air -> alpha = 0
            T[celli] = TB;     // set bubble temperature
            p[celli] = pB;     // set bubble pressure
        }
        
        if ( Ci.y() > freeSurfaceHeight ) {
            alpha[celli] = 0;       // air -> alpha = 0
        }
    }

    //alpha.write();
    runTime.writeNow();

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
