Info << "Reading bubbleProperties\n" << endl;

IOdictionary bubbleProperties(
    IOobject(
        "bubbleProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE));

const dictionary& bubbleDict(bubbleProperties.subDict("bubble"));
const dictionary& liquidDict(bubbleProperties.subDict("liquid"));

const vector centre(bubbleDict.lookup<vector>("centre"));
const scalar radius(bubbleDict.lookup<scalar>("radius"));
      scalar pB(bubbleDict.lookup<scalar>("p"));
      scalar TB(bubbleDict.lookup<scalar>("T"));

const scalar freeSurfaceHeight(liquidDict.lookup<scalar>("height"));
const scalar pL(liquidDict.lookup<scalar>("p"));
const scalar TL(liquidDict.lookup<scalar>("T"));

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

Info<< "Reading field p_rgh\n" << endl;
volScalarField p_rgh(
    IOobject(
        "p_rgh",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        args.optionFound("noWritePrgh") ? IOobject::NO_WRITE : IOobject::AUTO_WRITE
    ),
    mesh);

Info<< "Reading field T\n" << endl;
volScalarField T
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field alpha.water\n" << endl;
volScalarField alpha
(
    IOobject
    (
        "alpha.water",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

alpha = 1;      // read in the field, ignore the values and set all cells to 1 (water).