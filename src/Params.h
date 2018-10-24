#ifndef _PARAMS_H_
#define _PARAMS_H_

#include "includes.h"

/* arguments pass to the network */
class Params
{
private:
    /* type */
    enum elem_type
    {
        STRING,   // string
        SCALARI,  // integer scalar
        SCALARF,  // floating point scalar
        VECTORI,  // integer vector
        VECTORF   // floating point vector
    };

    /* element of one field */
    struct element
    {
        std::string name;     // name of this field
        elem_type type;       // type of the data

        std::string str;           // string
        int scalri;                // integer scalar
        float scalrf;              // floating point scalar
        std::vector<int> vectri;   // integer vector
        std::vector<float> vectrf; // floating point vector
    };

    std::vector<struct element> elems_;  // all the elements

public:
    /* add a string field */
    void addString(const char *fieldname, const char *value);

    /* add a scalari field */
    void addScalari(const char *fieldname, int value);

    /* add a scalarf field */
    void addScalarf(const char *fieldname, float value);

    /* add a vectori field */
    void addVectori(const char *fieldname, std::vector<int> value);

    /* add a vectorf field */
    void addVectorf(const char *fieldname, std::vector<float> value);

    /* check if a field exists */
    bool hasField(const char *fieldname);

    /* get a specific string field */
    std::string getString(const char *fieldname);

    /* get a specific scalari field */
    int getScalari(const char *fieldname);

    /* get a specific scalarf field */
    float getScalarf(const char *fieldname);

    /* get a specific vectori field */
    std::vector<int> getVectori(const char *fieldname);

    /* get a specific vectorf field */
    std::vector<float> getVectorf(const char *fieldname);

    /* get number of values in a specific field */
    int getNumel(const char *fieldname);
};

#endif
