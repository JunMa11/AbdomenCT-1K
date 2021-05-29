#include <Python.h>
#include "numpy/arrayobject.h"
#include "densecrf.h"
#include "densecrf_core.h"
#include <iostream>
using namespace std;

// example to use numpy object: http://blog.debao.me/2013/04/my-first-c-extension-to-numpy/
// write a c extension ot Numpy: http://folk.uio.no/hpl/scripting/doc/python/NumPy/Numeric/numpy-13.html
static PyObject *
densecrf_wrapper(PyObject *self, PyObject *args)
{
    PyObject *I=NULL, *fP=NULL, *param=NULL;
    PyArrayObject *arr_I=NULL, *arr_fP=NULL;
    
    if (!PyArg_ParseTuple(args, "OOO", &I, &fP, &param)) return NULL;
    
    arr_I = (PyArrayObject*)PyArray_FROM_OTF(I,  NPY_UINT8, NPY_IN_ARRAY);
    if (arr_I == NULL) return NULL;
    
    arr_fP = (PyArrayObject*)PyArray_FROM_OTF(fP, NPY_FLOAT32, NPY_IN_ARRAY);
    if (arr_fP == NULL) return NULL;

    
    /*vv* code that makes use of arguments *vv*/
    
    int nd_I = PyArray_NDIM(arr_I);   //number of dimensions
    npy_intp * shape = PyArray_DIMS(arr_I);  // npy_intp array of length nd showing length in each dim.

    
    int nd_P = PyArray_NDIM(arr_fP);
    npy_intp * shape_fP = PyArray_DIMS(arr_fP);

    CRFParam crf_param;
    crf_param.w1    = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 0));
    crf_param.alpha = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 1));
    crf_param.beta  = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 2));
    crf_param.w2    = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 3));
    crf_param.gamma = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 4));
    crf_param.iter  = int(PyFloat_AsDouble(PyTuple_GET_ITEM(param, 5)));

    VectorXs map = dense_crf_inference((const unsigned char *)arr_I->data, (const float *)arr_fP->data, 
                    shape_fP[0], shape_fP[1], shape_fP[2], crf_param);

    npy_intp outshape[2];
    outshape[0]=shape_fP[0];
    outshape[1]=shape_fP[1];
    PyArrayObject * labels = (PyArrayObject*)  PyArray_SimpleNew(2, outshape, NPY_INT8);
    for (int x=0;x<outshape[0]*outshape[1];x++)
    {
        *(labels->data + x*labels->strides[1]) = map[x];
    }
    
    Py_DECREF(arr_I);
    Py_DECREF(arr_fP);
    Py_INCREF(labels);
    return PyArray_Return(labels);
}


static PyMethodDef Methods[] = {
    {"densecrf",    densecrf_wrapper, METH_VARARGS, "computing 2D dense CRF"},
    {NULL, NULL, 0, NULL}
};

