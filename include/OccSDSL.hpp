#pragma once

#include "sdsl/rrr_vector.hpp"
#include <sdsl/wavelet_trees.hpp>

#include <sdsl/construct.hpp>

class OccBalancedWT {
private:
    sdsl::wt_blcd<sdsl::bit_vector> wt;

public:
    OccBalancedWT(std::vector<uint8_t> &seq) { 
        std::string s(seq.begin(), seq.end());
        construct_im(wt, s, 1);
    }
    uint64_t occ(uint8_t v, uint64_t k) { return wt.rank(k, v); }
};

class OccHuffmanWT {
private:
    sdsl::wt_huff<sdsl::rrr_vector<>> wt;

public:
    OccHuffmanWT(std::vector<uint8_t> &seq) { 
        std::string s(seq.begin(), seq.end());
        construct_im(wt, s, 1);
    }
    uint64_t occ(uint8_t v, uint64_t k) { return wt.rank(k, v); }
};
