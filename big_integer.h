#ifndef BIGINT_MY_BIG_INTEGER_H
#define BIGINT_MY_BIG_INTEGER_H

#include <vector>
#include <cstdint>
#include <iosfwd>
#include "big_integer.h"
#include <my_vector.h>

class big_integer {
public:
    big_integer();
    big_integer(int a);
    big_integer(uint32_t a);
    explicit big_integer(std::string const &s);

    big_integer &operator=(big_integer const &other);
    big_integer &operator+=(big_integer const &rhs);
    big_integer &operator-=(big_integer const &rhs);
    big_integer &operator*=(big_integer const &rhs);
    big_integer &operator/=(big_integer const &rhs);
    big_integer &operator/=(int b);

    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &rhs);
    big_integer &operator|=(big_integer const &rhs);
    big_integer &operator^=(big_integer const &rhs);

    big_integer &operator<<=(int rhs);
    big_integer &operator>>=(int rhs);

    big_integer& operator++();
    const big_integer operator++(int);
    big_integer& operator--();
    const big_integer operator--(int);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    friend bool operator==(big_integer const &a, big_integer const &b);
    friend bool operator!=(big_integer const &a, big_integer const &b);
    friend bool operator<(big_integer const &a, big_integer const &b);
    friend bool operator>(big_integer const &a, big_integer const &b);
    friend bool operator<=(big_integer const &a, big_integer const &b);
    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend std::string to_string(big_integer const &a);
private:
    bool sign;
    my_vector v;

    size_t length() const;
    void normalize();
    friend uint32_t get(big_integer &a, size_t pos);
    friend big_integer addition(big_integer a);
    friend big_integer smallDiv(big_integer a, int b);
    friend void smallMul(big_integer &a, uint32_t b);
    friend int compare(big_integer const& a, big_integer const& b);
    friend int compare_absolute_value(big_integer const& a, big_integer const& b);
    big_integer& add(big_integer &first, big_integer const &second, bool flag);
    template <class FunctorT>
    big_integer& bitOperation(big_integer &first, big_integer const &second, FunctorT functorT);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);
big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);


#endif //BIGINT_MY_BIG_INTEGER_H
