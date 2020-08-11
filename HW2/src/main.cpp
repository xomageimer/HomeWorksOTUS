#include "pch.h"
#include "allocator.h"
#include "List.h"

struct hard {

    friend std::ostream& operator<< (std::ostream& os, hard & h);

    int i;
    double d;

    hard(int i, double d) : i(i), d(d) { //std::cout << __PRETTY_FUNCTION__ << std::endl;
    };

 //   hard(const hard &)
 //   = delete;
//    { std::cout << __PRETTY_FUNCTION__ << std::endl; };

    hard(hard &&) noexcept
//    = delete;
    { //std::cout << __PRETTY_FUNCTION__ << std::endl;
    };

    ~hard() { //std::cout << __PRETTY_FUNCTION__ << std::endl;
    };
};

std::ostream& operator<< (std::ostream& os, hard & h){
    os << h.i << " " << h.d;
    return os;
}

template <long long val>
struct factorial{
    static const long long value = val * factorial<val - 1>::value;
};

template <>
struct factorial<1>{
    static const long long value = 1;
};

struct factorial_table{
    long long table[10];

    factorial_table (){
        table[0] = factorial<1>::value;
        table[1] = factorial<2>::value;
        table[2] = factorial<3>::value;
        table[3] = factorial<4>::value;
        table[4] = factorial<5>::value;
        table[5] = factorial<6>::value;
        table[6] = factorial<7>::value;
        table[7] = factorial<8>::value;
        table[8] = factorial<9>::value;
        table[9] = factorial<10>::value;
    };

    long long operator()(size_t s){
        return table[s];
    }

};


int main() {
    using namespace std;

    factorial_table tab{};

    {
        auto m = std::map < int, int > {};
        for (size_t i = 0; i < 10; ++i) {
            m.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(tab(i)));
        }

        for (auto &i : m)
            cout << i.first << " " << i.second << endl;


        cout << "__________________" << endl;
        auto m2 = std::map <int, int, std::less <int>, PoolAllocator<std::pair<const int, int>, 10>>{};
        for (size_t i = 0; i < 10; ++i) {
            m2.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(tab(i)));
        }

        for (auto &i : m2)
            cout << i.first << " " << i.second << endl;

        cout << "__________________" << endl;
        auto l = List<int>();
        for (int i = 0; i < 10; ++i) {
            l.emplace_front(tab(i));
        }

        for (auto &i : l) {
            cout << i << endl;
        }

        cout << "__________________" << endl;
        auto l2 = List<int, PoolAllocator<int, 10>>();
        for (int i = 0; i < 10; ++i) {
            l2.emplace_front(tab(i));
        }

        for (auto &i : l2) {
            cout << i << endl;
        }
    }

    std::cerr << err.str();

}
