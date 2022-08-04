#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <numeric>

const int MAX_LINE_SIZE = 100;
using namespace std;


int main() {
    cout << "processing..." << endl;

    ifstream file;
    file.open("../input6.txt");

    if (file.fail()) {
        cerr << "Impossible d'ouvrir le fichier" << endl;
        return 1;
    }

    vector<string> list;


    char fileLine[MAX_LINE_SIZE];
    while (file.getline(fileLine, MAX_LINE_SIZE)) {
        list.emplace_back(fileLine);
    }
    list.emplace_back("");

    vector<vector<set<char>>> groups{{}};

    int i = 0;

    for (auto &&line : list) {
        if (line.empty()) {
            i++;
            groups.emplace_back();
        } else {
            groups[i].emplace_back(line.begin(), line.end());
        }
    }

    int totalNbAnswers = 0;

    for (auto &group : groups) {
        if (!group.empty()) {
            set<char> s = accumulate(group.begin() + 1, group.end(), group[0], [](const set<char> &a, const set<char> &b) {
                set<char> ret{};
                set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(ret, ret.begin()));
                return ret;
            });
            cout << s.size() << endl;
            totalNbAnswers += s.size();
        }
    }


    cout << "total = " << totalNbAnswers << endl;

    return 0;
}
