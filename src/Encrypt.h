#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

#include "includes.h"

using namespace seal;

class SEALWrapper
{
public:
    /* constructor */
    SEALWrapper();

    /* destructor */
    ~SEALWrapper();

    FractionalEncoder *encoder_;   // encoder and decoder
    Encryptor *encryptor_;         // encryptor
    Evaluator *evaluator_;         // evaluator
    Decryptor *decryptor_;         // decryptro
    EvaluationKeys *ev_keys_;      // evaluation keys for relinearization
};

/* 1. the encrypted field is always up to date, i.e. after computation,
 *    only encrypted field is updated
 * 2. when assigning new values to value field and plain field, all the
 *    three fields are updated automatically
 * 3. when access the value field or plain field, all the three fields
 *    are updated automatically
 * 4. if use_plain field is set, then when used as operand, use its plain
 *    field for computation
 * 5. if use_plain field is set, then it can not be computated with raw
 *    value
 * 6. if use_plain field is set, then it can not be modified unless
      use_plain field is cleared */
class Encrypt
{
public:
    /* constructor */
    Encrypt();
    Encrypt(float f);
    Encrypt(int i);
    Encrypt(Plaintext p);
    Encrypt(Ciphertext c);

    /* computation operators */
    friend Encrypt operator +(Encrypt x, Encrypt y);
    friend Encrypt operator +(Encrypt x, float y);
    friend Encrypt operator +(float x, Encrypt y);
    friend Encrypt operator +(Encrypt x, int y);
    friend Encrypt operator +(int x, Encrypt y);
    friend Encrypt operator -(Encrypt x, Encrypt y);
    friend Encrypt operator -(Encrypt x, float y);
    friend Encrypt operator -(float x, Encrypt y);
    friend Encrypt operator -(Encrypt x, int y);
    friend Encrypt operator -(int x, Encrypt y);
    friend Encrypt operator *(Encrypt x, Encrypt y);
    friend Encrypt operator *(Encrypt x, float y);
    friend Encrypt operator *(float x, Encrypt y);
    friend Encrypt operator *(Encrypt x, int y);
    friend Encrypt operator *(int x, Encrypt y);
    friend Encrypt operator /(Encrypt x, Encrypt y);
    friend Encrypt operator /(Encrypt x, float y);
    friend Encrypt operator /(float x, Encrypt y);
    friend Encrypt operator /(Encrypt x, int y);
    friend Encrypt operator /(int x, Encrypt y);

    /* assignment operators */
    Encrypt &operator =(Encrypt x);
    Encrypt &operator =(float x);
    Encrypt &operator =(int x);
    Encrypt &operator +=(Encrypt x);
    Encrypt &operator +=(float x);
    Encrypt &operator +=(int x);
    Encrypt &operator -=(Encrypt x);
    Encrypt &operator -=(float x);
    Encrypt &operator -=(int x);
    Encrypt &operator *=(Encrypt x);
    Encrypt &operator *=(float x);
    Encrypt &operator *=(int x);
    Encrypt &operator /=(Encrypt x);
    Encrypt &operator /=(float x);
    Encrypt &operator /=(int x);

    /* comparison operators */
    friend bool operator ==(Encrypt x, Encrypt y);
    friend bool operator ==(Encrypt x, float y);
    friend bool operator ==(float x, Encrypt y);
    friend bool operator ==(Encrypt x, int y);
    friend bool operator ==(int x, Encrypt y);
    friend bool operator !=(Encrypt x, Encrypt y);
    friend bool operator !=(Encrypt x, float y);
    friend bool operator !=(float x, Encrypt y);
    friend bool operator !=(Encrypt x, int y);
    friend bool operator !=(int x, Encrypt y);
    friend bool operator >(Encrypt x, Encrypt y);
    friend bool operator >(Encrypt x, float y);
    friend bool operator >(float x, Encrypt y);
    friend bool operator >(Encrypt x, int y);
    friend bool operator >(int x, Encrypt y);
    friend bool operator <(Encrypt x, Encrypt y);
    friend bool operator <(Encrypt x, float y);
    friend bool operator <(float x, Encrypt y);
    friend bool operator <(Encrypt x, int y);
    friend bool operator <(int x, Encrypt y);
    friend bool operator >=(Encrypt x, Encrypt y);
    friend bool operator >=(Encrypt x, float y);
    friend bool operator >=(float x, Encrypt y);
    friend bool operator >=(Encrypt x, int y);
    friend bool operator >=(int x, Encrypt y);
    friend bool operator <=(Encrypt x, Encrypt y);
    friend bool operator <=(Encrypt x, float y);
    friend bool operator <=(float x, Encrypt y);
    friend bool operator <=(Encrypt x, int y);
    friend bool operator <=(int x, Encrypt y);

    /* sign operators */
    Encrypt operator +();
    Encrypt operator -();

    /* converter */
    operator float();
    operator int();

    /* stream operators */
    friend std::ostream& operator <<(std::ostream& out_file, Encrypt &x);
    friend std::istream& operator >>(std::istream& in_file, Encrypt &x);

    /* add many */
    void add_many(std::vector<Encrypt> &operands);

    /* square */
    Encrypt square();

    /* save and load encrypted into/from file */
    void save(std::ostream &stream);
    void load(std::istream &stream);

    /* getters */
    bool get_use_plain();
    float get_value();
    Plaintext get_plain();
    Ciphertext get_encrypted();

    /* setters */
    void set_use_plain(bool u);
    void set_value(float v);
    void set_plain(Plaintext p);
    void set_encrypted(Ciphertext e);

    /* get cipertext size */
    int size();

    /* get the noise budget */
    int noise_budget();

    /* initialize SEAL */
    static void initEncrypt();

private:
    /* given raw value, compute the plaintext and cyphertext */
    void from_value();

    /* given plaintext, compute the raw value and cyphertext */
    void from_plain();

    /* given cyphertext, compute the raw value and plaintext */
    void from_encrypted();

    bool use_plain;        // only encode, but not encrypt
    float value;           // original value
    Plaintext plain;       // encoded plain text
    Ciphertext encrypted;  // encrypted text

    static SEALWrapper *sealwrapper;
};

#endif
