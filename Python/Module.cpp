#include <Python.h>
#include <Stream/hsStream.h>
#include <Util/PlasmaVersions.h>

#include "Math/pyGeometry3.h"
#include "Math/pyMatrix44.h"
#include "ResManager/pyResManager.h"
#include "Stream/pyStream.h"
#include "Sys/pyColor.h"
#include "Util/pyUtil.h"

#include "PRP/pyCreatable.h"
#include "PRP/pySceneNode.h"
#include "PRP/KeyedObject/pyKey.h"
#include "PRP/KeyedObject/pyKeyedObject.h"
#include "PRP/Object/pySceneObject.h"
#include "PRP/Object/pySynchedObject.h"

static PyMethodDef PyPlasma_Methods[] = {
    { NULL, NULL, 0, NULL }
};

void initPyPlasma_Constants(PyObject* module) {
    /* Generic constants */
    PyModule_AddIntConstant(module, "pvUnknown", pvUnknown);
    PyModule_AddIntConstant(module, "pvPrime", pvPrime);
    PyModule_AddIntConstant(module, "pvPots", pvPots);
    PyModule_AddIntConstant(module, "pvLive", pvLive);
    PyModule_AddIntConstant(module, "pvEoa", pvEoa);
    PyModule_AddIntConstant(module, "pvHex", pvHex);

    /* File modes */
    PyModule_AddIntConstant(module, "fmRead", fmRead);
    PyModule_AddIntConstant(module, "fmWrite", fmWrite);
    PyModule_AddIntConstant(module, "fmReadWrite", fmReadWrite);
    PyModule_AddIntConstant(module, "fmCreate", fmCreate);
}

PyMODINIT_FUNC initPyPlasma() {
    PyObject* module = Py_InitModule3("PyPlasma", PyPlasma_Methods,
                                      "libPlasma Python Interface Module");
    initPyPlasma_Constants(module);

    /* Stream */
    PyModule_AddObject(module, "hsStream", Init_pyStream_Type());
    PyModule_AddObject(module, "hsFileStream", Init_pyFileStream_Type());
    PyModule_AddObject(module, "plEncryptedStream", Init_pyEncryptedStream_Type());

    /* Util */
    PyModule_AddObject(module, "hsBitVector", Init_pyBitVector_Type());

    /* pyColor */
    PyModule_AddObject(module, "hsColorRGBA", Init_pyColorRGBA_Type());
    PyModule_AddObject(module, "hsColor32", Init_pyColor32_Type());

    /* Math */
    PyModule_AddObject(module, "hsVector3", Init_pyVector3_Type());
    PyModule_AddObject(module, "hsMatrix44", Init_pyMatrix44_Type());

    /* ResManager */
    //PyModule_AddObject(module, "plFactory", Init_pyFactory_Type());
    PyModule_AddObject(module, "plResManager", Init_pyResManager_Type());
    PyModule_AddObject(module, "plPageInfo", Init_pyPageInfo_Type());
    PyModule_AddObject(module, "plAgeInfo", Init_pyAgeInfo_Type());

    /* pyKey */
    PyModule_AddObject(module, "plKey", Init_pyKey_Type());
    PyModule_AddObject(module, "plLocation", Init_pyLocation_Type());

    /* Creatables */
    PyModule_AddObject(module, "plCreatable", Init_pyCreatable_Type());
    PyModule_AddObject(module, "plCreatableStub", Init_pyCreatableStub_Type());

    PyModule_AddObject(module, "hsKeyedObject", Init_pyKeyedObject_Type());
    PyModule_AddObject(module, "plSceneNode", Init_pySceneNode_Type());
    PyModule_AddObject(module, "plSynchedObject", Init_pySynchedObject_Type());
    PyModule_AddObject(module, "plSceneObject", Init_pySceneObject_Type());
}
