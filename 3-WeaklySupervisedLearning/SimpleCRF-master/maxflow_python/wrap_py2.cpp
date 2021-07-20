#include "maxflow.cpp"


PyMODINIT_FUNC
initmaxflow(void) {
    (void) Py_InitModule("maxflow", Methods);
    import_array();
}
