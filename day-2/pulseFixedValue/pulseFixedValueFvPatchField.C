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

#include "pulseFixedValueFvPatchField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template <class Type>
Foam::scalar Foam::pulseFixedValueFvPatchField<Type>::pulseFraction() const
{
    // current time of the simulation
    const scalar t = this->db().time().timeOutputValue();

    // let's get the current cycle to see if we are still in the pulse or already out
    scalar cycleFraction = fmod(t / period_, 1.0);

    // already outside of the cycle?
    if (cycleFraction > duration_)
    {
        return 0.0;
    }
    else
    {
        return 1.0;
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class Type>
Foam::pulseFixedValueFvPatchField<Type>::
    pulseFixedValueFvPatchField(
        const fvPatch &p,
        const DimensionedField<Type, volMesh> &iF)
    : fixedValueFvPatchField<Type>(p, iF),
      period_(0.0),
      duration_(0.0),
      baseValue_(Zero),
      pulseValue_(p.size(), Zero)
{
}

template <class Type>
Foam::pulseFixedValueFvPatchField<Type>::
    pulseFixedValueFvPatchField(
        const fvPatch &p,
        const DimensionedField<Type, volMesh> &iF,
        const dictionary &dict)
    : fixedValueFvPatchField<Type>(p, iF),
      period_(dict.lookup<scalar>("period")),
      duration_(dict.lookup<scalar>("period")),
      baseValue_(dict.lookupOrDefault<Type>("baseValue", Zero)),
      pulseValue_("pulseValue", dict, p.size())
{

    if (duration_ <= 0 || duration_ >= 1)
    {
        FatalErrorInFunction
            << "duration = " << duration_ << " is invalid" << nl
            << "please provide a valid value for duration: 0 < duration < 1"
            << exit(FatalError);
    }

    fixedValueFvPatchField<Type>::evaluate();

    /*
    // Initialise with the value entry if evaluation is not possible
    fvPatchField<Type>::operator=
    (
        Field<Type>("value", dict, p.size())
    );
    */
}

template <class Type>
Foam::pulseFixedValueFvPatchField<Type>::
    pulseFixedValueFvPatchField(
        const pulseFixedValueFvPatchField<Type> &ptf,
        const fvPatch &p,
        const DimensionedField<Type, volMesh> &iF,
        const fvPatchFieldMapper &mapper)
    : fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
      period_(ptf.period_),
      duration_(ptf.period_),
      baseValue_(ptf.baseValue_),
      pulseValue_(mapper(ptf.pulseValue_))
{
}

template <class Type>
Foam::pulseFixedValueFvPatchField<Type>::
    pulseFixedValueFvPatchField(
        const pulseFixedValueFvPatchField<Type> &ptf,
        const DimensionedField<Type, volMesh> &iF)
    : fixedValueFvPatchField<Type>(ptf, iF),
      period_(ptf.period_),
      duration_(ptf.period_),
      baseValue_(ptf.baseValue_),
      pulseValue_(ptf.pulseValue_)
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class Type>
void Foam::pulseFixedValueFvPatchField<Type>::autoMap(
    const fvPatchFieldMapper &m)
{
    fixedValueFvPatchField<Type>::autoMap(m);
    m(pulseValue_, pulseValue_);
}

template <class Type>
void Foam::pulseFixedValueFvPatchField<Type>::rmap(
    const fvPatchField<Type> &ptf,
    const labelList &addr)
{
    fixedValueFvPatchField<Type>::rmap(ptf, addr);

    const pulseFixedValueFvPatchField<Type> &tiptf =
        refCast<const pulseFixedValueFvPatchField<Type>>(ptf);

    pulseValue_.rmap(tiptf.pulseValue_, addr);
}

template <class Type>
void Foam::pulseFixedValueFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    // implement the assign operator
    fixedValueFvPatchField<Type>::operator==(

        // calculate the current pulse value by calling the function pulseFraction()
        baseValue_ *(1.0 - pulseFraction()) // if inside the puls, the value in the parentheses will become 0
        + pulseValue_ * pulseFraction()     // and the pulseValue will be multiplied with 1

    );

    fixedValueFvPatchField<Type>::updateCoeffs();
}

template <class Type>
void Foam::pulseFixedValueFvPatchField<Type>::write(
    Ostream &os) const
{
    fvPatchField<Type>::write(os);
    writeEntry(os, "period", period_);
    writeEntry(os, "duration", duration_);
    writeEntry(os, "baseValue", baseValue_);
    writeEntry(os, "pulseValue", pulseValue_);
}

// ************************************************************************* //
