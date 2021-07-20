#include "densecrf3d.cpp"

PyMODINIT_FUNC
initdenseCRF3D(void) {
   (void) Py_InitModule("denseCRF3D", Methods);
   import_array();
}
