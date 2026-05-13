#pragma once

#include "utils.hpp"
#include "OccMyWT.hpp" // NOLINT
#include "OccBruteForce.hpp"
#include "OccSDSL.hpp"

#include <cstdint>
#include <vector>

template <typename OccStruct>
class FMIndex {
private:
    static const size_t SIGMA = 1 << 8; // 256
    std::vector<size_t> C;
    OccStruct occ_struct;

    void calculate_C(const std::vector<uint8_t> &text) {
        std::vector<size_t> freq(SIGMA + 1, 0);
        for (uint8_t c : text)
            freq[c]++;
        for (size_t i = 0; i < SIGMA; i++)
            C[i + 1] = C[i] + freq[i];
    }

public:
    FMIndex(const std::vector<uint8_t> &text) : C(SIGMA + 1, 0), occ_struct(get_bwt(text)) {
        calculate_C(text);
    }
    size_t count(std::vector<uint8_t> &pattern) {
        if (pattern.empty()) return 0;
        size_t m = pattern.size();
        size_t i = m - 1;
        uint8_t v = pattern[i];
        size_t sp = C[v] + 1, ep = C[v + 1];
        while ((sp <= ep) && (i > 0)) {
            v = pattern[i - 1];
            sp = C[v] + occ_struct.occ(v, sp - 1) + 1;
            ep = C[v] + occ_struct.occ(v, ep);
            i--;
        }
        if (ep > sp) return 0;
        return (ep - sp + 1);
    }
    double size_mb() const {
        size_t fm_mb = (sizeof(FMIndex) + C.capacity() * sizeof(size_t)) / (1024.0 * 1024.0);
        return fm_mb + occ_struct.size_mb();
    }
};
