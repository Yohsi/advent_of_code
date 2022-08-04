#include "lib.h"

const int NB_ITER = 10000000;
const int NB_CUPS = 1000000;

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();

    vector<string> lines = readFileLines("../input23.txt");

    string line = lines[0];

    list<int> cups{};
    vector<_List_iterator<int>> indexes{NB_CUPS};

    int maxVal = 0;
    int minVal = INT32_MAX;

    auto currentIt = cups.begin();

    for (char c : line) {
        int v = c - '0';
        cups.push_back(v);
        indexes[v - 1] = ++currentIt;
        minVal = min(minVal, v);
        maxVal = max(maxVal, v);
    }

    for (int i = maxVal + 1; i <= NB_CUPS; ++i) {
        cups.push_back(i);
        indexes[i - 1] = ++currentIt;
    }
    maxVal = NB_CUPS;

    auto current = cups.begin();

    list<int> pickedUp{};

    for (int i = 0; i < NB_ITER; ++i) {

//        if (i % 1000000 == 0) {
//        cout << "--- iteration " << i << " ---\n";
//        }

        auto right = current;
        ++right;

        auto pickedUpBegin = pickedUp.begin();
        for (int j = 0; j < 3; ++j) {
            if (right == cups.end()) right = cups.begin();
            pickedUp.splice(pickedUpBegin, cups, right++);
//            pickedUp.push_back(*right);
//            cups.erase(right++);
        }

//        cout << "picking up " << pickedUp << '\n';
//        cout << "list = " << cups << '\n';

        int currentVal = *current;
        _List_iterator<int> destCup;
        int destVal = currentVal;
        do {
            --destVal;
            if (destVal < minVal) {
                destVal = maxVal;
            }
        } while (find(pickedUp.begin(), pickedUp.end(), destVal) != pickedUp.end());

//        cout << "searching " << destVal << " in " << cups << '\n';
        destCup = indexes[destVal - 1];

//        cout << "dest cup = " << *destCup << '\n';

        auto destCupNext = destCup;
        ++destCupNext;
        if (destCupNext == cups.end()) {
            destCupNext = cups.begin();
        }

        for (auto it = pickedUp.begin(); it != pickedUp.end(); ++it) {
            indexes[(*it) - 1] = it;
        }

        cups.splice(destCupNext, pickedUp);

//        cout << "list after insertion : " << cups << '\n';
//        cout << '\n';

        ++current;
        if (current == cups.end()) current = cups.begin();
    }

    auto one = indexes[0];
    auto two = one;
    two++;
    auto three = two;
    three++;
    cout << *two << " * " << *three << " = " << u_long(*two) * u_long(*three) << '\n';

//    for (int c:cups) {
//        cout << c;
//    }
//    cout << '\n';

    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
