#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <random>

template<typename Generator>
class Tester {
public:
    Tester(std::string&& name) : name_(name), generator_(42) {}

    void Run() {
        constexpr size_t NUMBER_OF_REPEATS = 100000000;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t repeat = 0; repeat < NUMBER_OF_REPEATS; ++repeat) {
            generator_();
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);

        std::cout << name_ << ": ";
        std::cout << std::fixed << std::setprecision(3) << time_span.count();
        std::cout << " s." << std::endl;
    }

private:
    const std::string name_;
    Generator generator_;
};

int main() {
#define TEST_CASE(GENERATOR) Tester<GENERATOR>(#GENERATOR).Run()

    TEST_CASE(std::default_random_engine);
    TEST_CASE(std::minstd_rand);
    TEST_CASE(std::minstd_rand0);
    TEST_CASE(std::mt19937);
    TEST_CASE(std::mt19937_64);
    TEST_CASE(std::ranlux24_base);
    TEST_CASE(std::ranlux48_base);
    TEST_CASE(std::ranlux24);
    TEST_CASE(std::ranlux48);
    TEST_CASE(std::knuth_b);
#undef TEST_CASE
    return EXIT_SUCCESS;
}
