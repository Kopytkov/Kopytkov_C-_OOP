#include <iostream>
#include <cmath>
class Complex 
{
    double real_;
    double imag_;
 
public:
    constexpr Complex(const double& real = 0, const double& imag = 0) : real_(real), imag_(imag) { }
    
    ~Complex(){
    }

    void read() 
    {
        std::cout << "a: ";
        std::cin >> real_;
        std::cout << "b: ";
        std::cin >> imag_;
    }
 
    void display() 
    {
        std::cout << real_ << (imag_ < 0 ? " " : " + ") << imag_ << 'i';
    }
 
    friend Complex operator+(const Complex &c1, const Complex &c2) 
    {
        return Complex(c1.real_ + c2.real_, c1.imag_ + c2.imag_);
    }
 
    friend Complex operator-(const Complex &c1, const Complex &c2) 
    {
        return Complex(c1.real_ - c2.real_, c1.imag_ - c2.imag_);
    }
 
    friend Complex operator*(const Complex &c1, const Complex &c2) 
    {
        return Complex(c1.real_ * c2.real_ - c1.imag_ * c2.imag_, c1.real_ * c2.imag_ + c1.imag_ * c2.real_);
    }
 
    friend Complex operator/(const Complex &c1, const Complex &c2) 
    {
        return Complex(c1.real_ * c2.real_ + c1.imag_ * c2.imag_, c1.real_ * c2.imag_ - c1.imag_ * c2.real_);
    }
 
    friend bool operator==(const Complex &c1, const Complex &c2) 
    {
        return c1.real_ == c2.real_ && c1.imag_ == c2.imag_;
    }
 
    friend Complex operator-(const Complex &c1) 
    {
        return Complex(-c1.real_, -c1.imag_);
    }
    Complex& operator=(const Complex& c) 
    {
        if (this == &c) {
            return *this;
        }
        this->real_ = c.real_;
        this->imag_ = c.imag_;
        return *this; 
    }
    float len() const{
        return sqrt(real_ * real_ + imag_ * imag_);
    };
};
