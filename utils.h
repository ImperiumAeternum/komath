#pragma once
#include <chrono>
#include <future>
#include <iostream>

class Time {
public:
    using tp = std::chrono::steady_clock::time_point;
    Time(const Time&) = delete; //singleton
    Time& operator=(const Time&) = delete;

    inline static void show(tp t1, tp t2) {
        auto duration = t2 - t1;
        if (duration < std::chrono::milliseconds(1))
            std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() << "\t nanoseconds\n";
        else if (duration < std::chrono::seconds(1))
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << "\t microseconds\n";
        else if (duration < std::chrono::minutes(1))
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "\t milliseconds\n";
        else
            std::cout << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << "\t seconds\n";
    }
    inline static auto count(tp t1, tp t2) {
        auto duration = t2 - t1;
        if (duration < std::chrono::milliseconds(1))
            return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        else if (duration < std::chrono::seconds(1))
            return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        else if (duration < std::chrono::minutes(1))
            return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        else
            return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }
    inline static tp add() {
        tp p = std::chrono::steady_clock::now();
        return p;
    }
};

class Offload {
    template<typename F, typename... Ts>
    inline static auto async(F&& f, Ts&&... params) // C++14
    {
        return std::async(std::launch::async,
            std::forward<F>(f),
            std::forward<Ts>(params)...);
    }
};


class Vec4me {
    std::array<double, 4> arr;
public:
//Custom class to play with constexpr constructors
    constexpr Vec4me() : arr{ 0.0, 0.0, 0.0, 0.0 } {}
    constexpr Vec4me(double a, double b, double c, double d) : arr{ a, b, c, d } {}
    constexpr Vec4me(double value) : arr{ value, value, value, value } {}

    constexpr Vec4me(const Vec4me& other) = default;
    constexpr Vec4me(Vec4me&& other) noexcept = default;
    ~Vec4me() = default;

    constexpr Vec4me& operator=(const Vec4me& other) = default;
    constexpr Vec4me& operator=(Vec4me&& other) noexcept = default;

    constexpr double& operator[](std::size_t index) { return arr[index]; }
    constexpr const double& operator[](std::size_t index) const { return arr[index]; }

    void print() const {
        std::cout << "(" << arr[0] << ", " << arr[1] << ", " << arr[2] << ", " << arr[3] << ")" << std::endl;
    }
};
