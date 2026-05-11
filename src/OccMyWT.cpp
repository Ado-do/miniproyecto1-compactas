#include "OccMyWT.hpp"

#include "sdsl/construct_sa.hpp"
#include "sdsl/suffix_arrays.hpp"
#include <cmath>
#include <cstdint>

using namespace std;
using namespace sdsl;

// Definiciones de BitVector

BitVector::BitVector(uint32_t _size, uint8_t _default_value) {
    size = _size;
    uint64_t default_value = _default_value == 0 ? ZERO : ONE;
    uint32_t buckets_needed = size >> 6; // size / (2^i)
    uint64_t module_value = size & 63;   // size % 64;
    if (module_value != ZERO) buckets_needed++;
    bitarray.assign(buckets_needed, default_value);
}

uint64_t BitVector::access(uint32_t index) {
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    return (bitarray[bucket] >> inverted_bit_index) & 1;
}

void BitVector::flip(uint32_t index) {
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    bitarray[bucket] ^= (1ULL << inverted_bit_index);
}

void BitVector::set(uint32_t index) {
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    bitarray[bucket] |= (1ULL << inverted_bit_index);
}

void BitVector::add_rank() {
    // superblock with size 512
    uint64_t needed_sb = ((size + 511) >> 9) + 1ULL;
    super_blocks.reserve(needed_sb);
    // block with size 64
    uint64_t needed_b = ((size + 63) >> 6) + 1ULL;
    blocks.reserve(needed_b);

    uint32_t ps_sb = 0;
    uint16_t ps_b = 0;
    uint64_t c_bits_sb = 0;
    uint64_t c_bits_b = 0;
    super_blocks.push_back(0ULL);
    blocks.push_back(0ULL);
    for (uint64_t const &decimal : bitarray) {
        c_bits_sb += 64;
        c_bits_b += 64;
        uint32_t popcount_decimal = __builtin_popcountll(decimal);
        ps_sb += popcount_decimal;
        ps_b += popcount_decimal;
        if (!(c_bits_sb & 511)) {
            super_blocks.push_back(ps_sb);
            ps_b = 0ULL;
        }
        if (!(c_bits_b & 63)) {
            blocks.push_back(ps_b);
            c_bits_b = 0;
        }
    }
    super_blocks.push_back(ps_sb);
    blocks.push_back(0ULL);
}

uint32_t BitVector::rank1(uint32_t index) {
    uint64_t super_block = index >> 9;
    uint64_t block = index >> 6;
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    uint64_t ibi_p1 = inverted_bit_index + 1ULL;
    uint32_t popcount = ibi_p1 == 64 ? 0ULL : __builtin_popcountll(bitarray[bucket] >> ibi_p1);
    return super_blocks[super_block] + blocks[block] + popcount;
}

uint32_t BitVector::rank0(uint32_t index) { return index - rank1(index); }

// Definiciones de WaveletTree

WaveletTree::WaveletTree(vector<uint32_t> &sequence) {
    sigma = 0;
    for (auto const &e : sequence) {
        sigma = max(e, sigma);
    }
    size = (uint32_t)sequence.size();
    height = floor(log2(sigma));
    root = build(sequence, 0, size, height);
}

WaveletTree::Node *WaveletTree::build(vector<uint32_t> const &sequence, uint32_t l, uint32_t r, int h) {
    if ((uint32_t)sequence.size() == 0) return nullptr;
    if (h == -1) return nullptr;

    vector<uint32_t> left_side, right_side;
    WaveletTree::Node *node = new WaveletTree::Node(r - l);
    for (uint32_t i = 0; i < r; ++i) {
        uint32_t ele = sequence[i];
        if (ele & (1 << h)) {
            right_side.push_back(ele);
            node->representation.set(i);
        } else {
            left_side.push_back(ele);
        }
    }

    node->representation.add_rank();
    node->childs[0] = build(left_side, 0, (uint32_t)left_side.size(), h - 1);
    node->childs[1] = build(right_side, 0, (uint32_t)right_side.size(), h - 1);
    return node;
}

uint32_t WaveletTree::access(uint32_t index) {
    int h = height;
    uint64_t ele = 0;
    Node *aux = root;
    while (aux != nullptr) {
        if (aux->representation.access(index) == 0) {
            index = aux->representation.rank0(index);
            aux = aux->childs[0];
        } else {
            index = aux->representation.rank1(index);
            aux = aux->childs[1];
            ele |= (1 << h);
        }
        h--;
    }
    return ele;
}

uint32_t WaveletTree::rank(uint32_t symbol, uint32_t index) {
    int h = height;
    Node *aux = root;
    while (aux != nullptr) {
        if (symbol & (1 << h)) {
            index = aux->representation.rank1(index);
            aux = aux->childs[1];
        } else {
            index = aux->representation.rank0(index);
            aux = aux->childs[0];
        }
        h--;
    }
    return index;
}

// Definiciones OccMyWT

OccMyWT::OccMyWT(vector<uint8_t> &seq) {
    vector<uint32_t> casted(seq.begin(), seq.end());
    wt = WaveletTree(casted);
}

uint64_t OccMyWT::occ(uint8_t v, uint64_t k) { return wt.rank(v, k); }
