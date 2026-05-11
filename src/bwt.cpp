#include "bwt.hpp"

#include "sdsl/construct_sa.hpp"

using namespace std;
using namespace sdsl;

void get_bwt(vector<uint8_t> &text, vector<uint8_t> &bwt) {
    size_t n = text.size();

    int_vector<> sa(n);
    // int_vector<> sa(1, 0, bits::hi(n) + 1);
    // sa.reserve(n);
    sdsl::algorithm::calculate_sa((const unsigned char *)text.data(), n, sa);

    bwt.reserve(n);
    for (size_t i = 0; i < n; ++i)
        bwt[i] = text[sa[i] + ((sa[i] > 0) ? -1 : n - 1)];
}
