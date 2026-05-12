#pragma once

#include <cstdint>
#include <vector>

uint64_t const ZERO = 0x0000000000000000ULL;
uint64_t const ONE = 0xFFFFFFFFFFFFFFFFULL;

class BitVector {
private:
    std::vector<uint64_t> bitarray;
    std::vector<uint16_t> blocks;
    std::vector<uint32_t> super_blocks;
    // std::vector<uint32_t> S1;
    // std::vector<uint32_t> S2;
    // uint8_t s;
    uint32_t size;

public:
    BitVector() = default;
    BitVector(uint32_t _size, uint8_t _default_value);
    uint64_t access(uint32_t index);
    void flip(uint32_t index);
    void set(uint32_t index);
    void add_rank();
    uint32_t rank1(uint32_t index);
    uint32_t rank0(uint32_t index);
    // void add_select1_1();
    // uint32_t select1_1(uint32_t j);
    // void add_select1_2(uint8_t _s = 7);
    // uint32_t select1_2(uint32_t j);
    size_t dynamic_size_in_bytes() const {
        size_t bytes = 0;
        bytes += bitarray.capacity() * sizeof(uint64_t);
        bytes += blocks.capacity() * sizeof(uint16_t);
        bytes += super_blocks.capacity() * sizeof(uint32_t);
        return bytes;
    }
};

class WaveletTree {
private:
    struct Node {
        BitVector representation;
        Node *childs[2];

        Node() = default;
        Node(uint32_t len) {
            representation = BitVector(len + 1, 0);
            childs[0] = nullptr, childs[1] = nullptr;
        }
    };

    Node *root;
    uint32_t sigma;
    uint32_t size;
    uint32_t height;

    Node *build(std::vector<uint32_t> const &sequence, uint32_t l, uint32_t r, int h);
    size_t node_size_bytes(Node *n) const {
        if (n == nullptr) return 0;
        size_t bytes = sizeof(Node) + n->representation.dynamic_size_in_bytes();
        bytes += node_size_bytes(n->childs[0]);
        bytes += node_size_bytes(n->childs[1]);
        return bytes;
    }
public:
    WaveletTree() = default;
    WaveletTree(std::vector<uint32_t> &sequence);

    uint32_t access(uint32_t index);
    uint32_t rank(uint32_t symbol, uint32_t index);
    size_t size_bytes() const {
        return node_size_bytes(root);
    }
};

class OccMyWT {
private:
    WaveletTree wt;

public:
    OccMyWT() = default;
    OccMyWT(const std::vector<uint8_t> &seq);
    uint64_t occ(uint8_t v, uint64_t k);
    double size_mb() const {
        size_t bytes = sizeof(OccMyWT) + wt.size_bytes();
        return bytes / (1024.0 * 1024.0);
    }
};
