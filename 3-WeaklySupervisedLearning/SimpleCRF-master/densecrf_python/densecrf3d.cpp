#include <Python.h>
#include "numpy/arrayobject.h"
#include "densecrf.h"
#include "densecrf3d_util.h"
#include <iostream>
//#define DEBUG 
using namespace std;

// example to use numpy object: http://blog.debao.me/2013/04/my-first-c-extension-to-numpy/
// write a c extension ot Numpy: http://folk.uio.no/hpl/scripting/doc/python/NumPy/Numeric/numpy-13.html


static PyObject *
dense_crf_wrapper(PyObject *self, PyObject *args)
{
    PyObject *I=NULL, *P=NULL, *param=NULL, *BilateralModsStds=NULL;
    PyArrayObject *arr_I=NULL, *arr_P=NULL;
    
    if (!PyArg_ParseTuple(args, "OOO", &I, &P, &param)) return NULL;
    
    arr_I = (PyArrayObject*)PyArray_FROM_OTF(I, NPY_UINT8, NPY_IN_ARRAY);
    if (arr_I == NULL) return NULL;
    
    arr_P = (PyArrayObject*)PyArray_FROM_OTF(P, NPY_FLOAT32, NPY_IN_ARRAY);
    if (arr_P == NULL) return NULL;

    
    /*vv* code that makes use of arguments *vv*/
    
    int nd_I = PyArray_NDIM(arr_I);   //number of dimensions
    int nd_P = PyArray_NDIM(arr_P);
    if(nd_I != nd_P)
    {
        cerr<<"data dimension "<<nd_I<<" probability dimension "<<nd_P<<" do not match"<<endl;
        return NULL;

    }
    npy_intp * shape_I = PyArray_DIMS(arr_I);  // npy_intp array of length nd showing length in each dim.
    npy_intp * shape_P = PyArray_DIMS(arr_P);
#ifdef DEBUG
    cout<<"input data shape: ";
    for(int i=0; i<nd_I; i++)
    {
        cout<<shape_I[i]<<" ";
    }
    cout<<std::endl;
    cout<<"input probability shape: ";
    for(int i=0; i<nd_P; i++)
    {
        cout<<shape_P[i]<<" ";
    }
    cout<<std::endl;
#endif
    for(int i=0; i<nd_I-1; i++)
    {
        if(shape_I[i] !=shape_P[i] || shape_I[i]!=shape_P[i])
        {
            cerr<<"input shape does not match"<<endl;
            return NULL;
        }
    }

    double MaxIterations = PyFloat_AsDouble(PyDict_GetItemString(param, "MaxIterations"));
    double PosRStd  = PyFloat_AsDouble(PyDict_GetItemString(param, "PosRStd"));
    double PosCStd  = PyFloat_AsDouble(PyDict_GetItemString(param, "PosCStd"));
    double PosZStd  = PyFloat_AsDouble(PyDict_GetItemString(param, "PosZStd"));
    double PosW     = PyFloat_AsDouble(PyDict_GetItemString(param, "PosW"));
    double BilateralRStd = PyFloat_AsDouble(PyDict_GetItemString(param, "BilateralRStd"));
    double BilateralCStd = PyFloat_AsDouble(PyDict_GetItemString(param, "BilateralCStd"));
    double BilateralZStd = PyFloat_AsDouble(PyDict_GetItemString(param, "BilateralZStd"));
    double ModalityNum   = PyFloat_AsDouble(PyDict_GetItemString(param, "ModalityNum"));
    double BilateralW    = PyFloat_AsDouble(PyDict_GetItemString(param, "BilateralW"));
    BilateralModsStds = PyDict_GetItemString(param, "BilateralModsStds");
    cout<<"MaxIterations :"<<MaxIterations<<endl;
    cout<<"PosRStd       :"<<PosRStd      <<endl;
    cout<<"PosCStd       :"<<PosCStd      <<endl;
    cout<<"PosZStd       :"<<PosZStd      <<endl;
    cout<<"PosW          :"<<PosW         <<endl;
    cout<<"BilateralRStd :"<<BilateralRStd<<endl;
    cout<<"BilateralCStd :"<<BilateralCStd<<endl;
    cout<<"BilateralZStd :"<<BilateralZStd<<endl;
    cout<<"ModalityNum   :"<<ModalityNum  <<endl;
    cout<<"BilateralW    :"<<BilateralW   <<endl;
    
    if(ModalityNum != shape_I[3])
    {
        cerr<<"input data channel "<<shape_I[3]<<" and modality number "<<ModalityNum<<" do not match"<<endl;
        return NULL;
    }
    int modStds_size = PyTuple_GET_SIZE(BilateralModsStds);
    if(ModalityNum != modStds_size)
    {
        cerr<<"input data channel "<<shape_I[3]<<" and BilateralModsStds size "<<modStds_size<<" do not match"<<endl;
        return NULL;
    }
    cout<<"BilateralModsStds:(";
    std::vector<double> mod_std_vector;
    mod_std_vector.resize(modStds_size);
    for(int i = 0; i< modStds_size; i++)
    {
        mod_std_vector[i] = PyFloat_AsDouble(PyTuple_GET_ITEM(BilateralModsStds, i));
        cout<< mod_std_vector[i];
        if(i< modStds_size - 1) cout<<", ";
    }
    cout<<")"<<std::endl;
    
    //get unary potential array size num_class x num_voxel
    int num_voxel = shape_P[0] * shape_P[1] * shape_P[2];
    int num_class = shape_P[3];

    MatrixXf matrixUnaries( num_class, num_voxel );
    for(int z = 0; z< shape_P[0]; z++)
    {
        for(int y = 0; y< shape_P[1]; y++)
        {
            for(int x = 0; x< shape_P[2]; x++)
            {
                int voxel_id = z*shape_P[1]*shape_P[2] + y*shape_P[2] + x;
                for(int c = 0; c< shape_P[3]; c++)
                {
                    
                    float prob = *(float *)(arr_P->data + z*arr_P->strides[0] + y*arr_P->strides[1] +
                                            x*arr_P->strides[2] + c*arr_P->strides[3]);
                    float energy = -log(prob);
                    matrixUnaries(c, voxel_id) = energy;
                }
            }
        }
    }

    DenseCRF3D crf3d(shape_I[0], shape_I[1], shape_I[2], num_class);
    crf3d.setUnaryEnergy( matrixUnaries );
    crf3d.addPairwiseGaussian( PosRStd , PosCStd, PosZStd, new PottsCompatibility(PosW));
    addPairwiseBilateral(crf3d,
                        ModalityNum,
                        BilateralRStd,
                        BilateralCStd,
                        BilateralZStd,
                        mod_std_vector,
                        BilateralW,
                        (unsigned char *)arr_I->data);
    MatrixXf probMapsMatrix = crf3d.inference(MaxIterations);
    VectorXs segmentationVector = crf3d.currentMap(probMapsMatrix);
    npy_intp outshape[3];
    outshape[0] = shape_P[0];
    outshape[1] = shape_P[1];
    outshape[2] = shape_P[2];
    PyArrayObject * labels = (PyArrayObject*)  PyArray_SimpleNew(3, outshape, NPY_INT8);
    for(int i = 0; i < num_voxel; i++)
    {
        *(labels->data + i) = segmentationVector(i);
    }

    Py_DECREF(arr_I);
    Py_DECREF(arr_P);
    Py_INCREF(labels);
    return PyArray_Return(labels);
}

static PyMethodDef Methods[] = {
    {"densecrf3d",  dense_crf_wrapper, METH_VARARGS, "3D dense CRF"},
    {NULL, NULL, 0, NULL}
};
