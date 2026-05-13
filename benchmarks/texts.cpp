#include "FMIndex.hpp"
#include "bench-lib/benchmark.hpp"
#include "bench-lib/benchmark_manager.hpp"
#include "utils.hpp"

#include <exception>
#include <vector>

using namespace std;

const size_t m = 50;
const size_t n_patterns = 100;
const string csv_name = "text_results";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <text_file1> <text_file2> ...\n";
        return 1;
    }
    size_t n_texts = argc - 1;
    vector<string> text_names(n_texts);
    for (size_t i = 0; i < n_texts; i++)
        text_names[i] = sdsl::util::basename(argv[i+1]);

    cout << "* Experimentos sobre los textos: "; for (auto &name : text_names) cout << name << " ";
    cout << endl;

    vector<vector<uint8_t>> texts(n_texts);
    for (size_t i = 0; i < n_texts; i++) {
        try {
            texts[i] = read_file_to_vector(argv[i+1]);
        } catch (const exception &e) {
            cerr <<  e.what() << endl;
        }
    }

    for (size_t i = 0; i < n_texts; i++) {
        vector<vector<uint8_t>> patterns = generate_patterns(texts[i], m, n_patterns);

        FMIndex<OccMyWT>        fm1(texts[i]);
        // FMIndex<OccBruteForce>  fm2(texts[i]);
        FMIndex<OccBalancedWT>  fm3(texts[i]);
        FMIndex<OccHuffmanWT>   fm4(texts[i]);

        BenchLib::BenchmarkManager manager;
        for (size_t j = 0; j < n_patterns; j++) {
            BenchLib::Benchmark bench;
            string name = text_names[i];

            bench.add(name + "+OccMyWT", [&fm1](vector<uint8_t> &pattern) {
                return fm1.count(pattern);
            }, patterns[j]).set_label(name.substr(3) + "+occ1").set_input_size(m).set_size_in_megabytes(fm1.size_mb());

            // bench.add("OccBruteForce", [&fm2](vector<uint8_t> &pattern) {
            //     return fm2.count(pattern);
            // }, patterns[j]).set_label(name.substr(3) + "+occ2").set_input_size(m).set_size_in_megabytes(fm2.size_mb());

            bench.add(name + "+OccBalancedWT", [&fm3](vector<uint8_t> &pattern) {
                return fm3.count(pattern);
            }, patterns[j]).set_label(name.substr(3) + "+occ3").set_input_size(m).set_size_in_megabytes(fm3.size_mb());

            bench.add(name + "+OccHuffmanWT", [&fm4](vector<uint8_t> &pattern) {
                return fm4.count(pattern);
            }, patterns[j]).set_label(name.substr(3) + "+occ4").set_input_size(m).set_size_in_megabytes(fm4.size_mb());

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

    cout << "* Resultados guardados en: results/" << csv_name << ".csv" << endl;

    return 0;
}
