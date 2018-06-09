#ifndef BIGINT_MY_VECTOR_H
#define BIGINT_MY_VECTOR_H

#include <vector>
#include <cstdint>
#include <memory>

class my_vector {
public:
    my_vector();
    my_vector(uint32_t len);
    my_vector(size_t len, uint32_t val);
    my_vector(size_t len, int val);
    my_vector(my_vector const& other);
    void pop_back();
    void push_back(uint32_t a);
    size_t size() const;
    const uint32_t back() const;
    uint32_t &operator[](size_t pos);
    const uint32_t &operator[](size_t pos) const;
    void resize(size_t len, uint32_t val);
    void assign(size_t len, uint32_t val);
    my_vector &operator=(my_vector const &other);
    void reserve(size_t len);

private:
    bool isSmall = true;
    static const size_t SMALL_COUNT = 2;
    size_t small_size;

    union Data {
        uint32_t small[SMALL_COUNT] ;
        std::shared_ptr<std::vector<uint32_t> > v;
        Data() :  v(nullptr) {}
        ~Data() {
          v = nullptr;
        }
    } data;
    void copy();
    void turn_big();
};


#endif //BIGINT_MY_VECTOR_H
