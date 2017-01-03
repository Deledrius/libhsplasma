/* This file is part of HSPlasma.
*
* HSPlasma is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* HSPlasma is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _PLINTERESTINGMODIFIER_H
#define _PLINTERESTINGMODIFIER_H

#include "PRP/Modifier/plModifier.h"

class PLASMA_DLL plInterestingModifier : public plSingleModifier {
    CREATABLE(plInterestingModifier, kInterestingModifier, plSingleModifier)

public:
    enum { kTypeInteresting, kTypeLookAtMod };

protected:
    float fInterestWeight, fInterestRadius;
    int fType;

public:
    plInterestingModifier() { }
    virtual ~plInterestingModifier() { };

    const float getInterestWeight() const { return fInterestWeight; }
    const float getInterestRadius() const { return fInterestRadius; }
    const int getType() const { return fType; }

    void setInterestRadius(const float interestWeight) { fInterestRadius = interestWeight; }
    void setInterestWeight(const float interestRadius) { fInterestWeight = interestRadius; }
    void setType(const int type) { fType = type; }
};

#endif
