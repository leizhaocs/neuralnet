#include "includes.h"

/* add a string field */
void Params::addString(const char *fieldname, const char *value)
{
    element e;
    std::string fieldname_str(fieldname);
    e.name = fieldname_str;
    e.type = STRING;
    std::string value_str(value);
    e.str = value_str;
    elems_.push_back(e);
}

/* add a scalari field */
void Params::addScalari(const char *fieldname, int value)
{
    element e;
    std::string fieldname_str(fieldname);
    e.name = fieldname_str;
    e.type = SCALARI;
    e.scalri = value;
    elems_.push_back(e);
}

/* add a scalarf field */
void Params::addScalarf(const char *fieldname, float value)
{
    element e;
    std::string fieldname_str(fieldname);
    e.name = fieldname_str;
    e.type = SCALARF;
    e.scalrf = value;
    elems_.push_back(e);
}

/* add a vectori field */
void Params::addVectori(const char *fieldname, std::vector<int> value)
{
    element e;
    std::string fieldname_str(fieldname);
    e.name = fieldname_str;
    e.type = VECTORI;
    e.vectri = value;
    elems_.push_back(e);
}

/* add a vectorf field */
void Params::addVectorf(const char *fieldname, std::vector<float> value)
{
    element e;
    std::string fieldname_str(fieldname);
    e.name = fieldname_str;
    e.type = VECTORF;
    e.vectrf = value;
    elems_.push_back(e);
}

/* check if a field exists */
bool Params::hasField(const char *fieldname)
{
    std::string fieldname_str(fieldname);
    for (int i = 0; i < elems_.size(); i++)
    {
        if (elems_[i].name.compare(fieldname_str) == 0)
        {
            return true;
        }
    }
    return false;
}

/* get a specific string field */
std::string Params::getString(const char *fieldname)
{
    std::string fieldname_str(fieldname);
    for (int i = 0; i < elems_.size(); i++)
    {
        if (elems_[i].name.compare(fieldname_str) == 0)
        {
            return elems_[i].str;
        }
    }
    return NULL;
}

/* get a specific scalari field */
int Params::getScalari(const char *fieldname)
{
    std::string fieldname_str(fieldname);
    for (int i = 0; i < elems_.size(); i++)
    {
        if (elems_[i].name.compare(fieldname_str) == 0)
        {
            return elems_[i].scalri;
        }
    }
    return 0;
}

/* get a specific scalarf field */
float Params::getScalarf(const char *fieldname)
{
    std::string fieldname_str(fieldname);
    for (int i = 0; i < elems_.size(); i++)
    {
        if (elems_[i].name.compare(fieldname_str) == 0)
        {
            return elems_[i].scalrf;
        }
    }
    return 0;
}

/* get a specific vectori field */
std::vector<int> Params::getVectori(const char *fieldname)
{
    std::string fieldname_str(fieldname);
    for (int i = 0; i < elems_.size(); i++)
    {
        if (elems_[i].name.compare(fieldname_str) == 0)
        {
            return elems_[i].vectri;
        }
    }
    return std::vector<int>();
}

/* get a specific vectorf field */
std::vector<float> Params::getVectorf(const char *fieldname)
{
    std::string fieldname_str(fieldname);
    for (int i = 0; i < elems_.size(); i++)
    {
        if (elems_[i].name.compare(fieldname_str) == 0)
        {
            return elems_[i].vectrf;
        }
    }
    return std::vector<float>();
}

/* get number of values in a specific field */
int Params::getNumel(const char *fieldname)
{
    std::string fieldname_str(fieldname);
    std::vector<element>::iterator it;
    for (int i = 0; i < elems_.size(); i++)
    {
        if (elems_[i].name.compare(fieldname_str) == 0)
        {
            if (elems_[i].type == STRING)
            {
                return 1;
            }
            else if (elems_[i].type == SCALARI || elems_[i].type == SCALARF)
            {
                return 1;
            }
            else if (elems_[i].type == VECTORI)
            {
                return elems_[i].vectri.size();
            }
            else if (elems_[i].type == VECTORF)
            {
                return elems_[i].vectrf.size();
            }
            return 0;
        }
    }
    return 0;
}
