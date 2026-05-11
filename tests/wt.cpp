#include "OccMyWT.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

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

    vector<unsigned char> tmp_buf(n);
    file.read((char*)tmp_buf.data(), n);

    vector<uint32_t> buf(tmp_buf.begin(), tmp_buf.end());
    WaveletTree wt(buf);
    for (size_t i = 0; i < n; i++) {
        // Comprobar correctitud de construcción del WaveletTree
        assert(tmp_buf[i] == wt.access(i));
    }

    size_t real_cnt = count(tmp_buf.begin(), tmp_buf.end(), 'T');
    size_t wt_cnt = wt.rank('T', n);
    printf("real: %zu, wt: %zu\n", real_cnt, wt_cnt);

    // Comprobar correctitud del rank
    assert(real_cnt == wt_cnt);

    return 0;
}
