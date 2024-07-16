#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include <numeric>
#include <execution>
#include <numbers>

#include "../vcl/vectorclass.h"
#include "../vcl/vectormath_trig.h"; //VCL2


template<typename V>
[[nodiscard]] inline V sine(V x) noexcept
{
    const static V B(4. / std::numbers::pi);
    const static V C(-4. / (std::numbers::pi*std::numbers::pi));
    V y = mul_add(B, x, C * x * abs(x)); // y = B * x + C * x * abs(x); maximum error is 0.00109 within [-pi;pi]
    const static V P(0.225);
    const V yayy = mul_sub(y, abs(y), y);
    y = mul_add(P, yayy, y); // Q * y + P * y * abs(y); maximum error is 0.056 within [-pi;pi]
    return y;
}

template<typename V>
[[nodiscard]] inline V cosine(V x) noexcept
{
    const static V TP = 1. / (2. * std::numbers::pi;
    x *= TP;
    x -= V(.25) + floor(x + V(.25));
    x *= V(16.) * (abs(x) - V(.5));
    x += V(.225) * x * (abs(x) - V(1.));
    return x;
}

// Function to run the benchmark
template<typename V>
double benchmarkTrig(int num_iterations) {
    using namespace std;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 3.14159265f);  // Values between 0 and π
    vector<V> samples;
    samples.reserve(num_iterations);
    for (int i = 0; i < num_iterations; ++i) {
        array<float, V::size()> rnd = {};
        for (auto j = 0; j < V::size(); j++) {
            rnd[j] = dis(gen);
        }
        V vrnd;
        vrnd.load(&rnd[0]);
        samples.emplace_back(vrnd);
    }
    // Benchmark the trig operation on the samples
    auto t1 = Time::add();
    for (int i = 0; i < num_iterations; ++i) {
        samples[i] = sine<V>(samples[i]); //there will be an extra spent on operator= storing the data
    }
    auto t2 = Time::add();
    // Display the time taken for the benchsmark
    Time::show(t1, t2);
    auto duration = t2 - t1;
    cout << chrono::duration_cast<chrono::nanoseconds>(duration).count() / num_iterations << "\t nanoseconds per " << V::size() << " elements\n";
    auto result = reduce(samples.begin(), samples.end(), V(0.0f)); //prevent optimizing-out by compiler by using a result
    return horizontal_add(result);
}

// Function to verify accuracy of sine and cosine functions
template<typename V>
void verifyTrigonometricFunctions() {
    constexpr int num_samples = 1000; // Number of samples to test
    constexpr double range_start = -std::numbers::pi;
    constexpr double range_end = std::numbers::pi;
    constexpr double step_size = (range_end - range_start) / num_samples;

    double max_sin_error = 0.0;
    double max_cos_error = 0.0;

    for (int i = 0; i <= num_samples; ++i) {
        double angle = range_start + i * step_size;
        V angle_vec = V(angle);  // Create a SIMD vector with the current angle

        // Compute sine and cosine using our functions
        V std_sin_val = sine(angle_vec);
        V std_cos_val = cosine(angle_vec);

        // Convert back to scalar (assuming it's Vec8f or Vec16f)
        double sin_scalar = std_sin_val[0];
        double cos_scalar = std_cos_val[0];

        // Compare with standard library results
        double std_sin_scalar = std::sin(angle);
        double std_cos_scalar = std::cos(angle);

        // Calculate errors
        double sin_error = std::abs(sin_scalar - std_sin_scalar);
        double cos_error = std::abs(cos_scalar - std_cos_scalar);

        // Update maximum errors
        max_sin_error = std::max(max_sin_error, sin_error);
        max_cos_error = std::max(max_cos_error, cos_error);
    }

    std::cout << "Maximum absolute error for sine function: " << max_sin_error << std::endl;
    std::cout << "Maximum absolute error for cosine function: " << max_cos_error << std::endl;
}
