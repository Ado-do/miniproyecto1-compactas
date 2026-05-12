#include "FMIndex.hpp"
#include "bench-lib/benchmark.hpp"
#include "bench-lib/benchmark_manager.hpp"
#include "utils.hpp"

#include <vector>
#include <cstdint>

using namespace std;

const vector<size_t> sizes{32, 64, 128, 256, 512};
const size_t n_patterns = 100;
const string csv_name = "patterns_res";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <text_file>\n";
        return 1;
    }
    string text_path = argv[1];
    cout << "* Experimentos sobre el texto " << quoted(text_path) << " utilizando patrones de diferente largo!\n";

    vector<uint8_t> text = read_file_to_vector(text_path);

    FMIndex<OccMyWT>        fm1(text);
    FMIndex<OccBruteForce>  fm2(text);
    FMIndex<OccBalancedWT>  fm3(text);
    FMIndex<OccHuffmanWT>   fm4(text);

    for (size_t i = 0; i < sizes.size(); i++) {
        size_t sz = sizes[i];
        vector<vector<uint8_t>> patterns = generate_patterns(text, sz, n_patterns);

        BenchLib::BenchmarkManager manager;
        for (size_t j = 0; j < n_patterns; j++) {
            BenchLib::Benchmark bench;

            bench.add("OccMyWT", [&fm1](vector<uint8_t> &pattern) {
                return fm1.count(pattern);
            }, patterns[i]).set_label("occ1").set_input_size(sz).set_size_in_megabytes(fm1.size_mb());

            if (sz == sizes[0]) {
                bench.add("OccBruteForce", [&fm2](vector<uint8_t> &pattern) {
                    return fm2.count(pattern);
                }, patterns[i]).set_label("occ2").set_input_size(sz).set_size_in_megabytes(fm2.size_mb());
            }

            bench.add("OccBalancedWT", [&fm3](vector<uint8_t> &pattern) {
                return fm3.count(pattern);
            }, patterns[i]).set_label("occ3").set_input_size(sz).set_size_in_megabytes(fm3.size_mb());

            bench.add("OccHuffmanWT", [&fm4](vector<uint8_t> &pattern) {
                return fm4.count(pattern);
            }, patterns[i]).set_label("occ4").set_input_size(sz).set_size_in_megabytes(fm4.size_mb());

            bench.run();

            size_t r1 = bench.get_result<size_t>(0);
            size_t r3 = bench.get_result<size_t>(1);
            size_t r4 = bench.get_result<size_t>(2);

            assert(r1 == r3);
            assert(r3 == r4);

            manager.add_results(bench.get_tasks());
        }
        manager.average_by_task();

        if (i == 0) manager.write_csv(csv_name);
        else        manager.append_csv(csv_name);
    }

    cout << "* Resultados guardados en : results/" << csv_name << '\n';

    return 0;
}
