// SPDX-License-Identifier: MIT

#include <atomic>
#include <array>

// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 13:44 - Double Buffering

using FrequencySpectrum = std::array<float, 512>;

enum { SLOT_BIT = (1 << 0), NEWDATA_BIT = (1 << 1), BUSY_BIT = (1 << 2) };

std::array<FrequencySpectrum, 2> mostRecentSpectrum;
std::atomic<int> bits = {0};

void processAudio(const float *buffer, size_t n) {
    auto slot = bits.fetch_or(BUSY_BIT) & SLOT_BIT;
    mostRecentSpectrum[slot] = calculateSpectrum(buffer, n);
    bits.store(slot | NEWDATA_BIT); // clears busy bit
}

void updateSpectrumUIButtonClicked() {
    auto current = bits.load();
    if (current & NEWDATA_BIT) {
        int slot;
        do {
            current &= ~BUSY_BIT;
            slot = (current ^ SLOT_BIT) & SLOT_BIT;
        } while (!bits.compare_exchange_weak(current, slot));
        current = slot;
    }
    displaySpectrum(mostRecentSpectrum[(current & SLOT_BIT) ^ 1]);
}

// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 11:35 - Double Buffering - Non-atomic load and store apart

using FrequencySpectrum = std::array<float, 512>;

enum { SLOT_BIT = (1 << 0), NEWDATA_BIT = (1 << 1) };

std::array<FrequencySpectrum, 2> mostRecentSpectrum;
std::atomic<int> bits = {0};

void processAudio(const float *buffer, size_t n) {
    auto slot = bits.load() & SLOT_BIT;
    mostRecentSpectrum[slot] = calculateSpectrum(buffer, n);
    bits.store(slot | NEWDATA_BIT);
}

void updateSpectrumUIButtonClicked() {
    auto current = bits.load();
    if (current & NEWDATA_BIT) {
        current = (current & SLOT_BIT) ^ 1;
        bits.store(current);
    }
    displaySpectrum(mostRecentSpectrum[(current & SLOT_BIT) ^ 1]);
}

// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 11:35 - Double Buffering - May read stale data

using FrequencySpectrum = std::array<float, 512>;

std::array<FrequencySpectrum, 2> mostRecentSpectrum;
std::atomic<int> slot = {0};

void processAudio(const float *buffer, size_t n) {
    mostRecentSpectrum[slot.load()] = calculateSpectrum(buffer, n);
}

void updateSpectrumUIButtonClicked() {
    displaySpectrum(mostRecentSpectrum[slot.fetch_xor(1)]);
}
