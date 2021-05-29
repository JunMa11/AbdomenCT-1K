#include <Python.h>
#include "numpy/arrayobject.h"
#include "maxflow-v3.0/graph.h"
#include "util.h"
#include <iostream>
using namespace std;

// example to use numpy object: http://blog.debao.me/2013/04/my-first-c-extension-to-numpy/
// write a c extension ot Numpy: http://folk.uio.no/hpl/scripting/doc/python/NumPy/Numeric/numpy-13.html
static PyObject *
maxflow2d_wrapper(PyObject *self, PyObject *args)
{
    PyObject *I=NULL, *P=NULL, *param=NULL;
    PyArrayObject *arr_I=NULL, *arr_P=NULL;
    if (!PyArg_ParseTuple(args, "OOO", &I, &P, &param)) return NULL;
    
    arr_I = (PyArrayObject*)PyArray_FROM_OTF(I, NPY_FLOAT32, NPY_IN_ARRAY);
    arr_P = (PyArrayObject*)PyArray_FROM_OTF(P, NPY_FLOAT32, NPY_IN_ARRAY);
    if (arr_I == NULL || arr_P == NULL) return NULL;
    float lambda = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 0));
    float sigma  = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 1));

    /*vv* code that makes use of arguments *vv*/
    int dimI = PyArray_NDIM(arr_I);             // number of dimensions
    int dimP = PyArray_NDIM(arr_P); 
    npy_intp * shapeI = PyArray_DIMS(arr_I);    // npy_intp array of length nd showing length in each dim
    npy_intp * shapeP = PyArray_DIMS(arr_P);

    if(dimI > 3){
        cout << "the input dimension can only be 2 or 3"<<endl;
        return NULL;
    }
    if(dimP != 3){
        cout << "dimension of probabilily map should be 3"<<endl;
        return NULL;
    }
    if(shapeI[0] != shapeP[0] || shapeI[1] != shapeP[1]){
        cout << "image and probability map have different sizes"<<endl;
        return NULL;
    }
    if(shapeP[2] != 2){
        cout << "probabilily map should have two channels"<<endl;
        return NULL;
    }

    int chns = 1;
    if(dimI == 3) chns = shapeI[2];
    npy_intp outshape[2];
    outshape[0]=shapeI[0];
    outshape[1]=shapeI[1];
    PyArrayObject * arr_L = (PyArrayObject*)  PyArray_SimpleNew(2, outshape, NPY_INT8);
    maxflow_inference((unsigned char *) arr_L->data, (const float *) arr_I->data, (const float *) arr_P->data, NULL,
        shapeI[0], shapeI[1], chns, 2, lambda, sigma);
  
    Py_DECREF(arr_I);
    Py_DECREF(arr_P);
    Py_INCREF(arr_L);
    return PyArray_Return(arr_L);
}

static PyObject *
interactive_maxflow2d_wrapper(PyObject *self, PyObject *args)
{
    PyObject *I=NULL, *P=NULL, *S=NULL, *param=NULL;
    PyArrayObject *arr_I=NULL, *arr_P=NULL, *arr_S=NULL;
    if (!PyArg_ParseTuple(args, "OOOO", &I, &P, &S, &param)) return NULL;
    
    arr_I = (PyArrayObject*)PyArray_FROM_OTF(I, NPY_FLOAT32, NPY_IN_ARRAY);
    arr_P = (PyArrayObject*)PyArray_FROM_OTF(P, NPY_FLOAT32, NPY_IN_ARRAY);
    arr_S = (PyArrayObject*)PyArray_FROM_OTF(S, NPY_UINT8,   NPY_IN_ARRAY);
    if (arr_I == NULL || arr_P == NULL || arr_S == NULL) return NULL;
    float lambda = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 0));
    float sigma  = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 1));

    /*vv* code that makes use of arguments *vv*/
    int dimI = PyArray_NDIM(arr_I);              // number of dimensions
    int dimP = PyArray_NDIM(arr_P); 
    int dimS = PyArray_NDIM(arr_S); 
    npy_intp * shapeI = PyArray_DIMS(arr_I);     // npy_intp array of length nd showing length in each dim.
    npy_intp * shapeP = PyArray_DIMS(arr_P);
    npy_intp * shapeS = PyArray_DIMS(arr_S);
    
    if(dimI > 3){
        cout << "the input dimension can only be 2 or 3"<<endl;
        return NULL;
    }
    if(dimP != 3 || dimS != 3){
        cout << "dimension of probabilily map and seed map should be 3"<<endl;
        return NULL;
    }
    if(shapeI[0] != shapeP[0] || shapeI[1] != shapeP[1] || 
       shapeI[0] != shapeS[0] || shapeI[1] != shapeS[1]){
        cout << "image, probability map, and seed map should have the same spatial size"<<endl;
        return NULL;
    }
    if(shapeP[2] != 2 || shapeS[2] !=2){
        cout << "probabilily map and seed map should have two channels"<<endl;
        return NULL;
    }
   
    int chns = 1;
    if(dimI == 3) chns = shapeI[2];
    npy_intp outshape[2];
    outshape[0]=shapeI[0];
    outshape[1]=shapeI[1];
    PyArrayObject * arr_L = (PyArrayObject*)  PyArray_SimpleNew(2, outshape, NPY_INT8);
    maxflow_inference((unsigned char *) arr_L->data, (const float *) arr_I->data, 
        (const float *) arr_P->data, (const unsigned char *) arr_S->data,
        shapeI[0], shapeI[1], chns, 2, lambda, sigma);
    
    Py_DECREF(arr_I);
    Py_DECREF(arr_P);
    Py_DECREF(arr_S);
    Py_INCREF(arr_L);
    return PyArray_Return(arr_L);
}

static PyObject *
maxflow3d_wrapper(PyObject *self, PyObject *args)
{
    PyObject *I=NULL, *P=NULL, *param=NULL;
    PyArrayObject *arr_I=NULL, *arr_P=NULL;
    if (!PyArg_ParseTuple(args, "OOO", &I, &P, &param)) return NULL;
    
    arr_I = (PyArrayObject*)PyArray_FROM_OTF(I, NPY_FLOAT32, NPY_IN_ARRAY);
    arr_P = (PyArrayObject*)PyArray_FROM_OTF(P, NPY_FLOAT32, NPY_IN_ARRAY);
    if (arr_I == NULL || arr_P == NULL) return NULL;
    float lambda = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 0));
    float sigma  = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 1));
    
    /*vv* code that makes use of arguments *vv*/
    int dimI = PyArray_NDIM(arr_I);             // number of dimensions
    int dimP = PyArray_NDIM(arr_P); 
    npy_intp * shapeI = PyArray_DIMS(arr_I);    // npy_intp array of length nd showing length in each dim
    npy_intp * shapeP = PyArray_DIMS(arr_P);

    if(dimI !=3 && dimI != 4){
        cout << "the input dimension can only be 3 or 4"<<endl;
        return NULL;
    }
    if(dimP != 4){
        cout << "dimension of probabilily map should be 4"<<endl;
        return NULL;
    }
    if(shapeI[0] != shapeP[0] || shapeI[1] != shapeP[1] || shapeI[2] != shapeP[2]){
        cout << "image and probability map have different sizes"<<endl;
        return NULL;
    }
    if(shapeP[3] != 2){
        cout << "probabilily map should have two channels"<<endl;
        return NULL;
    }
    
    int chns = 1;
    if(dimI == 4) chns = shapeI[3];
    npy_intp outshape[3] = {shapeI[0], shapeI[1], shapeI[2]};
    PyArrayObject * arr_L = (PyArrayObject*)  PyArray_SimpleNew(3, outshape, NPY_INT8);
    maxflow3d_inference((unsigned char *) arr_L->data, (const float *) arr_I->data, 
        (const float *) arr_P->data, NULL,
        shapeI[0], shapeI[1], shapeI[2], chns, 2, lambda, sigma);

    Py_DECREF(arr_I);
    Py_DECREF(arr_P);
    Py_INCREF(arr_L);
    return PyArray_Return(arr_L);
}

static PyObject *
interactive_maxflow3d_wrapper(PyObject *self, PyObject *args)
{
    PyObject *I=NULL, *P=NULL, *S=NULL, *param=NULL;
    PyArrayObject *arr_I=NULL, *arr_P=NULL, *arr_S=NULL;
    if (!PyArg_ParseTuple(args, "OOOO", &I, &P, &S, &param)) return NULL;
    
    arr_I = (PyArrayObject*)PyArray_FROM_OTF(I, NPY_FLOAT32, NPY_IN_ARRAY);
    arr_P = (PyArrayObject*)PyArray_FROM_OTF(P, NPY_FLOAT32, NPY_IN_ARRAY);
    arr_S = (PyArrayObject*)PyArray_FROM_OTF(S, NPY_UINT8,   NPY_IN_ARRAY);
    if (arr_I == NULL || arr_P == NULL || arr_S == NULL) return NULL;
    float lambda = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 0));
    float sigma  = PyFloat_AsDouble(PyTuple_GET_ITEM(param, 1));

    /*vv* code that makes use of arguments *vv*/
    int dimI = PyArray_NDIM(arr_I);              // number of dimensions
    int dimP = PyArray_NDIM(arr_P); 
    int dimS = PyArray_NDIM(arr_S); 
    npy_intp * shapeI = PyArray_DIMS(arr_I);     // npy_intp array of length nd showing length in each dim.
    npy_intp * shapeP = PyArray_DIMS(arr_P);
    npy_intp * shapeS = PyArray_DIMS(arr_S);
    
    if(dimI !=3 && dimI != 4){
        cout << "the input dimension can only be 3 or 4"<<endl;
        return NULL;
    }
    if(dimP != 4 || dimS != 4){
        cout << "dimension of probabilily map and seed map should be 4"<<endl;
        return NULL;
    }
    if(shapeI[0] != shapeP[0] || shapeI[1] != shapeP[1] || shapeI[2] != shapeP[2] ||
       shapeI[0] != shapeS[0] || shapeI[1] != shapeS[1] || shapeI[2] != shapeS[2]){
        cout << "image, probability map, and seed map should have the same spatial size"<<endl;
        return NULL;
    }
    if(shapeP[3] != 2 || shapeS[3] !=2){
        cout << "probabilily map and seed map should have two channels"<<endl;
        return NULL;
    }
    
    int chns = 1;
    if(dimI == 4) chns = shapeI[3];
    npy_intp outshape[3] = {shapeI[0], shapeI[1], shapeI[2]};
    PyArrayObject * arr_L = (PyArrayObject*)  PyArray_SimpleNew(3, outshape, NPY_INT8);
    maxflow3d_inference((unsigned char *) arr_L->data, (const float *) arr_I->data, 
        (const float *) arr_P->data, (const unsigned char *) arr_S->data,
        shapeI[0], shapeI[1], shapeI[2], chns, 2, lambda, sigma);

    Py_DECREF(arr_I);
    Py_DECREF(arr_P);
    Py_DECREF(arr_S);
    Py_INCREF(arr_L);
    return PyArray_Return(arr_L);
}

static PyMethodDef Methods[] = {
    {"maxflow2d",  maxflow2d_wrapper, METH_VARARGS, "computing 2D max flow"},
    {"interactive_maxflow2d",  interactive_maxflow2d_wrapper, METH_VARARGS, "computing 2D max flow with interactions"},
    {"maxflow3d",  maxflow3d_wrapper, METH_VARARGS, "computing 3D max flow"},
    {"interactive_maxflow3d",  interactive_maxflow3d_wrapper, METH_VARARGS, "computing 3D max flow with interactions"},
    {NULL, NULL, 0, NULL}
};
