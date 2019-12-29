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

class TesterBase
{
public:
    explicit TesterBase(const std::string& name) : name_(name) {}
    virtual ~TesterBase() {}

    void Run() {
        auto start = std::chrono::high_resolution_clock::now();
        const size_t number_of_repeats = DoRun();
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);

        // We don't need to measure empty tries.
        if (number_of_repeats == 0) {
            std::cerr << "Number of repeats is zero!" << std::endl;
            return;
        }

        constexpr double NANOSECONDS_PER_SECOND = 1e9;
        std::cout << name_ << ": ";
        std::cout << std::fixed << std::setprecision(3) << time_span.count() / number_of_repeats * NANOSECONDS_PER_SECOND;
        std::cout << " ns." << std::endl;
    }

protected:
    virtual size_t DoRun() { return 0u; }

    const std::string name_;
};

template<typename RandomEngine>
class Tester : public TesterBase {
public:
    Tester(const std::string& name) : TesterBase(name), random_engine_(42) {}
    ~Tester() override = default;

private:
    size_t DoRun() override {
        constexpr size_t NUMBER_OF_REPEATS = 100000000;
        for (size_t repeat = 0; repeat < NUMBER_OF_REPEATS; ++repeat) {
            random_engine_();
        }
        return NUMBER_OF_REPEATS;
    }

    RandomEngine random_engine_;
};

template<typename Distribution>
class DistributionTester : public TesterBase
{
public:
    DistributionTester(const std::string& name)
        : TesterBase(name), random_engine_(42), distribution_()
    {}
    ~DistributionTester() override = default;

private:
    size_t DoRun() override {
        constexpr size_t NUMBER_OF_REPEATS = 100000000;
        for (size_t repeat = 0; repeat < NUMBER_OF_REPEATS; ++repeat) {
            distribution_(random_engine_);
        }
        return NUMBER_OF_REPEATS;
    }

    std::mt19937 random_engine_;
    Distribution distribution_;
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

#define TEST_CASE(DISTRIBUTION) DistributionTester<DISTRIBUTION>(#DISTRIBUTION).Run()

    TEST_CASE(std::uniform_int_distribution<int>);
    TEST_CASE(std::uniform_real_distribution<float>);
#undef TEST_CASE

    return EXIT_SUCCESS;
}
