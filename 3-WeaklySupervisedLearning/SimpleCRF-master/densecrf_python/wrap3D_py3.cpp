#include "densecrf3d.cpp"

static struct PyModuleDef sDenseCRF = 
{
    PyModuleDef_HEAD_INIT,
    "denseCRF3D", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    Methods
};


PyMODINIT_FUNC PyInit_denseCRF3D(void) {
    import_array();
    return PyModule_Create(&sDenseCRF);
}