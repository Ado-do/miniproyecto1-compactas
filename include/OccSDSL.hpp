#pragma once

#include "sdsl/io.hpp"
#include "sdsl/rrr_vector.hpp"
#include <sdsl/wavelet_trees.hpp>

#include <sdsl/construct.hpp>

class OccBalancedWT {
private:
    sdsl::wt_blcd<sdsl::bit_vector> wt;

public:
    OccBalancedWT() = default;
    OccBalancedWT(std::vector<uint8_t> &seq) { 
        std::string s(seq.begin(), seq.end());
        construct_im(wt, s, 1);
    }
    uint64_t occ(uint8_t v, uint64_t k) { return wt.rank(k, v); }
    double size_mb() const {
        size_t bytes = sizeof(OccBalancedWT) + sdsl::size_in_mega_bytes(wt);
        return bytes / (1024.0 * 1024.0);
    }
};

class OccHuffmanWT {
private:
    sdsl::wt_huff<sdsl::rrr_vector<>> wt;

public:
    OccHuffmanWT() = default;
    OccHuffmanWT(const std::vector<uint8_t> &seq) {
        std::string s(seq.begin(), seq.end());
        construct_im(wt, s, 1);
    }
    uint64_t occ(uint8_t v, uint64_t k) { return wt.rank(k, v); }
    double size_mb() const {
        size_t bytes = sizeof(OccHuffmanWT) + sdsl::size_in_mega_bytes(wt);
        return bytes / (1024.0 * 1024.0);
    }
};
