#include "maxflow.cpp"

static struct PyModuleDef Maxflow = 
{
    PyModuleDef_HEAD_INIT,
    "maxflow", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    Methods
};

PyMODINIT_FUNC PyInit_maxflow(void) {
    import_array();
    return PyModule_Create(&Maxflow);
}
