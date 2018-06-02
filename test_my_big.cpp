#include <iostream>
#include <memory>
#include "big_integer.h"

using namespace std;

int main() {
    std::shared_ptr<int> x(new int(1));
    std::shared_ptr<int> y(new int(2));
    cout << *x << '\n';
    cout << *y << "\n\n";
    x = y;
    cout << *x << '\n';
    cout << *y << "\n\n";
    y = std::shared_ptr<int>(new int(5));
    cout << *x << '\n';
    cout << *y << "\n\n";

    return 0;
}