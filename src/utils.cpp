#include "utils.hpp"

#include "sdsl/construct_sa.hpp"
#include <random>

using namespace std;
using namespace sdsl;

vector<uint8_t> read_file_to_vector(const string &filepath) {
    ifstream file(filepath, ios::binary | ios::ate);
    if (!file.is_open()) throw runtime_error("Error: No se pudo abrir el archivo " + filepath);

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char *>(buffer.data()), size);
    return buffer;
}

vector<uint8_t> get_bwt(const vector<uint8_t> &text) {
    size_t n = text.size();
    vector<uint8_t> bwt(n);

    int_vector<> sa(n);
    // int_vector<> sa(1, 0, bits::hi(n) + 1);
    // sa.reserve(n);
    sdsl::algorithm::calculate_sa((const unsigned char *)text.data(), n, sa);

    bwt.resize(n);
    for (size_t i = 0; i < n; ++i)
        bwt[i] = text[sa[i] + ((sa[i] > 0) ? -1 : n - 1)];

    return bwt;
}

vector<vector<uint8_t>> generate_patterns(const vector<uint8_t> &text, size_t m, size_t cnt) {
    size_t n = text.size();
    if (n < m) return {};

    vector<vector<uint8_t>> patterns;

    // static random_device rd;
    static mt19937 gen(6'7);
    uniform_int_distribution<size_t> dist(0, n - m);

    for (size_t i = 0; i < cnt; i++) {
        int idx = dist(gen);
        vector<uint8_t> pattern(&text[idx], &text[idx] + m);
        patterns.push_back(pattern);
    }

    return patterns;
}
