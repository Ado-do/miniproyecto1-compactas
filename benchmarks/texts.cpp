#include "FMIndex.hpp"
#include "OccBruteForce.hpp"
#include "bench-lib/benchmark.hpp"
#include "utils.hpp"

#include <exception>
#include <vector>

using namespace std;

const size_t m = 50;
const size_t n_patterns = 100;
const string csv_name = "text_res";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <text_file1> <text_file2> ...\n";
        return 1;
    }

    size_t n_texts = argc - 1;
    vector<string> text_names(n_texts);
    vector<vector<uint8_t>> texts(n_texts);
    for (size_t i = 0; i < n_texts; i++) {
        text_names[i] = argv[i + 1];
        try {
            texts[i] = read_file_to_vector(text_names[i]);
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    for (size_t i = 0; i < n_texts; i++) {
        vector<vector<uint8_t>> patterns = generate_patterns(texts[i], m, n_patterns);

        FMIndex<OccBruteForce>  fm2(texts[i]);
        FMIndex<OccMyWT>        fm1(texts[i]);
        FMIndex<OccBalancedWT>  fm3(texts[i]);
        FMIndex<OccHuffmanWT>   fm4(texts[i]);

        BenchLib::Benchmark bench;
        bench.add("OccMyWT", [&fm1, &patterns]() {
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm1.count(pattern);
            return cnt = 0;
        }).set_input_size(n_texts).set_label("occ1");

        bench.add("OccBruteForce", [&fm2, &patterns]() {
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm2.count(pattern);
            return cnt;
        }).set_input_size(n_patterns).set_label("occ2");

        bench.add("OccBalancedWT", [&fm3, &patterns]() {
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm3.count(pattern);
            return cnt;
        }) .set_input_size(n_patterns).set_label("occ3");

        bench.add("OccHuffmanWT", [&fm4, &patterns]() {
            size_t cnt = 0;
            for (auto &pattern : patterns)
                cnt += fm4.count(pattern);
            return cnt;
        })
        .set_input_size(n_patterns).set_label("occ4");
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

    cout << "Experimentos sobre los textos: "; for (auto &name : text_names) cout << name << " ";
    cout << "\nResultados guardados en: results/" << csv_name << '\n';

    return 0;
}
