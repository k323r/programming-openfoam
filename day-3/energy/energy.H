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

Class
    Foam::functionObjects::energy

Description
    This function object...

    Example of function object specification:
    \verbatim
    energy1
    {
        type           energy;
        libs ("libenergyFunctionObject.so");
        ...
        wordData       someWord;
        scalarData     1.0;
        labelData      1;
    }
    \endverbatim

Usage
    \table
        Property     | Description               | Required | Default value
        type         | type name: energy | yes      |
        wordData     | some word option...       | no       | defaultWord
        scalarData   | some scalar value...      | yes      |
        labelData    | some label value...       | yes      |
    \endtable

SourceFiles
    energy.C

\*---------------------------------------------------------------------------*/

#ifndef energy_H
#define energy_H

#include "fvMeshFunctionObject.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class energy Declaration
\*---------------------------------------------------------------------------*/

// const DimensionedField<scalar, volMesh>& V = mesh.V();

class energy
:
    public fvMeshFunctionObject
{
    // Private Data

        //- word
        word UName_;

        //- scalar
        scalar rho_;

        // output file stream
        OFstream file_;

    // Private Member Functions


public:

    //- Runtime type information
    TypeName("energy");


    // Constructors

        //- Construct from Time and dictionary
        energy
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- Disallow default bitwise copy construction
        energy(const energy&) = delete;


    //- Destructor
    virtual ~energy();


    // Member Functions

        //- Read the energy data
        virtual bool read(const dictionary&);

        //- Execute, currently does nothing
        virtual bool execute();

        //- Execute at the final time-loop, currently does nothing
        virtual bool end();

        //- Write the energy
        virtual bool write();


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const energy&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
