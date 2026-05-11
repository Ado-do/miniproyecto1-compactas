#pragma once

#include <cstdint>
#include <vector>

class OccBruteForce {
    std::vector<uint8_t> &seq;
public:
    OccBruteForce(std::vector<uint8_t> &seq) : seq(seq) {}

    uint64_t occ(uint8_t v, uint64_t k) {
        k = std::min(k, seq.size());
        uint64_t cnt = 0;
        for (uint64_t i = 0; i < k; i++)
            if (seq[i] == v) cnt++;
        return cnt;
    }
};
