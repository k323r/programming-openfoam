# notes from "programming openfoam" - day 2

- dictionary lookups can be dimensioned as well by supplying a dimensions keyword. 
- dimensioned lookups are also possible, look e.g. at `createFields.H` from the `icoFoam` directory.
- assigning values to a field: boundary faces with a fixedValue will not be overwritten. However, there is an operator `==` available that overrides this: `p_rgh == p`.
