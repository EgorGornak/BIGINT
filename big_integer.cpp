#include "big_integer.h"
#include <iostream>
#include <algorithm>
#include <climits>

void big_integer::normalize() {
    while (v.size() > 1 && v.back() == 0) {
        v.pop_back();
    }
    if (v.size() == 1 && v[0] == 0) {
        sign = false;
    }
}

big_integer::big_integer() : sign(false), v(1, 0) {}

big_integer::big_integer(uint32_t a) : sign(false), v(1, a) {}

big_integer::big_integer(int a) {
    if (a == INT_MIN) {
        v.push_back(1u << 31u);
        sign = true;
        return;
    }
    v.push_back(static_cast<uint32_t>(a > 0 ? a : -a));
    sign = (a < 0);
}

big_integer::big_integer(std::string const &s) {
    sign = false;
    size_t i = 0;
    if (s[0] == '-') {
        sign = true;
        i++;
    }
    big_integer ans;
    for (; i < s.size(); i++) {
        ans *= 10;
        ans += int(s[i] - '0');
    }
    v = ans.v;
    normalize();
}

size_t big_integer::length() const {
    return v.size();
}

void summOverflow(uint32_t &first, uint32_t const second, bool &carry) {
    uint64_t summ = (uint64_t) first + second + carry;
    first = (uint32_t) summ;
    carry = summ >> 32u;
}

void subOverflow(uint32_t &first, uint32_t const second, bool &carry) {
    uint64_t difference = (uint64_t) first - second - carry;
    first = (uint32_t) difference;
    carry = ((int64_t) difference < 0);
}

void mulOverflow(uint32_t &a, uint32_t const b, uint32_t const c, uint64_t &carry) {
    uint64_t ans = static_cast<uint64_t>(b) * static_cast<uint64_t>(c);
    ans += a + carry;
    a = static_cast<uint32_t>(ans);
    carry = ans >> 32u;
}

int compare(big_integer const &first, big_integer const &second) {
    if (!first.sign && second.sign) {
        return 1;
    }
    if (first.sign && !second.sign) {
        return -1;
    }
    if (first.v.size() > second.v.size()) {
        return (!first.sign ? 1 : -1);
    }
    if (first.v.size() < second.v.size()) {
        return (!first.sign ? -1 : 1);
    }
    for (size_t i = first.v.size(); i > 0;) {
        i--;
        if (first.v[i] > second.v[i]) {
            return (!first.sign ? 1 : -1);
        }
        if (first.v[i] < second.v[i]) {
            return (!first.sign ? -1 : 1);
        }
    }
    return 0;
}

int compare_absolute_value(big_integer const &first, big_integer const &second) {
    if (first.v.size() > second.v.size()) {
        return 1;
    }
    if (first.v.size() < second.v.size()) {
        return -1;
    }
    for (size_t i = first.v.size(); i > 0;) {
        i--;
        if (first.v[i] > second.v[i]) {
            return 1;
        }
        if (first.v[i] < second.v[i]) {
            return -1;
        }
    }
    return 0;
}

big_integer &big_integer::add(big_integer &first, big_integer const &second, bool flag) {
    bool secondSign = second.sign ^flag;
    size_t n = std::max(first.length(), second.length());
    size_t m = std::min(first.length(), second.length());
    bool bigger = false;
    if (first.length() > second.length()) {
        bigger = true;
    }
    if ((first.sign && secondSign) || (!first.sign && !secondSign)) {
        first.v.resize(n + 1, 0);
        bool carry = false;
        for (size_t i = 0; i < m; i++) {
            summOverflow(first.v[i], second.v[i], carry);
        }
        for (size_t i = m; i < n; i++) {
            bigger ? summOverflow(first.v[i], 0, carry) : summOverflow(first.v[i], second.v[i], carry);
        }
        summOverflow(first.v[n], 0, carry);
        first.normalize();
    } else {
        int cmp = compare_absolute_value(first, second);
        bool sign;
        big_integer ans;
        big_integer &b = first;
        if (cmp == 1) {
            ans = first;
            b = second;
            sign = ans.sign;
        } else {
            ans = second;
            b = first;
            sign = secondSign;
        }
        if (cmp == 0) {
            sign = false;
        }
        bool carry = false;
        for (size_t i = 0; i < n; i++) {
            if (i < m) {
                subOverflow(ans.v[i], b.v[i], carry);
            } else {
                subOverflow(ans.v[i], 0, carry);
            }
        }
        ans.normalize();
        ans.sign = sign;
        first = ans;
    }
    return first;
}

big_integer addition(big_integer a) {
    if (!a.sign) {
        return a;
    }
    for (size_t i = 0; i < a.length(); i++) {
        a.v[i] = ~a.v[i];
    }
    return a;
}

template<class FunctorT>
big_integer &big_integer::bitOperation(big_integer &first, big_integer const &second, FunctorT functorT) {
    first = addition(first);
    big_integer doubleSecond = addition(second);

    first.sign ? --first : first;
    doubleSecond.sign ? --doubleSecond : doubleSecond;

    if (first.length() > doubleSecond.length()) {
        std::swap(first, doubleSecond);
    }

    size_t n = std::max(first.length(), doubleSecond.length());
    size_t m = std::min(first.length(), doubleSecond.length());


    for (size_t i = 0; i < n; i++) {
        if (i >= m) {
            first.v.push_back(first.sign == 1 ? std::numeric_limits<uint32_t>::max() : 0);
        }
        first.v[i] = functorT(first.v[i], doubleSecond.v[i]);
        first.sign = functorT(first.sign, doubleSecond.sign);
    }

    if (first.sign) {
        first.v.push_back(std::numeric_limits<uint32_t>::max());
        first = addition(first + 1);
    }

    first.normalize();
    return first;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    return add(*this, rhs, false);
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return add(*this, rhs, true);
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    if (rhs.length() == 1) {
        smallMul(*this, rhs.v[0]);
        sign = sign ^ rhs.sign;
        return *this;
    }

    big_integer ans;
    size_t n = length();
    size_t m = rhs.length();
    ans.v.assign(n + m, 0);

    for (size_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < m; ++j) {
            mulOverflow(ans.v[i + j], v[i], rhs.v[j], carry);
        }
        ans.v[i + m] = static_cast<uint32_t>(carry);
    }
    ans.normalize();
    ans.sign = sign ^ rhs.sign;
    return *this = ans;
}

void smallMul(big_integer &a, uint32_t b) {
    size_t n = a.length();
    uint64_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint64_t ans = static_cast<uint64_t>(a.v[i]) * b + carry;
        a.v[i] = static_cast<uint32_t>(ans);
        carry = (ans >> 32u);
    }
    if (carry) {
        a.v.push_back(carry);
    }
    a.normalize();
}


big_integer &big_integer::operator/=(big_integer const &rhs) {
    if (rhs == 0) {
        throw std::runtime_error("division by zero");
    }

    if (compare_absolute_value(*this, rhs) == -1) {
        return *this = 0;
    }
    bool new_sign = sign ^rhs.sign;
    sign = false;
    big_integer second = rhs;
    second.sign = false;
    uint64_t base = (1ll << 32);
    uint32_t normalize_coeff = static_cast<uint32_t>(base / (static_cast<uint64_t >(second.v.back() + 1)));
    smallMul(*this, normalize_coeff);
    smallMul(second, normalize_coeff);

    int n = (int) second.length();
    int m = (int) length() - n;
    my_vector result;
    result.resize(m + 1, 0);

    big_integer tmp = second << (32 * m);

    if (*this >= tmp) {
        result[m] = 1;
        *this -= second << (32 * m);
    }

    for (int j = m - 1; j >= 0; j--) {
        uint64_t q_star = (uint64_t(get(*this, n + j)) * base + get(*this, n + j - 1)) / second.v[n - 1];
        if (q_star > base - 1) {
            q_star = base - 1;
        }
        result[j] = static_cast<uint32_t>(q_star);
        big_integer temp = (second << (32 * j));
        smallMul(temp, result[j]);
        *this -= temp;
        while (*this < 0) {
            --result[j];
            *this += (second << (32 * j));
        }
    }
    v = result;
    sign = new_sign;
    normalize();
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    *this = *this - (*this / rhs) * rhs;
    normalize();
    return *this;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return compare(a, b) == 0;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return compare(a, b) != 0;
}

bool operator<(big_integer const &a, big_integer const &b) {
    return compare(a, b) == -1;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return compare(a, b) == 1;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    int comp = compare(a, b);
    return (comp == 0 || comp == -1);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    int comp = compare(a, b);
    return (comp == 0 || comp == 1);
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer tmp = *this;
    if (tmp != 0) {
        tmp.sign ^= 1;
    }
    return tmp;
}

big_integer &big_integer::operator=(big_integer const &other) = default;

big_integer &big_integer::operator++() {
    (*this) += 1;
    return *this;
}

const big_integer big_integer::operator++(int) {
    big_integer tmp = *this;
    (*this) += 1;
    return tmp;
}

big_integer &big_integer::operator--() {
    (*this) -= 1;
    return *this;
}

const big_integer big_integer::operator--(int) {
    big_integer tmp = *this;
    (*this) -= 1;
    return tmp;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    return bitOperation(*this, rhs, std::bit_and<>());
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    return bitOperation(*this, rhs, std::bit_or<>());
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    return bitOperation(*this, rhs, std::bit_xor<>());
}

big_integer big_integer::operator~() const {
    return -(*this) - 1;
}

big_integer &big_integer::operator<<=(int rhs) {
    uint32_t zeroBlocks = rhs / 32;
    my_vector blocks(zeroBlocks);
    blocks.reserve(blocks.size() + v.size());
    for (size_t i = 0; i < v.size(); i++) {
        blocks.push_back(v[i]);
    }
    v = blocks;
    rhs %= 32;
    if (rhs) {
        uint32_t lastElement = v.back() >> (32 - rhs);
        v.push_back(lastElement);
        for (int i = (int) v.size() - 2; i > 0; i--) {
            v[i] = (v[i] << rhs) | (v[i - 1] >> (32 - rhs));
        }
        v[0] = v[0] << rhs;
    }

    normalize();
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    *this = addition(*this);
    sign ? --*this : *this;

    int cntErase = rhs / 32;
    for (int i = 0; i < cntErase; i++) {
        if (v.size() == 1) {
            *this = 0;
            return *this;
        } else {
            v.pop_back();
        }
    }

    rhs %= 32;
    if (rhs) {
        for (size_t i = 0; i < v.size() - 1; i++) {
            v[i] = (v[i] >> rhs) | (v[i + 1] << (32 - rhs));
        }
        v[v.size() - 1] >>= rhs;
    }

    if (sign) {
        v.push_back(std::numeric_limits<uint32_t>::max());
        *this = addition(*this + 1);
    }

    normalize();
    if (sign) {
        v[v.size() - 1] <<= rhs;
        v[v.size() - 1] >>= rhs;
    }

    return *this;
}

big_integer smallDiv(big_integer a, int b) {
    if (b == 0) {
        throw std::runtime_error("division by zero");
    }
    if (b < 0) {
        b *= -1;
        a.sign = !a.sign;
    }
    uint32_t carry = 0;
    uint64_t base = static_cast<uint64_t>(UINT32_MAX) + 1;
    for (int i = (int) a.v.size() - 1; i >= 0; i--) {
        uint64_t cur = static_cast<uint64_t>(a.v[i]) + static_cast<uint64_t>(carry) * base;
        a.v[i] = static_cast<uint32_t>(cur / b);
        carry = static_cast<uint32_t>(cur % b);
    }
    a.normalize();
    return a;
}

std::string to_string(big_integer const &a) {
    if (a == 0) {
        return "0";
    }
    big_integer x = a;
    std::string ans;
    while (x != 0) {
        big_integer tmp = x - smallDiv(x, 10) * 10;
        ans += '0' + tmp.v[0];
        x = smallDiv(x, 10);
    }
    if (a.sign) {
        ans += '-';
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

big_integer &big_integer::operator/=(int b) {
    return *this = smallDiv(*this, b);
}

uint32_t get(big_integer &a, size_t pos) {
    if (pos >= a.v.size()) {
        return 0;
    } else {
        return a.v[pos];
    }
}


big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}
