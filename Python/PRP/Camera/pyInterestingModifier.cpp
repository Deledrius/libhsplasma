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

#include <PRP/Camera/plInterestingModifier.h>
#include "pyInterestingModifier.h"
#include "PRP/Modifier/pyModifier.h"


PY_PLASMA_NEW(InterestingModifier, plInterestingModifier)

PY_PROPERTY(float, InterestingModifier, interestWeight, getInterestWeight, setInterestWeight)
PY_PROPERTY(float, InterestingModifier, interestRadius, getInterestRadius, setInterestRadius)
PY_PROPERTY(int, InterestingModifier, type, getType, setType)

static PyGetSetDef pyInterestingModifier_GetSet[] = {
    pyInterestingModifier_interestWeight_getset,
    pyInterestingModifier_interestRadius_getset,
    pyInterestingModifier_type_getset,
    PY_GETSET_TERMINATOR
};

PY_PLASMA_TYPE(InterestingModifier, plInterestingModifier, "pfInterestingModifier wrapper");

PY_PLASMA_TYPE_INIT(InterestingModifier) {
    pyInterestingModifier_Type.tp_new = pyInterestingModifier_new;
    pyInterestingModifier_Type.tp_getset = pyInterestingModifier_GetSet;
    pyInterestingModifier_Type.tp_base = &pySingleModifier_Type;
    if (PyType_CheckAndReady(&pyInterestingModifier_Type) < 0)
        return NULL;

    /* Konstants */
    PY_TYPE_ADD_CONST(InterestingModifier, "kTypeInteresting", plInterestingModifier::kTypeInteresting);
    PY_TYPE_ADD_CONST(InterestingModifier, "kTypeLookAtMod", plInterestingModifier::kTypeLookAtMod);

    Py_INCREF(&pyInterestingModifier_Type);
    return (PyObject*)&pyInterestingModifier_Type;
}

PY_PLASMA_IFC_METHODS(InterestingModifier, plInterestingModifier)

