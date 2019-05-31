/*
© Leo Tamminen. Do not use without explicit3uw permission.
shophorn@protonmail.com

Codetest program for job application
C extension for python, module definition
*/

#include <Python.h>

/*
Defined in vector_functions.c
*/
PyObject * py_dot(PyObject * self, PyObject * args);
PyObject * py_cross2d(PyObject * self, PyObject * args);
PyObject * py_pointInsideTriangles(PyObject * self, PyObject * args);
PyObject * py_windTriangles(PyObject * self, PyObject * args);

/*
Define method signatures for python
*/
static PyMethodDef methods[] = {
    { "dot", py_dot, METH_VARARGS, "Dot product of 2d float vectors" },
    { "cross2d", py_cross2d, METH_VARARGS, "Reduced (2d) cross product of float vectors" },
    { "pointInsideTriangles", py_pointInsideTriangles, METH_VARARGS, "Check if point is inside any triangle in list" },
    { "windTriangles", py_windTriangles, METH_VARARGS, "Wind triangles clockwise" },
    { NULL, NULL, 0, NULL }
};

/*
Define module. This is singleton module.
*/
static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "codetest",
    "Code Test Module for Job application",
    -1,
    methods
};

PyMODINIT_FUNC PyInit_codetest(void)
{
    return PyModule_Create(&module);
}