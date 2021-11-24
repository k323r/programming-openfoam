# notes from "programming openfoam" - day 2

- dictionary lookups can be dimensioned as well by supplying a dimensions keyword. 
- dimensioned lookups are also possible, look e.g. at `createFields.H` from the `icoFoam` directory.
- assigning values to a field: boundary faces with a fixedValue will not be overwritten. However, there is an operator `==` available that overrides this: `p_rgh == p`.
- best practice: if you are not sure how to implement stuff, find a suitable example already implemented in openfoam and copy the parts of the code that implement the feature you want. Compile as early and as often as possible.
- elegantly switch writing on and off with the switches `NO_WRITE` and `AUTO_WRITE` when reading in a volScalarField.
- also a cool tool `setFields` to initialize fields in the 0 folder.
- `foamGet decomposeParDict` searches and copies a given dict. Tripple-TAB to see all available dicts (and options)
- `Foam::constant::thermodynamic` and `Foam::constant::mathematical` includes a couple of useful global variables for physical computations
- How are the model loaded into the different applications? they are selected in the dictionary and then need to be dynamically selected at run time...
  - each model exposes itself into a hash table by calling `addToRunTimeSelectionTable`
  - a pointer pointing to a turbulence model is created. Upon the create of the pointer, a selector function is called (`New()`) that actually looks up the model and loads it from the has table.
  - there is a generic interface for models, called `fvModels` e.g. for momentum sources.
  - each turbulence model has it's own correct function that actually executes the turbulence model. How can we implement the same function over and over again? you create a base class with a **virtual function** `correct`. And each derived class (aka turbulence model) then implements the `correct` function.

## Boundary conditions

- generally stored under `$FOAM_SRC/finiteVolume/fields/fvPatchFields`
- whats `cyclicSlip`?
