#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <string.h>
#include <omp.h>

/* weight and intermediate data type */
#define ENCRYPT 1

/* binarize or not */
#define BINARIZE 0

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define OPENMP_THREADS 8
#define Euler 2.718281828
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define DIVUP(x, y) (((x) + (y) - 1) / (y))

class SEALWrapper;
class Encrypt;
class Tensor;
class Params;
class Layer;
class LayerInput;
class LayerAct;
class LayerConv;
class LayerFull;
class LayerPool;
class LayerBN;
class LayerRes;
class LayerCat;
class Net;

#if ENCRYPT == 0
typedef float DataType;
#elif ENCRYPT == 1
typedef Encrypt DataType;
#endif

/* assert */
inline void Assert(bool b, std::string msg)
{
    if (b)
    {
        return;
    }
    std::string _errmsg = std::string("Assertion Failed: ") + msg;
    std::cerr << _errmsg.c_str() << std::endl;
    exit(1);
}

#include "seal/seal.h"
#include "Encrypt.h"
#include "Tensor.h"
#include "Params.h"
#include "Layer.h"
#include "LayerInput.h"
#include "LayerAct.h"
#include "LayerConv.h"
#include "LayerFull.h"
#include "LayerPool.h"
#include "LayerBN.h"
#include "LayerRes.h"
#include "LayerCat.h"
#include "Net.h"

#endif
