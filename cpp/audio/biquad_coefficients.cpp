// SPDX-License-Identifier: MIT

#include <atomic>

// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 7:48 - CAS Exchange Loop

struct BiquadCoefficients { float b0, b1, b2, a1, a2 };
std::unique_ptr<BiquadCoefficients> storage{ std::make_unique<BiquadCoefficients>(); }
std::atomic<BiquadCoefficients*> biquad_coeffs{ storage.get() };

BiquadCoefficients calculateLowPassCoefficients(float freq);

void processBiquadCoefficients(const float *src, float *dst, size_t n, BiquadCoefficients* coeffs) {
    static float level1, level2;

    for (size_t i = 0; i < n; ++i) {
        auto input = src[i];
        auto output = (input * coeffs->b0) + level1;
        dst[i] = output;

        level1 = (input * coeffs->b1) - (output * coeffs->a1) + level2;
        level2 = (input * coeffs->b2) - (output * coeffs->a2);
    }
}

void audioThread(const float *src, float *dst, size_t n) {
    auto coeffs = biquad_coeffs.exchange(nullptr);
    processBiquadCoefficients(src, dst, n, coeffs);
    coeffs->b0 *= 2.f;  // mutations will be lost
    biquad_coeffs = coeffs;
}

void updateFrequencyParameter(float value) {
    auto local_storage = std::make_unique<BiquadCoefficients>(calculateLowPassCoefficients(value));

    while (auto expected = storage.get())
        if (biquad_coeffs.compare_exchange_strong(expected, local_storage.get())
            break;

    storage = std::move(local_storage); // old storage deleted here
}


// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 3:27 - CAS Exchange Loop with data race

struct BiquadCoefficients { float b0, b1, b2, a1, a2 };
BiquadCoefficients* biquad_coeffs;
std::atomic<bool> busy{ false };

BiquadCoefficients calculateLowPassCoefficients(float freq);

void processBiquadCoefficients(const float *src, float *dst, size_t n, BiquadCoefficients* coeffs) {
    static float level1, level2;

    for (size_t i = 0; i < n; ++i) {
        auto input = src[i];
        auto output = (input * coeffs->b0) + level1;
        dst[i] = output;

        level1 = (input * coeffs->b1) - (output * coeffs->a1) + level2;
        level2 = (input * coeffs->b2) - (output * coeffs->a2);
    }
}

void audioThread(const float *src, float *dst, size_t n) {
    busy = true;
    processBiquadCoefficients(src, dst, n, biquad_coeffs);
    busy = false;
}

void updateFrequencyParameter(float value) {
    auto coeffs = new BiquadCoefficients(calculateLowPassCoefficients(value));
    while (busy.load()) { /* spin */ }
    // data race here as isInAudioThread could be changed here
    std::swap(coeffs, biquad_coeffs);
    delete coeffs;
}

// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 2:52 - CAS Exchange Loop with Memory Leak

struct BiquadCoefficients { float b0, b1, b2, a1, a2 };
std::unique_ptr<BiquadCoefficients> storage{ std::make_unique<BiquadCoefficients>(); }
std::atomic<BiquadCoefficients*> biquad_coeffs;

BiquadCoefficients calculateLowPassCoefficients(float freq);

void processBiquadCoefficients(const float *src, float *dst, size_t n, BiquadCoefficients* coeffs) {
    static float level1, level2;

    for (size_t i = 0; i < n; ++i) {
        auto input = src[i];
        auto output = (input * coeffs->b0) + level1;
        dst[i] = output;

        level1 = (input * coeffs->b1) - (output * coeffs->a1) + level2;
        level2 = (input * coeffs->b2) - (output * coeffs->a2);
    }
}

void audioThread(const float *src, float *dst, size_t n) {
    processBiquadCoefficients(src, dst, n, biquad_coeffs.load());
}

void updateFrequencyParameter(float value) {
    biquad_coeffs = new BiquadCoefficients(calculateLowPassCoefficients(value));  // memory leak here
}


// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 2:15 fails is_always_lock_free

struct BiquadCoefficients { float b0, b1, b2, a1, a2 };
std::atomic<BiquadCoefficients> biquad_coeffs;
// static_assert(std::atomic<BiquadCoefficients>::is_always_lock_free);  // fails

BiquadCoefficients calculateLowPassCoefficients(float freq);

void audioThread(const float *src, float *dst, size_t n) {
    static float level1, level2;
    auto coeffs = biquad_coeffs.load();

    for (size_t i = 0; i < n; ++i) {
        auto input = src[i];
        auto output = (input * coeffs.b0) + level1;
        dst[i] = output;

        level1 = (input * coeffs.b1) - (output * coeffs.a1) + level2;
        level2 = (input * coeffs.b2) - (output * coeffs.a2);
    }
}

void updateFrequencyParameter(float value) {
    biquad_coeffs = calculateLowPassCoefficients(value);
}
