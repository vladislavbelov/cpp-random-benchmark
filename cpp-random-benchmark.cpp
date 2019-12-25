#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <random>

// Provides the simple random engine based on the rand() function.
// Warning: rand() might have non-uniform distribution in some implementations.
class RandEngine {
public:
    using result_type = int;
    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return RAND_MAX; }

    static_assert(std::numeric_limits<result_type>::max() >= RAND_MAX,
                  "result_type should be enough to fit RAND_MAX");

    RandEngine(unsigned seed) {
        std::srand(seed);
    }

    result_type operator()() {
        return std::rand();
    }
};

template<typename RandomEngine>
class Tester {
public:
    Tester(std::string&& name) : name_(name), random_engine_(42) {}

    void Run() {
        constexpr size_t NUMBER_OF_REPEATS = 100000000;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t repeat = 0; repeat < NUMBER_OF_REPEATS; ++repeat) {
            random_engine_();
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);

        constexpr double NANOSECONDS_PER_SECOND = 1e9;
        std::cout << name_ << ": ";
        std::cout << std::fixed << std::setprecision(3) << time_span.count() / NUMBER_OF_REPEATS * NANOSECONDS_PER_SECOND;
        std::cout << " ns." << std::endl;
    }

private:
    const std::string name_;
    RandomEngine random_engine_;
};

int main() {
#define TEST_CASE(RANDOM_ENGINE) Tester<RANDOM_ENGINE>(#RANDOM_ENGINE).Run()

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
    TEST_CASE(RandEngine);
#undef TEST_CASE
    return EXIT_SUCCESS;
}
