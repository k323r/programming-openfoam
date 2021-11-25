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

\*---------------------------------------------------------------------------*/

#include "energy.H"
#include "Time.H"
#include "fvMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(energy, 0);
    addToRunTimeSelectionTable(functionObject, energy, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::energy::energy
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),
    UName_(dict.lookupOrDefault<word>("UName", "U")),
    rho_(dict.lookup<scalar>("rho")),
    file_(obr_.time().rootPath()/obr_.time().globalCaseName()/name+".dat")
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::energy::~energy()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::energy::read(const dictionary& dict)
{
    // does not make sense to change input parameters during run time
    // dict.readIfPresent("UName", UName_);
    // dict.lookup("rho") >> rho_;

    return true;
}


bool Foam::functionObjects::energy::execute()
{
    return true;
}


bool Foam::functionObjects::energy::end()
{
    return true;
}


bool Foam::functionObjects::energy::write()
{
    // 1. lookup velocity field from the object registry
    // 2. loop over all cells and calculate mag(sqr(velocity))
    // 3. sum up

    // the objec registry is made availabel as a protected reference
    const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);
    
    // we do not need to loop over all cells as OpenFOAM can do field algebra
    scalar KE = 0.0; 

    // for a compressible simulation we need to retrieve rho from the object registry
    if (obr_.foundObject<volScalarField>("rho"))
    {
        const volScalarField& rho = obr_.lookupObject<volScalarField>("rho");
        KE = 0.5 * fvc::domainIntegrate(rho * magSqr(U)).value();
    }
    else
    {
        KE = 0.5 * rho_ * fvc::domainIntegrate(magSqr(U)).value();
    }

    Info << "KE = " << KE << nl << endl;

    file_ << obr_.time().timeOutputValue() << tab << KE << endl;

    return true;
}


// ************************************************************************* //
