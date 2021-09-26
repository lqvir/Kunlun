#ifndef KUNLUN_CRYPTO_BIGINT_HPP_
#define KUNLUN_CRYPTO_BIGINT_HPP_

#include "std.inc"
#include "openssl.inc"
#include "context.hpp"

// wrapper class for openssl BIGNUM

class BigInt{
public:
    BIGNUM* bn_ptr;
    
    // constructor functions
    BigInt();
    BigInt(const BigInt& other);
    BigInt(const BIGNUM *other);
    BigInt(size_t number);

    // destuctor function
    ~BigInt();

    // arithmetic operations 
    
    // Returns a BigInt whose value is (- *this). Causes a check failure if the operation fails.
    BigInt Negate() const;

    // Returns a BigInt whose value is (*this + other). Causes a check failure if the operation fails.
    BigInt Add(const BigInt& other) const;

    // Returns a BigInt whose value is (*this - other). Causes a check failure if the operation fails.
    BigInt Sub(const BigInt& other) const;

    // Returns a BigInt whose value is (*this * other). Causes a check failure if the operation fails.
    BigInt Mul(const BigInt& other) const;

    // Returns a BigInt whose value is (*this / other).
    // Causes a check failure if the remainder != 0 or if the operation fails.
    BigInt Div(const BigInt& other) const;

    // Returns a BigInt whose value is *this / val, rounding towards zero.
    // Causes a check failure if the remainder != 0 or if the operation fails.
    BigInt DivAndTruncate(const BigInt& other) const;

    // Returns a BigInt whose value is (*this ^ exponent).
    // Causes a check failure if the operation fails.
    BigInt Exp(const BigInt& exponent) const;

    BigInt Square() const;

    // Returns a BigInt whose value is (*this mod m).
    BigInt Mod(const BigInt& modulus) const;

    // Returns a BigInt whose value is (*this + other mod m).
    // Causes a check failure if the operation fails.
    BigInt ModAdd(const BigInt& other, const BigInt& modulus) const;

    // Returns a BigInt whose value is (*this - other mod m).
    // Causes a check failure if the operation fails.
    BigInt ModSub(const BigInt& other, const BigInt& modulus) const;

    // Returns a BigInt whose value is (*this * other mod m).
    // For efficiency, use Montgomery multiplication module if this is done multiple times with the same modulus.
    // Causes a check failure if the operation fails.
    BigInt ModMul(const BigInt& other, const BigInt& modulus) const;

    // Returns a BigInt whose value is (*this ^ exponent mod m).
    // Causes a check failure if the operation fails.
    BigInt ModExp(const BigInt& exponent, const BigInt& modulus) const;

    // Return a BigInt whose value is (*this ^ 2 mod m).
    // Causes a check failure if the operation fails.
    BigInt ModSquare(const BigInt& modulus) const;

    // Returns a BigInt whose value is (*this ^ -1 mod m).
    // Causes a check failure if the operation fails.
    BigInt ModInverse(const BigInt& modulus) const;

    // Returns r such that r^2 == *this mod p.
    // Causes a check failure if the operation fails.
    BigInt ModSquareRoot(const BigInt& modulus) const;

    // Computes -a mod m.
    // Causes a check failure if the operation fails.
    BigInt ModNegate(const BigInt& modulus) const;

    // Computes the greatest common divisor of *this and other.
    // Causes a check failure if the operation fails.
    BigInt GCD(const BigInt& other) const;

    
    // logic operations

    // Compares this BigInt with the specified BigInt.
    // Returns -1 if *this < other, 0 if *this == other and 1 if *this > other.
    int CompareTo(const BigInt& other) const;

    // Returns a BigInt whose value is (*this >> n).
    BigInt Lshift(int n) const; 

    // Returns a BigInt whose value is (*this << n).
    BigInt Rshift(int n) const;

    // operator overload

    inline BigInt& operator=(const BigInt& other) { BN_copy(this->bn_ptr, other.bn_ptr); return *this; }

    inline BigInt operator-() const { return this->Negate(); }

    inline BigInt operator+(const BigInt& other) const { return this->Add(other); }

    inline BigInt operator*(const BigInt& other) const { return this->Mul(other); }

    inline BigInt operator-(const BigInt& other) const { return this->Sub(other); }

    inline BigInt operator/(const BigInt& other) const { return this->Div(other); }

    inline BigInt& operator+=(const BigInt& other) { return *this = *this + other; }

    inline BigInt& operator*=(const BigInt& other) { return *this = *this * other; }

    inline BigInt& operator-=(const BigInt& other) { return *this = *this - other; }

    inline BigInt& operator/=(const BigInt& other) { return *this = *this / other; }

    inline bool operator==(const BigInt& other) const { return 0 == this->CompareTo(other); }

    inline bool operator!=(const BigInt& other) const { return !(*this == other); }

    inline bool operator<(const BigInt& other) const { return -1 == this->CompareTo(other); }

    inline bool operator>(const BigInt& other) const { return 1 == this->CompareTo(other); }

    inline bool operator<=(const BigInt& other) const { return this->CompareTo(other) <= 0; }

    inline bool operator>=(const BigInt& other) const { return this->CompareTo(other) >= 0; }

    inline BigInt operator%(const BigInt& modulus) const { return this->Mod(modulus); }

    inline BigInt operator>>(int n) { return this->Rshift(n); }

    inline BigInt operator<<(int n) { return this->Lshift(n); }

    inline BigInt& operator%=(const BigInt& other) { return *this = *this % other; }

    inline BigInt& operator>>=(int n) { return *this = *this >> n; }

    inline BigInt& operator<<=(int n) { return *this = *this << n; }



    // serialization and deserialization 

    /* save bigint object binary form */  
    void Serialize(std::ofstream &fout);

    /* recover bigint object from binary file */
    void Deserialize(std::ifstream &fin);

    friend std::ofstream &operator<<(std::ofstream &fout, const BigInt &A); 
    friend std::ifstream &operator>>(std::ifstream &fin, BigInt &A); 

    // attribute test routines

    inline int GetTheNthBit(size_t j) const;

    // returns 0 on error (if r is already shorter than n bits)
    // return value in that case should be the original value so there is no need to have error checking here.
    inline BigInt GetLastNBits(int n) const {
        BigInt result = *this;
        BN_mask_bits(result.bn_ptr, n);
        return result;
    }

    // returns the bit length of this BigInt.
    inline size_t GetBitLength() const { return BN_num_bits(this->bn_ptr); }
    inline size_t GetByteLength() const { return BN_num_bytes(this->bn_ptr); }

    inline bool IsBitSet(int n) const { return BN_is_bit_set(this->bn_ptr, n); }

    inline bool IsZero() const { return BN_is_zero(this->bn_ptr); }

    inline bool IsOne() const { return BN_is_one(this->bn_ptr); }

    inline bool IsNonNegative() const { 
        if (BN_is_negative(this->bn_ptr) == 1) return false; 
        else return true;
    }

    bool IsPrime(double prime_error_probability) const;

    bool IsSafePrime(double prime_error_probability) const;

    // print BigInt object, mode = {10, 16}
    void Print() const; 
    
    void Print(std::string note) const; 
};

// global bigint objects
const static BigInt bn_0(uint64_t(0)); 
const static BigInt bn_1(uint64_t(1)); 
const static BigInt bn_2(uint64_t(2)); 
const static BigInt bn_3(uint64_t(3));  


// Copies the given BigInt.
BigInt::BigInt(){ 
    this->bn_ptr = BN_new(); 
}

BigInt::BigInt(const BigInt& other){
    this->bn_ptr = BN_new();
    BN_copy(this->bn_ptr, other.bn_ptr);
}

BigInt::BigInt(const BIGNUM *other){
    this->bn_ptr = BN_new(); 
    BN_copy(this->bn_ptr, other); 
}

// Creates a new BigInt object from the number.
BigInt::BigInt(size_t number){
    this->bn_ptr = BN_new();
    CRYPTO_CHECK(BN_set_word(this->bn_ptr, number));
}

BigInt::~BigInt(){
    BN_free(this->bn_ptr); 
}


// Converts this BigInt to a uint64_t value. Returns an INVALID_ARGUMENT
uint64_t ToInt64(const BigInt& a)
{
    uint64_t result = BN_get_word(a.bn_ptr);
    return std::move(result);
}

// Creates a new BigInt object from a bytes string.
BigInt BigIntFromByteString(const std::string& str)
{
    BigInt result; 
    BN_bin2bn(reinterpret_cast<const unsigned char*>(str.data()), str.size(), result.bn_ptr);
    return std::move(result); 
}
  
std::string BigIntToByteString(const BigInt &a)
{
    size_t LEN = a.GetByteLength();
    unsigned char buffer[LEN];
    memset(buffer, 0, LEN);  
    BN_bn2bin(a.bn_ptr, buffer);
    std::string result(reinterpret_cast<char *>(buffer), LEN); 
    return std::move(result);
}  

/* convert a Big number to string */
std::string BigIntToHexString(const BigInt &a)
{
    std::stringstream ss; 
    ss << BN_bn2hex(a.bn_ptr);
    return ss.str();  
}


// Returns a BigInt whose value is (- *this).
// Causes a check failure if the operation fails.
BigInt BigInt::Negate() const {
    BigInt result = *this;
    BN_set_negative(result.bn_ptr, !BN_is_negative(result.bn_ptr));
    return std::move(result);
}

// Returns a BigInt whose value is (*this + val).
// Causes a check failure if the operation fails.
BigInt BigInt::Add(const BigInt& other) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_add(result.bn_ptr, this->bn_ptr, other.bn_ptr));
    return std::move(result);
}

// Returns a BigInt whose value is (*this - val).
// Causes a check failure if the operation fails.
BigInt BigInt::Sub(const BigInt& other) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_sub(result.bn_ptr, this->bn_ptr, other.bn_ptr));
    return std::move(result);
}

// Returns a BigInt whose value is (*this * val).
// Causes a check failure if the operation fails.
BigInt BigInt::Mul(const BigInt& other) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_mul(result.bn_ptr, this->bn_ptr, other.bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns a BigInt whose value is (*this / val).
// Causes a check failure if the remainder != 0 or if the operation fails.
BigInt BigInt::Div(const BigInt& other) const {
    BigInt result;
    BigInt remainder;
    CRYPTO_CHECK(1 == BN_div(result.bn_ptr, remainder.bn_ptr, this->bn_ptr, other.bn_ptr, bn_ctx));
    if (BN_is_zero(remainder.bn_ptr)){
        std::cerr << "Use DivAndTruncate() instead of Div() if you want truncated division." << std::endl;  
    } 
    return std::move(result);
}

// Returns a BigInt whose value is *this / val, rounding towards zero.
// Causes a check failure if the remainder != 0 or if the operation fails.
BigInt BigInt::DivAndTruncate(const BigInt& other) const {
    BigInt result;
    BigInt remainder;
    CRYPTO_CHECK(1 == BN_div(result.bn_ptr, remainder.bn_ptr, this->bn_ptr, other.bn_ptr, bn_ctx));
    return std::move(result);
}

// Compares this BigInt with the specified BigInt.
// Returns -1 if *this < val, 0 if *this == val and 1 if *this > val.
int BigInt::CompareTo(const BigInt& other) const {
    return BN_cmp(this->bn_ptr, other.bn_ptr);
}

// Returns a BigInt whose value is (*this ^ exponent).
// Causes a check failure if the operation fails.
BigInt BigInt::Exp(const BigInt& exponent) const{
    BigInt result;
    CRYPTO_CHECK(1 == BN_exp(result.bn_ptr, this->bn_ptr, exponent.bn_ptr, bn_ctx));
    return std::move(result);
}

// return square
BigInt BigInt::Square() const{
    BigInt result;
    CRYPTO_CHECK(1 == BN_sqr(result.bn_ptr, this->bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns a BigInt whose value is (*this mod m).
BigInt BigInt::Mod(const BigInt& modulus) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_nnmod(result.bn_ptr, this->bn_ptr, modulus.bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns a BigInt whose value is (*this + val mod m).
// Causes a check failure if the operation fails.
BigInt BigInt::ModAdd(const BigInt& other, const BigInt& modulus) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_mod_add(result.bn_ptr, this->bn_ptr, other.bn_ptr, modulus.bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns a BigInt whose value is (*this - val mod m).
// Causes a check failure if the operation fails.
BigInt BigInt::ModSub(const BigInt& other, const BigInt& modulus) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_mod_sub(result.bn_ptr, this->bn_ptr, other.bn_ptr, modulus.bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns a BigInt whose value is (*this * val mod m).
BigInt BigInt::ModMul(const BigInt& other, const BigInt& modulus) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_mod_mul(result.bn_ptr, this->bn_ptr, other.bn_ptr, modulus.bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns a BigInt whose value is (*this ^ exponent mod m).
// Causes a check failure if the operation fails.
BigInt BigInt::ModExp(const BigInt& exponent, const BigInt& modulus) const {
    if (exponent.IsNonNegative() == false){
        std::cerr << "Cannot use a negative exponent in BigInt ModExp." << std::endl; 
    } 
    BigInt result;
    CRYPTO_CHECK(1 == BN_mod_exp(result.bn_ptr, this->bn_ptr, exponent.bn_ptr, modulus.bn_ptr, bn_ctx));

    return std::move(result);
}

// Return a BigInt whose value is (*this^2 mod m).
// Causes a check failure if the operation fails.
BigInt BigInt::ModSquare(const BigInt& modulus) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_mod_sqr(result.bn_ptr, this->bn_ptr, modulus.bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns a BigInt whose value is (*this ^ -1 mod m).
// Causes a check failure if the operation fails.
BigInt BigInt::ModInverse(const BigInt& modulus) const {
    BigInt result;
    CRYPTO_CHECK(nullptr != BN_mod_inverse(result.bn_ptr, this->bn_ptr, modulus.bn_ptr, bn_ctx));
    return std::move(result);
}

// Returns r such that r^2 == *this mod p.
// Causes a check failure if the operation fails.
BigInt BigInt::ModSquareRoot(const BigInt& modulus) const {
    BigInt result;
    CRYPTO_CHECK(nullptr != BN_mod_sqrt(result.bn_ptr, bn_ptr, modulus.bn_ptr, bn_ctx));
    return std::move(result);
}

// Computes -a mod m.
// Causes a check failure if the operation fails.
BigInt BigInt::ModNegate(const BigInt& modulus) const {
    if (IsZero()) {
        return *this;
    }
    return modulus - Mod(modulus);
}

// Returns a BigInt whose value is (*this >> n).
BigInt BigInt::Lshift(int n) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_lshift(result.bn_ptr, this->bn_ptr, n));
    return std::move(result);
}

// Returns a BigInt whose value is (*this << n).
// Causes a check failure if the operation fails.
BigInt BigInt::Rshift(int n) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_rshift(result.bn_ptr, this->bn_ptr, n));
    return std::move(result);
}

// Computes the greatest common divisor of *this and val.
// Causes a check failure if the operation fails.
BigInt BigInt::GCD(const BigInt& other) const {
    BigInt result;
    CRYPTO_CHECK(1 == BN_gcd(result.bn_ptr, this->bn_ptr, other.bn_ptr, bn_ctx));
    return std::move(result);
}


// Returns False if the number is composite
// True if it is prime with an error probability of 1e-40, which gives at least 128 bit security.
bool BigInt::IsPrime(double prime_error_probability) const {
    int rounds = static_cast<int>(ceil(-log(prime_error_probability) / log(4)));
    return (1 == BN_is_prime_ex(this->bn_ptr, rounds, bn_ctx, nullptr));
}

bool BigInt::IsSafePrime(double prime_error_probability = 1e-40) const {
    return IsPrime(prime_error_probability) && ((*this - bn_1) / bn_2).IsPrime(prime_error_probability);
}


BigInt GenRandomBigIntLessThan(const BIGNUM* max) {
    BigInt result;
    CRYPTO_CHECK(1 == BN_rand_range(result.bn_ptr, max));
    // BN_priv_rand_range(result.bn_ptr, max.bn_ptr);
    return std::move(result);
}


BigInt GenRandomBigIntLessThan(const BigInt& max) {
    BigInt result;
    CRYPTO_CHECK(1 == BN_rand_range(result.bn_ptr, max.bn_ptr));
    // BN_priv_rand_range(result.bn_ptr, max.bn_ptr);
    return std::move(result);
}

// Generates a cryptographically strong pseudo-random in the range [start, end).
BigInt GenRandomBigIntBetween(const BigInt& start, const BigInt& end) {
    if (start > end) {
        std::cerr << "provided range is invalid" << std::endl; 
    }
    return GenRandomBigIntLessThan(end - start) + start;
}

// Generates a cryptographically strong pseudo-random bytes of the specified length.
std::string GenRandomBytes(int num_bytes) {
    if (num_bytes < 0){
        std::cerr << "num_bytes must be nonnegative, provided value was" << num_bytes << "."<<std::endl;
    } 
    std::unique_ptr<unsigned char[]> bytes(new unsigned char[num_bytes]);
    CRYPTO_CHECK(1 == RAND_bytes(bytes.get(), num_bytes));
    return std::string(reinterpret_cast<char*>(bytes.get()), num_bytes);
}

// Returns a BigNum that is relatively prime to the num and less than the num.
BigInt GenCoPrimeLessThan(const BigInt& num) {
    BigInt rand_num = GenRandomBigIntLessThan(num);
    while (rand_num.GCD(num) > bn_1) {
        rand_num = GenRandomBigIntLessThan(num);
    }
    return std::move(rand_num);
}

// Creates a safe prime BigNum with the given bit-length.
BigInt GenSafePrime(int prime_length) {
    BigInt result;
    CRYPTO_CHECK(1 == BN_generate_prime_ex(result.bn_ptr, prime_length, 1, nullptr, nullptr, nullptr));
    return std::move(result);
}

// Creates a prime BigNum with the given bit-length.
// Note: In many cases, we need to use a safe prime for cryptographic security to hold. 
// In this case, we should use GenerateSafePrime.
BigInt GenPrime(int prime_length) {
    BigInt result;
    CRYPTO_CHECK(1 == BN_generate_prime_ex(result.bn_ptr, prime_length, 0, nullptr, nullptr, nullptr));
    return std::move(result);
}


BigInt HashToBigInt(const std::string& input){
    unsigned char digest[SHA256_DIGEST_LENGTH]; 
    memset(digest, 0, SHA256_DIGEST_LENGTH); 
    
    SHA256(reinterpret_cast<const unsigned char*>(input.data()), input.size(), digest);

    BigInt result; 
    BN_bin2bn(digest, SHA256_DIGEST_LENGTH, result.bn_ptr);
    return std::move(result); 
}


void BigInt::Print() const
{
    char *bn_str; 
    bn_str = BN_bn2hex(this->bn_ptr);
    // switch(mode){
    //     case 16: bn_str = BN_bn2hex(this->bn_ptr); break; 
    //     case 10: bn_str = BN_bn2dec(this->bn_ptr); break;
    // }
    std::cout << bn_str << std::endl;
    OPENSSL_free(bn_str);
}

void BigInt::Print(std::string note) const
{
    std::cout << note << " = "; 
    this->Print();
}


/* compute the jth bit of a big integer i (count from little endian to big endian) */
int BigInt::GetTheNthBit(size_t j) const
{
    BigInt a = *this;  
    a = a >> j;
    a = a.GetLastNBits(1);  

    int result; 
    if (a.IsOne()) return 1; 
    else return 0; 
}


/* save bigint object binary form */  
void BigInt::Serialize(std::ofstream &fout)
{
    unsigned char buffer[BN_BYTE_LEN];
    BN_bn2binpad(this->bn_ptr, buffer, BN_BYTE_LEN);
    fout.write(reinterpret_cast<char *>(buffer), BN_BYTE_LEN);   // write to outfile
}

/* recover bigint object from binary file */
void BigInt::Deserialize(std::ifstream &fin)
{
    char buffer[BN_BYTE_LEN];
    fin.read(buffer, BN_BYTE_LEN);
    BN_bin2bn(reinterpret_cast<unsigned char *>(buffer), BN_BYTE_LEN, this->bn_ptr);
}

std::ofstream &operator<<(std::ofstream &fout, const BigInt& a)
{ 
    unsigned char buffer[BN_BYTE_LEN];
    BN_bn2binpad(a.bn_ptr, buffer, BN_BYTE_LEN);
    fout.write(reinterpret_cast<char *>(buffer), BN_BYTE_LEN);   // write to output file
    return fout;            
}
 
std::ifstream &operator>>(std::ifstream &fin, BigInt &a)
{ 
    char buffer[BN_BYTE_LEN];
    fin.read(buffer, BN_BYTE_LEN);
    BN_bin2bn(reinterpret_cast<unsigned char *>(buffer), BN_BYTE_LEN, a.bn_ptr); // red from input file
    return fin;            
}

// print a Big Number vector
void Print_BigIntVector(std::vector<BigInt> &vec_a, std::string note)
{
    for (auto i = 0; i < vec_a.size(); i++)
    {
        std::cout << note <<"[" << i << "]="; 
        vec_a[i].Print(); 
    }
}

/* a[i] = (a[i]+b[i]) mod order */
void BigIntVector_ModAdd(std::vector<BigInt> &result, std::vector<BigInt> &vec_a, std::vector<BigInt> &vec_b)
{
    if (vec_a.size() != vec_b.size()) {
        std::cerr << "vector size does not match!" << std::endl;
        exit(EXIT_FAILURE); 
    }
    //#pragma omp parallel for
    for (auto i = 0; i < vec_a.size(); i++) 
    {
        result[i] = (vec_a[i] + vec_b[i]) % order;  
        //BN_mod_add(result[i].bn_ptr, vec_a[i].bn_ptr, vec_b[i].bn_ptr, order, bn_ctx); 
    }
}

/* a[i] = (a[i]-b[i]) mod order */ 
void BigIntVector_ModSub(std::vector<BigInt> &result, std::vector<BigInt> &vec_a, std::vector<BigInt> &vec_b)
{
    if (vec_a.size() != vec_b.size()) {
        std::cout << "vector size does not match!" << std::endl;
        exit(EXIT_FAILURE); 
    }
    //#pragma omp parallel for
    for (auto i = 0; i < vec_a.size(); i++) {
        result[i] = (vec_a[i] - vec_b[i]) % order;
        //BN_mod_sub(result[i].bn_ptr, vec_a[i].bn_ptr, vec_b[i].bn_ptr, order, nullptr); 
    } 
}

/* c[i] = a[i]*b[i] mod order */ 
void BigIntVector_ModProduct(std::vector<BigInt> &result, std::vector<BigInt> &vec_a, std::vector<BigInt> &vec_b)
{
    if (vec_a.size() != vec_b.size()) {
        std::cerr << "vector size does not match!" << std::endl;
        exit(EXIT_FAILURE); 
    }
    //#pragma omp parallel for
    for (auto i = 0; i < vec_a.size(); i++) {
        result[i] = (vec_a[i] * vec_b[i]) % order; // product = (vec_a[i]*vec_b[i]) mod order
        //BN_mod_mul(result[i].bn_ptr, vec_a[i].bn_ptr, vec_b[i].bn_ptr, order, nullptr);
    }
}

/* compute the inverse of a[i] */ 
void BigIntVector_ModInverse(std::vector<BigInt> &result, std::vector<BigInt> &vec_a)
{
    //#pragma omp parallel for
    for (auto i = 0; i < vec_a.size(); i++) {
        result[i] = vec_a[i].ModInverse(order); 
        //BN_mod_inverse(result[i].bn_ptr, vec_a[i].bn_ptr, order, nullptr); 
    }
}


/* result[i] = c * a[i] */  
void BigIntVector_ModScalar(std::vector<BigInt> &result, std::vector<BigInt> &vec_a, BigInt &c)
{
    //#pragma omp parallel for
    for (auto i = 0; i < vec_a.size(); i++) {
        result[i] = (vec_a[i] * c) % order;
        //BN_mod_mul(result[i].bn_ptr, vec_a[i].bn_ptr, c.bn_ptr, order, nullptr);
    } 
}

/* result[i] = c * a[i] */  
void BigIntVector_Scalar(std::vector<BigInt> &result, std::vector<BigInt> &vec_a, BigInt &c)
{
    //#pragma omp parallel for
    for (auto i = 0; i < vec_a.size(); i++) {
        result[i] = vec_a[i] * c;
        //BN_mul(result[i].bn_ptr, vec_a[i].bn_ptr, c.bn_ptr, bn_ctx);
    } 
}

/* result[i] = -result[i] */  
void BigIntVector_ModNegate(std::vector<BigInt> &result, BigInt &modulus)
{
    for (auto i = 0; i < result.size(); i++) {
        result[i] = result[i].ModNegate(modulus);
    } 
}


/* sum_i^n a[i]*b[i] */
BigInt BigIntVector_ModInnerProduct(std::vector<BigInt> &vec_a, std::vector<BigInt> &vec_b)
{
    BigInt result(bn_0); 

    if (vec_a.size() != vec_b.size()){
        std::cerr << "vector size does not match!" << std::endl;
        exit(EXIT_FAILURE); 
    }   
    for (auto i = 0; i < vec_a.size(); i++) {
        result += vec_a[i] * vec_b[i]; // product = (vec_a[i]*vec_b[i]) mod order
    }
    result = result % BigInt(order);
    return std::move(result); 
}


/* generate a vector of random EC points */  
void GenRandomBigIntVectorLessThan(std::vector<BigInt> &vec_a, const BigInt &order)
{
    for(auto i = 0; i < vec_a.size(); i++){ 
        vec_a[i] = GenRandomBigIntLessThan(order); 
    }
}

#endif  // KUNLUN_CRYPTO_BIGINT_HPP_