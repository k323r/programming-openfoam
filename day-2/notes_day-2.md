# notes from "programming openfoam" - day 2

- dictionary lookups can be dimensioned as well by supplying a dimensions keyword. 
- dimensioned lookups are also possible, look e.g. at `createFields.H` from the `icoFoam` directory.
- assigning values to a field: boundary faces with a fixedValue will not be overwritten. However, there is an operator `==` available that overrides this: `p_rgh == p`.
- best practice: if you are not sure how to implement stuff, find a suitable example already implemented in openfoam and copy the parts of the code that implement the feature you want. Compile as early and as often as possible.
- elegantly switch writing on and off with the switches `NO_WRITE` and `AUTO_WRITE` when reading in a volScalarField.
- also a cool tool `setFields` to initialize fields in the 0 folder.
- `foamGet decomposeParDict` searches and copies a given dict. Tripple-TAB to see all available dicts (and options)
- `Foam::constant::thermodynamic` and `Foam::constant::mathematical` includes a couple of useful global variables for physical computations
