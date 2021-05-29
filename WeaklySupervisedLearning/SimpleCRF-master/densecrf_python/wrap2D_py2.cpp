#include "densecrf.cpp"

PyMODINIT_FUNC
initdenseCRF(void) {
    (void) Py_InitModule("denseCRF", Methods);
    import_array();
}
