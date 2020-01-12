#include <chrono>
#include <cstddef>
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
        const std::size_t number_of_repeats = DoRun();
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);

        // We don't need to measure empty tries.
        if (number_of_repeats == 0) {
            std::cerr << "Number of repeats is zero!" << std::endl;
            return;
        }

        constexpr double NANOSECONDS_PER_SECOND = 1e9;
        std::cout << name_ << ": " << std::fixed << std::setprecision(3)
                  << time_span.count() / number_of_repeats * NANOSECONDS_PER_SECOND
                  << " ns/call (total " << time_span.count() << " s)." << std::endl;
    }

protected:
    virtual std::size_t DoRun() { return 0u; }

    const std::string name_;
};

template<typename RandomEngine>
class Tester : public TesterBase {
public:
    Tester(const std::string& name) : TesterBase(name), random_engine_(42) {}
    ~Tester() override = default;

private:
    std::size_t DoRun() override {
        constexpr std::size_t NUMBER_OF_REPEATS = 100000000;
        for (std::size_t repeat = 0; repeat < NUMBER_OF_REPEATS; ++repeat) {
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
    template<typename... Args>
    DistributionTester(const std::string& name, Args... args)
        : TesterBase(name), random_engine_(42), distribution_(std::forward<Args>(args)...)
    {}
    ~DistributionTester() override = default;

private:
    std::size_t DoRun() override {
        constexpr std::size_t NUMBER_OF_REPEATS = 100000000;
        for (std::size_t repeat = 0; repeat < NUMBER_OF_REPEATS; ++repeat) {
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

#define TEST_CASE(DISTRIBUTION, ...) \
    DistributionTester<DISTRIBUTION>( \
        #DISTRIBUTION "(" #__VA_ARGS__ ")", __VA_ARGS__).Run()

    TEST_CASE(std::uniform_int_distribution<int>,
              std::numeric_limits<int>::min(),
              std::numeric_limits<int>::max());
    TEST_CASE(std::uniform_int_distribution<int>, 0, 1);
    TEST_CASE(std::uniform_int_distribution<int>, 0, 7);
    TEST_CASE(std::uniform_real_distribution<float>,
              std::numeric_limits<float>::min(),
              std::numeric_limits<float>::max());
    TEST_CASE(std::uniform_real_distribution<float>, 0.0f, 1.0f);

    TEST_CASE(std::normal_distribution<float>,
              /* mean = */ 0.0f, /* standard_deviation = */ 1.0f);
    TEST_CASE(std::lognormal_distribution<float>,
              /* mean = */ 0.0f, /* standard_deviation = */ 1.0f);
    TEST_CASE(std::geometric_distribution<int>, /* probability = */ 0.5f);
    TEST_CASE(std::gamma_distribution<float>,
              /* alpha = */ 1.0f, /* beta = */ 1.0f);
    TEST_CASE(std::poisson_distribution<int>, /* mean = */ 1.0);
    TEST_CASE(std::student_t_distribution<float>,
              /* degrees_of_freedom = */ 3.0f);
    TEST_CASE(std::bernoulli_distribution, /* probability = */ 0.5);
    TEST_CASE(std::binomial_distribution<int>,
              /* trials = */ 10, /* probability = */ 0.5);
    TEST_CASE(std::exponential_distribution<float>, /* lambda = */ 1.0f);
#undef TEST_CASE
    return EXIT_SUCCESS;
}
