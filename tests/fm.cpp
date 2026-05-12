#include "FMIndex.hpp"

#include <cassert>

using namespace std;

int main(int argc, char *argv[]) {
    // Recibir archivo via CLI
    assert(argc == 2);
    const string file_path = argv[1];

    FMIndex<OccMyWT> fm(file_path);
    string s = "foo";
    vector<uint8_t> pattern(s.begin(), s.end());
    assert(fm.count(pattern) == 2);

    s = "Foo";
    pattern.assign(s.begin(), s.end());
    assert(fm.count(pattern) == 1);

    s = "oo";
    pattern.assign(s.begin(), s.end());
    assert(fm.count(pattern) == 3);

    return 0;
}
