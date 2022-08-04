#include "lib.h"
#include <bitset>

const int8_t X = -1;

bitset<36> arrayToBitset(const array<int8_t, 36> &arr) {
    bitset<36> ret{};
    for (int i = 0; i < 36; ++i) {
        ret[35 - i] = arr[i];
    }
    return ret;
}

vector<bitset<36>> getAddresses(const array<int8_t, 36> &indexes) {
    auto firstX = find(indexes.begin(), indexes.end(), X);
    if (firstX == indexes.end()) {
        return {arrayToBitset(indexes)};
    }
    size_t firstXIndex = firstX - indexes.begin();
    array<int8_t, 36> index1{indexes};
    index1[firstXIndex] = 0;
    vector<bitset<36>> ret = getAddresses(index1);

    array<int8_t, 36> index2{indexes};
    index2[firstXIndex] = 1;
    vector<bitset<36>> ret2 = getAddresses(index2);

    copy(ret2.begin(), ret2.end(), back_inserter(ret));
    return ret;
}

array<int8_t, 36> maskIndex(size_t index, const array<int8_t, 36> &mask) {
    array<int8_t, 36> ret{};

    for (int i = 0; i < 36; i++) {
        bool currentBitValue = (index >> (35 - i)) & 1;
        if (mask[i] == X) {
            ret[i] = X;
        } else if (mask[i] == 1) {
            ret[i] = 1;
        } else {
            ret[i] = currentBitValue;
        }
    }
//    cout << "indexes = " << ret << '\n';
    return ret;
}


void changeMemory(map<u_long, bitset<36>> &memory, size_t index, size_t val, const array<int8_t, 36> &mask) {
    auto addresses = getAddresses(maskIndex(index, mask));
//    cout << addresses << endl;
    for (auto &&addr : addresses) {
        memory[addr.to_ulong()] = val;
    }
}

size_t maskValue(size_t val, const int8_t mask[36]) {
    size_t ret = 0;
    for (int i = 0; i < 36; i++) {
        size_t currentBitMask = (1ull << (35 - i));
        ret &= ~currentBitMask;
        if (mask[i] == X) {
            ret |= (val & currentBitMask);
        } else {
            ret |= (unsigned long long) mask[i] << (35 - i);
        }
    }
    return ret;
}

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();


    vector<string> lines = readFileLines("../input14.txt");

    map<u_long, bitset<36>> memory{};
    array<int8_t, 36> currentMask{}; // 0, 1, or X

    for (auto &&line : lines) {

        if (line.substr(0, 7) == "mask = ") {
            string mask = line.substr(7, 36);
            //       cout << mask << endl;
            for (int i = 0; i < 36; i++) {
                currentMask[i] = mask[i] == 'X' ? X : mask[i] == '0' ? 0 : 1;
            }

        } else {
            size_t index = stoull(line.substr(4, line.find(']') - 4));
            size_t val = stoull(line.substr(line.find(" = ") + 3));
            //       cout << index << " : " << val << endl;

//      memory[index] = maskValue(val, currentMask);
            changeMemory(memory, index, val, currentMask);
        }
    }
//  cout << memory << endl;
    cout << accumulate(memory.begin(), memory.end(), 0ull,
                       [](size_t a, const pair<int, bitset<36>> &b) {
                           return a + b.second.to_ullong();
                       })
         << endl;



    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "Temps d'exécution = " << float(time) / 1000.0f << "ms" << endl;


    return 0;
}

