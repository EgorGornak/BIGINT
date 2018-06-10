#include <iostream>
#include <cstring>
#include "my_vector.h"
#include "big_integer.h"

void my_vector::pop_back() {
    if (isSmall) {
        small_size--;
    } else {
        copy();
        data.v->pop_back();
    }
}

size_t my_vector::size() const {
    if (isSmall) {
        return small_size;
    } else {
        return data.v->size();
    }
}

const uint32_t my_vector::back() const {
    if (isSmall) {
        return data.small[small_size - 1];
    } else {
        return data.v->back();
    }
}

void my_vector::push_back(uint32_t x){
    if (isSmall && small_size < SMALL_COUNT) {
        data.small[small_size] = x;
        small_size++;
        return;
    } else if (isSmall) {
        isSmall = false;
        turn_big();
    }
    data.v->push_back(x);
}

uint32_t &my_vector::operator[](size_t pos) {
    if (isSmall) {
        return data.small[pos];
    } else {
        copy();
        return (*data.v)[pos];
    }
}

const uint32_t &my_vector::operator[](size_t pos) const {
    if (isSmall) {
        return data.small[pos];
    } else {
        return (*data.v)[pos];
    }
}

void my_vector::resize(size_t len, uint32_t val) {
    if (isSmall && len > SMALL_COUNT) {
        isSmall = false;
        turn_big();
    } else if (isSmall) {
        for (size_t i = small_size; i < len; i++) {
            data.small[i] = val;
        }
        small_size = len;
        return;
    }
    copy();
    data.v->resize(len, val);
}

void my_vector::assign(size_t len, uint32_t val) {
    if (isSmall && len > SMALL_COUNT) {
        isSmall = false;
        turn_big();
    } if (isSmall) {
        for (size_t i = 0; i < len; i++) {
            data.small[i] = val;
        }
        small_size = len;
        return;
    }
    copy();
    data.v->assign(len, val);
}

my_vector::my_vector() : isSmall(true), small_size(0) {}

my_vector::my_vector(size_t len, uint32_t val) {
    if (len > SMALL_COUNT) {
        isSmall = false;
        data.v = std::make_shared<std::vector<uint32_t>>(len, val);
    } else {
        for (unsigned int i = 0; i < len; i++) {
            data.small[i] = val;
        }
        small_size = len;
    }
}

my_vector::my_vector(uint32_t len): my_vector(len, 0) {}


void my_vector::reserve(size_t len) {
    if (isSmall && len > SMALL_COUNT) {
        isSmall = false;
        turn_big();
    } else if (isSmall) {
        return;
    }
    data.v->reserve(len);
}


void my_vector::swap(my_vector &a) {
    std::swap(isSmall, a.isSmall);
    std::swap(small_size, a.small_size);

    char temp[sizeof(Data)];
    memcpy(temp, &data, sizeof(Data));
    memcpy(&data, &a.data, sizeof(Data));
    memcpy(&a.data , temp, sizeof(Data));
}

my_vector &my_vector::operator=(const my_vector &other) {
    my_vector tmp(other);
    swap(tmp);
    return *this;
}

my_vector::my_vector(my_vector const &other) : isSmall(other.isSmall), small_size(other.small_size) {
    if (isSmall) {
        for (size_t i = 0; i < SMALL_COUNT; i++) {
            data.small[i] = other.data.small[i];
        }
    } else {
        data.v = other.data.v;
    }
}

void my_vector::copy() {
    if (data.v.use_count() != 1) {
        data.v = std::make_shared<std::vector<uint32_t> > (*data.v);
    }
}

void my_vector::turn_big() {
    std::vector <uint32_t> temp(SMALL_COUNT);
    for (size_t i = 0; i < SMALL_COUNT; i++) {
        temp[i] = data.small[i];
    }
    data.v = std::make_shared<std::vector<uint32_t> >(small_size);
    for (size_t i = 0; i < small_size; i++) {
        (*data.v)[i] = temp[i];
    }
}

