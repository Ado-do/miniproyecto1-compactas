#include "FMIndex.hpp"
#include "OccBruteForce.hpp"
#include "bench-lib/benchmark.hpp"
#include "utils.hpp"

#include <vector>

using namespace std;

const vector<size_t> sizes{32, 64, 128, 256, 512};
const size_t n_patterns = 50;
const string csv_name = "patterns_res";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <text_file>\n";
        return 1;
    }

    string text_path = argv[1];
    vector<uint8_t> text = read_file_to_vector(text_path);

    FMIndex<OccMyWT>        fm1(text);
    FMIndex<OccBruteForce>  fm2(text);
    FMIndex<OccBalancedWT>  fm3(text);
    FMIndex<OccHuffmanWT>   fm4(text);

    for (size_t i = 0; i < sizes.size(); i++) {
        size_t sz = sizes[i];
        vector<vector<uint8_t>> patterns = generate_patterns(text, sz, n_patterns);

        BenchLib::Benchmark bench;
        bench.add("OccMyWT", [&fm1, &patterns](){
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm1.count(pattern);
            return cnt = 0;
        }).set_input_size(sz).set_label("occ1");
        bench.add("OccBruteForce", [&fm2, &patterns](){
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm2.count(pattern);
            return cnt;
        }).set_input_size(sz).set_label("occ2");
        bench.add("OccBalancedWT", [&fm3, &patterns](){
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm3.count(pattern);
            return cnt;
        }).set_input_size(sz).set_label("occ3");
        bench.add("OccHuffmanWT", [&fm4, &patterns](){
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm4.count(pattern);
            return cnt;
        }).set_input_size(sz).set_label("occ4");
        bench.run();

        size_t r1 = bench.get_result<size_t>(0);
        size_t r2 = bench.get_result<size_t>(1);
        size_t r3 = bench.get_result<size_t>(2);
        size_t r4 = bench.get_result<size_t>(3);
        assert(r1 == r2);
        assert(r2 == r3);
        assert(r3 == r4);

        if (i == 0) bench.write_csv(csv_name);
        else        bench.append_csv(csv_name);
    }

    cout << "Experimentos sobre el texto " << quoted(text_path) << " utilizando patrones de diferente largo terminado!\n";
    cout << "Resultados guardados en : results/" << csv_name << '\n';

    return 0;
}
