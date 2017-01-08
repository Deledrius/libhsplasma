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

#include "pyNotifyMsg.h"

#include <PRP/Message/plNotifyMsg.h>
#include "pyMessage.h"
#include "pyEventData.h"
#include "PRP/pyCreatable.h"

extern "C" {

PY_PLASMA_NEW(NotifyMsg, plNotifyMsg)

PY_METHOD_NOARGS(NotifyMsg, clearEvents, "Remove all event objects") {
    self->fThis->clearEvents();
    Py_RETURN_NONE;
}

PY_METHOD_VA(NotifyMsg, addEvent,
    "Params: event\n"
    "Add an event")
{
    pyEventData* evt;
    if (!PyArg_ParseTuple(args, "O", &evt)) {
        PyErr_SetString(PyExc_TypeError, "addEvent expects a proEventData");
        return NULL;
    }
    if (!pyEventData_Check((PyObject*)evt)) {
        PyErr_SetString(PyExc_TypeError, "addEvent expects a proEventData");
        return NULL;
    }
    self->fThis->addEvent(evt->fThis);
    evt->fPyOwned = false;
    Py_RETURN_NONE;
}

PY_METHOD_VA(NotifyMsg, delEvent,
    "Params: idx\n"
    "Remove an event")
{
    int idx;
    if (!PyArg_ParseTuple(args, "i", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delEvent expects an int");
        return NULL;
    }
    self->fThis->delEvent(idx);
    Py_RETURN_NONE;
}

static PyObject* pyNotifyMsg_getEvents(pyNotifyMsg* self, void*) {
    PyObject* list = PyList_New(self->fThis->getEvents().size());
    for (size_t i=0; i<self->fThis->getEvents().size(); i++)
        PyList_SET_ITEM(list, i, ICreateEventData(self->fThis->getEvents()[i]));
    return list;
}

static int pyNotifyMsg_setEvents(pyNotifyMsg* self, PyObject* value, void*) {
    PyErr_SetString(PyExc_RuntimeError, "to add events, use addEvent");
    return -1;
}

static PyMethodDef pyNotifyMsg_Methods[] = {
    pyNotifyMsg_clearEvents_method,
    pyNotifyMsg_addEvent_method,
    pyNotifyMsg_delEvent_method,
    PY_METHOD_TERMINATOR
};

PY_PROPERTY(int, NotifyMsg, type, getType, setType)
PY_PROPERTY(float, NotifyMsg, state, getState, setState)
PY_PROPERTY(int, NotifyMsg, id, getID, setID)

static PyGetSetDef pyNotifyMsg_GetSet[] = {
    { _pycs("events"), (getter)pyNotifyMsg_getEvents,
        (setter)pyNotifyMsg_setEvents, NULL, NULL },
    pyNotifyMsg_type_getset,
    pyNotifyMsg_state_getset,
    pyNotifyMsg_id_getset,
    PY_GETSET_TERMINATOR
};

PY_PLASMA_TYPE(NotifyMsg, plNotifyMsg, "plNotifyMsg wrapper")

PY_PLASMA_TYPE_INIT(NotifyMsg) {
    pyNotifyMsg_Type.tp_new = pyNotifyMsg_new;
    pyNotifyMsg_Type.tp_methods = pyNotifyMsg_Methods;
    pyNotifyMsg_Type.tp_getset = pyNotifyMsg_GetSet;
    pyNotifyMsg_Type.tp_base = &pyMessage_Type;
    if (PyType_CheckAndReady(&pyNotifyMsg_Type) < 0)
        return NULL;

    PY_TYPE_ADD_CONST(NotifyMsg, "kActivator", plNotifyMsg::kActivator);
    PY_TYPE_ADD_CONST(NotifyMsg, "kVarNotification", plNotifyMsg::kVarNotification);
    PY_TYPE_ADD_CONST(NotifyMsg, "kNotifySelf", plNotifyMsg::kNotifySelf);
    PY_TYPE_ADD_CONST(NotifyMsg, "kResponderFF", plNotifyMsg::kResponderFF);
    PY_TYPE_ADD_CONST(NotifyMsg, "kResponderChangeState", plNotifyMsg::kResponderChangeState);

    Py_INCREF(&pyNotifyMsg_Type);
    return (PyObject*)&pyNotifyMsg_Type;
}

PY_PLASMA_IFC_METHODS(NotifyMsg, plNotifyMsg)

}
