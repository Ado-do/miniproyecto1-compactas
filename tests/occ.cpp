#include "OccMyWT.hpp"
#include "OccBruteForce.hpp"
#include "OccSDSL.hpp"
#include "bwt.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    // Recibir archivo via CLI
    assert(argc == 2);
    const string file_path = argv[1];
    ifstream file(file_path, ios::binary | ios::ate);

    // Comprobar que se abrió
    assert(file.is_open());
    size_t n = file.tellg();
    file.seekg(0, ios::beg);

    vector<uint8_t> text(n), bwt(n);
    file.read((char *)text.data(), n);
    get_bwt(text, bwt);

    unsigned char c = text[0];
    size_t cnt_text = count(text.begin(), text.end(), c);
    size_t cnt_bwt = count(bwt.begin(), bwt.end(), c);
    printf("* '%c': cnt_text = %zu, cnt_bwt = %zu\n", c, cnt_text, cnt_bwt);
    assert(cnt_text == cnt_bwt);

    string s(bwt.begin(), bwt.end());

    OccBruteForce o0(bwt);
    OccMyWT o1(bwt);
    OccBalancedWT o2(bwt);
    OccHuffmanWT o3(bwt);

    uint8_t v = c;
    uint64_t k = n;

    uint64_t r0 = o0.occ(v, k);
    uint64_t r1 = o1.occ(v, k);
    uint64_t r2 = o2.occ(v, k);
    uint64_t r3 = o3.occ(v, k);
    printf("* occ(%c, %zu): r0 = %zu, r1 = %zu, r2 = %zu, r3 = %zu\n", v, k, r0, r1, r2, r3);
    assert(r0 == r1);
    assert(r1 == r2);
    assert(r2 == r3);

    return 0;
}
