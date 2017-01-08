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

#include "pyOccluder.h"

#include <PRP/Geometry/plOccluder.h>
#include "PRP/Object/pyObjInterface.h"
#include "PRP/Region/pyBounds.h"
#include "PRP/KeyedObject/pyKey.h"
#include "PRP/pyCreatable.h"

extern "C" {

PY_PLASMA_NEW(Occluder, plOccluder)

PY_METHOD_NOARGS(Occluder, clearPolys, "Remove all plCullPolys from the occluder") {
    self->fThis->clearPolys();
    Py_RETURN_NONE;
}

PY_METHOD_VA(Occluder, addPoly,
    "Params: poly\n"
    "Add a plCullPoly to the occluder")
{
    pyCullPoly* poly;
    if (!PyArg_ParseTuple(args, "O", &poly)) {
        PyErr_SetString(PyExc_TypeError, "addPoly expects a plCullPoly");
        return NULL;
    }
    if (!pyCullPoly_Check((PyObject*)poly)) {
        PyErr_SetString(PyExc_TypeError, "addPoly expects a plCullPoly");
        return NULL;
    }
    self->fThis->addPoly(*poly->fThis);
    Py_RETURN_NONE;
}

PY_METHOD_VA(Occluder, delPoly,
    "Params: idx\n"
    "Remove a plCullPoly from the occluder")
{
    int idx;
    if (!PyArg_ParseTuple(args, "i", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delPoly expects an int");
        return NULL;
    }
    self->fThis->delPoly(idx);
    Py_RETURN_NONE;
}

PY_METHOD_NOARGS(Occluder, clearVisRegions,
    "Remove all Vis Regions from the occluder")
{
    self->fThis->clearVisRegions();
    Py_RETURN_NONE;
}

PY_METHOD_VA(Occluder, addVisRegion,
    "Params: region\n"
    "Add a Vis Region to the occluder")
{
    pyKey* region;
    if (!PyArg_ParseTuple(args, "O", &region)) {
        PyErr_SetString(PyExc_TypeError, "addVisRegion expects a plKey");
        return NULL;
    }
    if (!pyKey_Check((PyObject*)region)) {
        PyErr_SetString(PyExc_TypeError, "addVisRegion expects a plKey");
        return NULL;
    }
    self->fThis->addVisRegion(*region->fThis);
    Py_RETURN_NONE;
}

PY_METHOD_VA(Occluder, delVisRegion,
    "Params: idx\n"
    "Remove a Vis Region from the occluder")
{
    int idx;
    if (!PyArg_ParseTuple(args, "i", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delVisRegion expects an int");
        return NULL;
    }
    self->fThis->delVisRegion(idx);
    Py_RETURN_NONE;
}

static PyObject* pyOccluder_getPolys(pyOccluder* self, void*) {
    PyObject* list = PyList_New(self->fThis->getPolys().size());
    for (size_t i=0; i<self->fThis->getPolys().size(); i++)
        PyList_SET_ITEM(list, i, pyCullPoly_FromCullPoly(self->fThis->getPolys()[i]));
    return list;
}

static PyObject* pyOccluder_getVisRegions(pyOccluder* self, void*) {
    PyObject* list = PyList_New(self->fThis->getVisRegions().size());
    for (size_t i=0; i<self->fThis->getVisRegions().size(); i++)
        PyList_SET_ITEM(list, i, pyKey_FromKey(self->fThis->getVisRegions()[i]));
    return list;
}

static int pyOccluder_setPolys(pyOccluder* self, PyObject* value, void*) {
    PyErr_SetString(PyExc_RuntimeError, "to add polys, use addPoly()");
    return -1;
}

static int pyOccluder_setVisRegions(pyOccluder* self, PyObject* value, void*) {
    PyErr_SetString(PyExc_RuntimeError, "to add visRegions, use addVisRegion()");
    return -1;
}

static PyMethodDef pyOccluder_Methods[] = {
    pyOccluder_clearPolys_method,
    pyOccluder_addPoly_method,
    pyOccluder_delPoly_method,
    pyOccluder_clearVisRegions_method,
    pyOccluder_addVisRegion_method,
    pyOccluder_delVisRegion_method,
    PY_METHOD_TERMINATOR
};

PY_PROPERTY(float, Occluder, priority, getPriority, setPriority)
PY_PROPERTY_BOUNDS(Bounds3Ext, Occluder, worldBounds, getWorldBounds, setWorldBounds)
PY_PROPERTY(plKey, Occluder, sceneNode, getSceneNode, setSceneNode)

static PyGetSetDef pyOccluder_GetSet[] = {
    pyOccluder_priority_getset,
    pyOccluder_worldBounds_getset,
    pyOccluder_sceneNode_getset,
    { _pycs("polys"), (getter)pyOccluder_getPolys,
        (setter)pyOccluder_setPolys, NULL, NULL },
    { _pycs("visRegions"), (getter)pyOccluder_getVisRegions,
        (setter)pyOccluder_setVisRegions, NULL, NULL },
    PY_GETSET_TERMINATOR
};

PY_PLASMA_TYPE(Occluder, plOccluder, "plOccluder wrapper")

PY_PLASMA_TYPE_INIT(Occluder) {
    pyOccluder_Type.tp_new = pyOccluder_new;
    pyOccluder_Type.tp_methods = pyOccluder_Methods;
    pyOccluder_Type.tp_getset = pyOccluder_GetSet;
    pyOccluder_Type.tp_base = &pyObjInterface_Type;
    if (PyType_CheckAndReady(&pyOccluder_Type) < 0)
        return NULL;

    Py_INCREF(&pyOccluder_Type);
    return (PyObject*)&pyOccluder_Type;
}

PY_PLASMA_IFC_METHODS(Occluder, plOccluder)

}
