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
    double dynamic_size_in_bytes() const {
        double bytes = 0;
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
    void destroy_tree(Node *n) {
        if (n == nullptr) return;
        destroy_tree(n->childs[0]);
        destroy_tree(n->childs[1]);
        delete n;
    }
    double node_size_bytes(Node *n) const {
        if (n == nullptr) return 0;
        double bytes = sizeof(Node) + n->representation.dynamic_size_in_bytes();
        bytes += node_size_bytes(n->childs[0]);
        bytes += node_size_bytes(n->childs[1]);
        return bytes;
    }

public:
    WaveletTree() = delete;
    WaveletTree(std::vector<uint32_t> sequence);
    ~WaveletTree() { destroy_tree(root); }

    uint32_t access(uint32_t index);
    uint32_t rank(uint32_t symbol, uint32_t index);
    double size_bytes() const { return node_size_bytes(root); }
};

class OccMyWT {
private:
    WaveletTree wt;

    std::vector<uint32_t> cast_vector(std::vector<uint8_t> og) {
        return std::vector<uint32_t>(og.begin(), og.end());
    }
public:
    OccMyWT() = delete;
    OccMyWT(const std::vector<uint8_t> &seq) : wt(cast_vector(seq)) {}
    uint64_t occ(uint8_t v, uint64_t k) { return wt.rank(v, k); }
    double size_mb() const {
        double bytes = sizeof(OccMyWT) + wt.size_bytes();
        return bytes / (1024.0 * 1024.0);
    }
};
