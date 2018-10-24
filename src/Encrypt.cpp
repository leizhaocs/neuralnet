#include "includes.h"

/* constructor */
SEALWrapper::SEALWrapper()
{
    EncryptionParameters parms;
    parms.set_poly_modulus("1x^8192 + 1");
    parms.set_coeff_modulus(coeff_modulus_128(8192));
    parms.set_plain_modulus(1 << 25);

    SEALContext context(parms);

    KeyGenerator keygen(context);
    PublicKey public_key = keygen.public_key();
    SecretKey secret_key = keygen.secret_key();

    encoder_   = new FractionalEncoder(context.plain_modulus(), context.poly_modulus(), 64, 32);
    encryptor_ = new Encryptor(context, public_key);
    evaluator_ = new Evaluator(context);
    decryptor_ = new Decryptor(context, secret_key);

    ev_keys_ = new EvaluationKeys();
    keygen.generate_evaluation_keys(16, *ev_keys_);
}

/* destructor */
SEALWrapper::~SEALWrapper()
{
    delete encoder_;
    delete encryptor_;
    delete evaluator_;
    delete decryptor_;
    delete ev_keys_;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SEALWrapper *Encrypt::sealwrapper;

/* constructor */
Encrypt::Encrypt()
{
    use_plain = false;
}
Encrypt::Encrypt(float f)
{
    use_plain = false;
    value = f;
    from_value();
}
Encrypt::Encrypt(int i)
{
    use_plain = false;
    value = (float)i;
    from_value();
}
Encrypt::Encrypt(Plaintext p)
{
    use_plain = false;
    plain = p;
    from_plain();
}
Encrypt::Encrypt(Ciphertext c)
{
    use_plain = false;
    encrypted = c;
}

/* computation operators */
Encrypt operator +(Encrypt x, Encrypt y)
{
    Assert(!x.get_use_plain() || !y.get_use_plain(), "At least one operand must be both plaintext.");
    Ciphertext c;
    if (x.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->add_plain(y.get_encrypted(), x.get_plain(), c);
    }
    else if (y.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->add_plain(x.get_encrypted(), y.get_plain(), c);
    }
    else
    {
        Encrypt::sealwrapper->evaluator_->add(x.get_encrypted(), y.get_encrypted(), c);
    }
    Encrypt result(c);
    return result;
}
Encrypt operator +(Encrypt x, float y)
{
    Assert(!x.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(y);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->add_plain(x.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator +(float x, Encrypt y)
{
    Assert(!y.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(x);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->add_plain(y.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator +(Encrypt x, int y)
{
    Assert(!x.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)y);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->add_plain(x.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator +(int x, Encrypt y)
{
    Assert(!y.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)x);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->add_plain(y.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator -(Encrypt x, Encrypt y)
{
    Assert(!x.get_use_plain() || !y.get_use_plain(), "At least one operand must be both plaintext.");
    Ciphertext c;
    if (x.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->sub_plain(y.get_encrypted(), x.get_plain(), c);
    }
    else if (y.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->sub_plain(x.get_encrypted(), y.get_plain(), c);
    }
    else
    {
        Encrypt::sealwrapper->evaluator_->sub(x.get_encrypted(), y.get_encrypted(), c);
    }
    Encrypt result(c);
    return result;
}
Encrypt operator -(Encrypt x, float y)
{
    Assert(!x.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(y);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->sub_plain(x.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator -(float x, Encrypt y)
{
    Assert(!y.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(x);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->sub_plain(y.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator -(Encrypt x, int y)
{
    Assert(!x.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)y);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->sub_plain(x.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator -(int x, Encrypt y)
{
    Assert(!y.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)x);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->sub_plain(y.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator *(Encrypt x, Encrypt y)
{
    Assert(!x.get_use_plain() || !y.get_use_plain(), "At least one operand must be both plaintext.");
    Ciphertext c;
    if (x.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->multiply_plain(y.get_encrypted(), x.get_plain(), c);
    }
    else if (y.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->multiply_plain(x.get_encrypted(), y.get_plain(), c);
    }
    else
    {
        Encrypt::sealwrapper->evaluator_->multiply(x.get_encrypted(), y.get_encrypted(), c);
        Encrypt::sealwrapper->evaluator_->relinearize(c, *(Encrypt::sealwrapper->ev_keys_));
    }
    Encrypt result(c);
    return result;
}
Encrypt operator *(Encrypt x, float y)
{
    Assert(!x.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(y);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->multiply_plain(x.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator *(float x, Encrypt y)
{
    Assert(!y.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(x);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->multiply_plain(y.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator *(Encrypt x, int y)
{
    Assert(!x.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)y);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->multiply_plain(x.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator *(int x, Encrypt y)
{
    Assert(!y.get_use_plain(), "Can not operate on plaintext and raw value.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)x);
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->multiply_plain(y.get_encrypted(), p, c);
    Encrypt result(c);
    return result;
}
Encrypt operator /(Encrypt x, Encrypt y)
{
    std::cerr << "Encrypt does not support / operator" << std::endl;
    float temp1 = x.get_value();
    float temp2 = y.get_value();
    Encrypt result(temp1/temp2);
    return result;
}
Encrypt operator /(Encrypt x, float y)
{
    std::cerr << "Encrypt does not support / operator" << std::endl;
    float temp1 = x.get_value();
    Encrypt result(temp1/y);
    return result;
}
Encrypt operator /(float x, Encrypt y)
{
    std::cerr << "Encrypt does not support / operator" << std::endl;
    float temp2 = y.get_value();
    Encrypt result(x/temp2);
    return result;
}
Encrypt operator /(Encrypt x, int y)
{
    std::cerr << "Encrypt does not support / operator" << std::endl;
    float temp1 = x.get_value();
    Encrypt result(temp1/y);
    return result;
}
Encrypt operator /(int x, Encrypt y)
{
    std::cerr << "Encrypt does not support / operator" << std::endl;
    float temp2 = y.get_value();
    Encrypt result(x/temp2);
    return result;
}

/* assignment operators */
Encrypt &Encrypt::operator =(Encrypt x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    if (x.get_use_plain())
    {
        plain = x.get_plain();
        from_plain();
    }
    else
    {
        encrypted = x.get_encrypted();
    }
    return *this;
}
Encrypt &Encrypt::operator =(float x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    value = x;
    from_value();
    return *this;
}
Encrypt &Encrypt::operator =(int x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    value = (float)x;
    from_value();
    return *this;
}
Encrypt &Encrypt::operator +=(Encrypt x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    if (x.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->add_plain(encrypted, x.get_plain(), encrypted);
    }
    else
    {
        Encrypt::sealwrapper->evaluator_->add(encrypted, x.get_encrypted(), encrypted);
    }
    return *this;
}
Encrypt &Encrypt::operator +=(float x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(x);
    Encrypt::sealwrapper->evaluator_->add_plain(encrypted, p, encrypted);
    return *this;
}
Encrypt &Encrypt::operator +=(int x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)x);
    Encrypt::sealwrapper->evaluator_->add_plain(encrypted, p, encrypted);
    return *this;
}
Encrypt &Encrypt::operator -=(Encrypt x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    if (x.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->sub_plain(encrypted, x.get_plain(), encrypted);
    }
    else
    {
        Encrypt::sealwrapper->evaluator_->sub(encrypted, x.get_encrypted(), encrypted);
    }
    return *this;
}
Encrypt &Encrypt::operator -=(float x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(x);
    Encrypt::sealwrapper->evaluator_->sub_plain(encrypted, p, encrypted);
    return *this;
}
Encrypt &Encrypt::operator -=(int x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)x);
    Encrypt::sealwrapper->evaluator_->sub_plain(encrypted, p, encrypted);
    return *this;
}
Encrypt &Encrypt::operator *=(Encrypt x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    if (x.get_use_plain())
    {
        Encrypt::sealwrapper->evaluator_->multiply_plain(encrypted, x.get_plain(), encrypted);
    }
    else
    {
        Encrypt::sealwrapper->evaluator_->multiply(encrypted, x.get_encrypted(), encrypted);
        Encrypt::sealwrapper->evaluator_->relinearize(encrypted, *(Encrypt::sealwrapper->ev_keys_));
    }
    return *this;
}
Encrypt &Encrypt::operator *=(float x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode(x);
    Encrypt::sealwrapper->evaluator_->multiply_plain(encrypted, p, encrypted);
    return *this;
}
Encrypt &Encrypt::operator *=(int x)
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Plaintext p = Encrypt::sealwrapper->encoder_->encode((float)x);
    Encrypt::sealwrapper->evaluator_->multiply_plain(encrypted, p, encrypted);
    return *this;
}
Encrypt &Encrypt::operator /=(Encrypt x)
{
    std::cerr << "Encrypt does not support /= operator" << std::endl;
    float temp = get_value() / x.get_value();
    set_value(temp);
    return *this;
}
Encrypt &Encrypt::operator /=(float x)
{
    std::cerr << "Encrypt does not support /= operator" << std::endl;
    float temp = get_value() / x;
    set_value(temp);
    return *this;
}
Encrypt &Encrypt::operator /=(int x)
{
    std::cerr << "Encrypt does not support /= operator" << std::endl;
    float temp = get_value() / x;
    set_value(temp);
    return *this;
}

/* comparison operators */
bool operator ==(Encrypt x, Encrypt y)
{
    std::cerr << "Encrypt does not support == operator" << std::endl;
    return x.get_value() == y.get_value();
}
bool operator ==(Encrypt x, float y)
{
    std::cerr << "Encrypt does not support == operator" << std::endl;
    return x.get_value() == y;
}
bool operator ==(float x, Encrypt y)
{
    std::cerr << "Encrypt does not support == operator" << std::endl;
    return x == y.get_value();
}
bool operator ==(Encrypt x, int y)
{
    std::cerr << "Encrypt does not support == operator" << std::endl;
    return x.get_value() == y;
}
bool operator ==(int x, Encrypt y)
{
    std::cerr << "Encrypt does not support == operator" << std::endl;
    return x == y.get_value();
}
bool operator !=(Encrypt x, Encrypt y)
{
    std::cerr << "Encrypt does not support != operator" << std::endl;
    return x.get_value() != y.get_value();
}
bool operator !=(Encrypt x, float y)
{
    std::cerr << "Encrypt does not support != operator" << std::endl;
    return x.get_value() != y;
}
bool operator !=(float x, Encrypt y)
{
    std::cerr << "Encrypt does not support != operator" << std::endl;
    return x != y.get_value();
}
bool operator !=(Encrypt x, int y)
{
    std::cerr << "Encrypt does not support != operator" << std::endl;
    return x.get_value() != y;
}
bool operator !=(int x, Encrypt y)
{
    std::cerr << "Encrypt does not support != operator" << std::endl;
    return x != y.get_value();
}
bool operator >(Encrypt x, Encrypt y)
{
    std::cerr << "Encrypt does not support > operator" << std::endl;
    return x.get_value() > y.get_value();
}
bool operator >(Encrypt x, float y)
{
    std::cerr << "Encrypt does not support > operator" << std::endl;
    return x.get_value() > y;
}
bool operator >(float x, Encrypt y)
{
    std::cerr << "Encrypt does not support > operator" << std::endl;
    return x > y.get_value();
}
bool operator >(Encrypt x, int y)
{
    std::cerr << "Encrypt does not support > operator" << std::endl;
    return x.get_value() > y;
}
bool operator >(int x, Encrypt y)
{
    std::cerr << "Encrypt does not support > operator" << std::endl;
    return x > y.get_value();
}
bool operator <(Encrypt x, Encrypt y)
{
    std::cerr << "Encrypt does not support < operator" << std::endl;
    return x.get_value() < y.get_value();
}
bool operator <(Encrypt x, float y)
{
    std::cerr << "Encrypt does not support < operator" << std::endl;
    return x.get_value() < y;
}
bool operator <(float x, Encrypt y)
{
    std::cerr << "Encrypt does not support < operator" << std::endl;
    return x < y.get_value();
}
bool operator <(Encrypt x, int y)
{
    std::cerr << "Encrypt does not support < operator" << std::endl;
    return x.get_value() < y;
}
bool operator <(int x, Encrypt y)
{
    std::cerr << "Encrypt does not support < operator" << std::endl;
    return x < y.get_value();
}
bool operator >=(Encrypt x, Encrypt y)
{
    std::cerr << "Encrypt does not support >= operator" << std::endl;
    return x.get_value() >= y.get_value();
}
bool operator >=(Encrypt x, float y)
{
    std::cerr << "Encrypt does not support >= operator" << std::endl;
    return x.get_value() >= y;
}
bool operator >=(float x, Encrypt y)
{
    std::cerr << "Encrypt does not support >= operator" << std::endl;
    return x >= y.get_value();
}
bool operator >=(Encrypt x, int y)
{
    std::cerr << "Encrypt does not support >= operator" << std::endl;
    return x.get_value() >= y;
}
bool operator >=(int x, Encrypt y)
{
    std::cerr << "Encrypt does not support >= operator" << std::endl;
    return x >= y.get_value();
}
bool operator <=(Encrypt x, Encrypt y)
{
    std::cerr << "Encrypt does not support <= operator" << std::endl;
    return x.get_value() <= y.get_value();
}
bool operator <=(Encrypt x, float y)
{
    std::cerr << "Encrypt does not support <= operator" << std::endl;
    return x.get_value() <= y;
}
bool operator <=(float x, Encrypt y)
{
    std::cerr << "Encrypt does not support <= operator" << std::endl;
    return x <= y.get_value();
}
bool operator <=(Encrypt x, int y)
{
    std::cerr << "Encrypt does not support <= operator" << std::endl;
    return x.get_value() <= y;
}
bool operator <=(int x, Encrypt y)
{
    std::cerr << "Encrypt does not support <= operator" << std::endl;
    return x <= y.get_value();
}

/* sign operators */
Encrypt Encrypt::operator +()
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Encrypt result(encrypted);
    return result;
}
Encrypt Encrypt::operator -()
{
    Assert(!get_use_plain(), "If only plain text, then value can not be modifed.");
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->negate(encrypted, c);
    Encrypt result(c);
    return result;
}

/* converter */
Encrypt::operator float()
{
    from_encrypted();
    return value;
}
Encrypt::operator int()
{
    from_encrypted();
    return (int)value;
}

/* stream operators */
std::ostream& operator <<(std::ostream& out_file, Encrypt &x)
{
    x.from_encrypted();
    out_file << x.get_value();
    return (out_file);
}
std::istream& operator >>(std::istream& in_file, Encrypt &x)
{
    float temp;
    in_file >> temp;
    x.set_value(temp);
    x.from_value();
    return (in_file);
}

/* add many */
void Encrypt::add_many(std::vector<Encrypt> &operands)
{
    std::vector<Ciphertext> encrypted_operands;
    for (int i = 0; i < operands.size(); i++)
    {
        encrypted_operands.emplace_back(operands[i].get_encrypted());
    }
    Encrypt::sealwrapper->evaluator_->add_many(encrypted_operands, encrypted);
}

/* square */
Encrypt Encrypt::square()
{
    Ciphertext c;
    Encrypt::sealwrapper->evaluator_->square(encrypted, c);
    Encrypt::sealwrapper->evaluator_->relinearize(c, *(Encrypt::sealwrapper->ev_keys_));
    Encrypt result(c);
    return result;
}

/* save and load encrypted into/from file */
void Encrypt::save(std::ostream &stream)
{
    encrypted.save(stream);
}
void Encrypt::load(std::istream &stream)
{
    encrypted.load(stream);
}

/* getters */
bool Encrypt::get_use_plain()
{
    return use_plain;
}
float Encrypt::get_value()
{
    from_encrypted();
    return value;
}
Plaintext Encrypt::get_plain()
{
    from_encrypted();
    return plain;
}
Ciphertext Encrypt::get_encrypted()
{
    return encrypted;
}

/* setters */
void Encrypt::set_use_plain(bool u)
{
    if (!use_plain && u)
    {
        from_encrypted();
    }
    use_plain = u;
}
void Encrypt::set_value(float v)
{
    value = v;
    from_value();
}
void Encrypt::set_plain(Plaintext p)
{
    plain = p;
    from_plain();
}
void Encrypt::set_encrypted(Ciphertext e)
{
    encrypted = e;
}

/* get cipertext size */
int Encrypt::size()
{
    return encrypted.size();
}

/* get the noise budget */
int Encrypt::noise_budget()
{
    return Encrypt::sealwrapper->decryptor_->invariant_noise_budget(encrypted);
}

/* initialize SEAL */
void Encrypt::initEncrypt()
{
    sealwrapper = new SEALWrapper;
}

/* given raw value, compute the plaintext and cyphertext */
void Encrypt::from_value()
{
    plain = Encrypt::sealwrapper->encoder_->encode(value);
    Encrypt::sealwrapper->encryptor_->encrypt(plain, encrypted);
}

/* given plaintext, compute the raw value and cyphertext */
void Encrypt::from_plain()
{
    value = (float)Encrypt::sealwrapper->encoder_->decode(plain);
    Encrypt::sealwrapper->encryptor_->encrypt(plain, encrypted);
}

/* given cyphertext, compute the raw value and plaintext */
void Encrypt::from_encrypted()
{
    Encrypt::sealwrapper->decryptor_->decrypt(encrypted, plain);
    value = (float)Encrypt::sealwrapper->encoder_->decode(plain);
}
