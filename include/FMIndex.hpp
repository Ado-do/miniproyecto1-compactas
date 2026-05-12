#pragma once

#include "bwt.hpp"
#include "OccMyWT.hpp" // NOLINT
#include "OccBruteForce.hpp"
#include "OccSDSL.hpp"

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

template <typename OccStruct>
class FMIndex {
private:
    static const size_t SIGMA = 1 << 8; // 256
    OccStruct occ_struct;
    std::vector<size_t> C;
    std::vector<uint8_t> bwt;

    void read_file(const std::string &path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        size_t n = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<uint8_t> buf(n);
        file.read((char *)buf.data(), n);
        get_bwt(buf, bwt);
    }
    void calculate_C() {
        std::vector<size_t> freq(SIGMA + 1, 0);
        for (uint8_t c : bwt)
            freq[c]++;

        for (size_t i = 0; i < SIGMA; i++)
            C[i + 1] = C[i] + freq[i];
    }

public:
    FMIndex(const std::string &text_path) : C(SIGMA + 1, 0) {
        read_file(text_path);
        calculate_C();

        occ_struct = OccStruct(bwt);
    }
    size_t count(std::vector<uint8_t> pattern) {
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
};
