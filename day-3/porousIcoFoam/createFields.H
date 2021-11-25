Info<< "Reading transportProperties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

// get the viscosity
dimensionedScalar nu
(
    "nu",
    dimViscosity,
    transportProperties.lookup("nu")
);

Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


#include "createPhi.H"

label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, mesh.solutionDict().subDict("PISO"), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());

const labelList& porousCells = mesh.cellZones()[0];

Info << "creating field D" << endl;

// create a field D with the body force due to porosity
// D = nu * d where nu is the viscosity and d is the porosity 
// d = 1000 1/m^2
// Because we don't need boundaries, we will use an internal volScalarField
volSymmTensorField::Internal D 
(
    // IO object
    IOobject
    (
        "D",                    // name
        runTime.timeName(),     // current time
        mesh,                   // belongs to the mesh in the object registry
        IOobject::NO_READ,      // do not read as it will be created here
        IOobject::AUTO_WRITE    // write out to enable restart of simulation
    ),
    mesh,
    dimensionedSymmTensor(
        "D",                    // name of the scalar
        dimViscosity/dimArea,   // dimension of the scalar
        Zero                       // default value
    )
);

// calculate the desired 'permeability'
// within the porous media
// the porosity 1000 is hard coded
//                                  xx, xy, xz, yy, yz, zz
const symmTensor dNu = symmTensor(1000,  0,  0,  1,  0,  1)*nu.value();

// iterate over the cells in the cell zone and set the D value to dNu
forAll(porousCells, celli)
{
    D[porousCells[celli]] = dNu;
}

