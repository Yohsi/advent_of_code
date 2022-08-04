#include "lib.h"
#include <iterator>

size_t getNextSpokenNumber(const vector<int> &spokenNumbers, const map<int, size_t> &indexes) {
    auto it = indexes.find(spokenNumbers.back());
    if (it == indexes.end()) {
        return 0;
    } else {
        return spokenNumbers.size() - 1 - it->second;
    }
}

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();

    vector<string> lines = readFileLines("../input15.txt");
    vector<string> split = splitLine(lines[0], ",");

    vector<int> spokenNumbers{};
    map<int, size_t> spokenNumbersIndexes{};

    for (int i = 0; i < split.size(); i++) {
        int nb = stoi(split[i]);
        spokenNumbers.push_back(nb);
        if (i != 0) {
            spokenNumbersIndexes[spokenNumbers[spokenNumbers.size() - 2]] = spokenNumbers.size() - 2;
        }
    }

    for (int i = 0; i < 30000000 - split.size(); ++i) {
        int next = getNextSpokenNumber(spokenNumbers, spokenNumbersIndexes);
        spokenNumbers.push_back(next);
        spokenNumbersIndexes[spokenNumbers[spokenNumbers.size() - 2]] = spokenNumbers.size() - 2;

        if (i % 1000000 == 0)
            cout << i << '\n';
    }
    cout << spokenNumbers.back() << '\n';


    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
