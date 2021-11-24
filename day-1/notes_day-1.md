# Day 1 of programming OpenFOAM

## tools

- `wmakeFilesAndOptions` creates a `Make` folder for compiling OpenFOAM projects.
- `foamNewApp` creates a setup for a new application ready to be compiled with `wmake`

## structure of a typical OpenFOAM applications

```C++
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

    #include "createTime.H"     // deals with all time related things but also writing stuff out
                                // creates an openfoam Foam::Time object runTime

    #include "createMesh.H"     // creates an openfoam Foam::fvMesh object of name mesh    

    #include "createFields.H"   // create fields p, p_rgh, alpha, T

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    // writes all fields
    runTime.writeNow();

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
```