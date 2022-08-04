#include "lib.h"

void eraseFromAllSets(vector<set<string>> &possibleFields, const string &key) {
    for (auto &&s: possibleFields) {
        if (s.size() > 1) {
            int nbErased = s.erase(key);
            if (nbErased > 0 && s.size() == 1) {
                eraseFromAllSets(possibleFields, *s.begin());
            }
        }
    }
}

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();

    auto lines = readFileLines("../input16.txt");

    map<string, vector<pair<int, int>>> rangesMap{};
    vector<int> myTicket{};
    vector<vector<int>> otherTickets{};

    int filePart = 0;
    for (auto &&line : lines) {
        if (!line.empty()) {
            if (line == "your ticket:") {
                filePart = 1;
            } else if (line == "nearby tickets:") {
                filePart = 2;
            } else if (filePart == 0) {
                vector<string> split = splitLine(line, ": ");
                string &field = split[0];
                string &constraints = split[1];
                vector<string> splitConstraints = splitLine(constraints, " or ");
                vector<pair<int, int>> ranges{};
                for (auto &&constraint : splitConstraints) {
                    vector<string> minMax = splitLine(constraint, "-");
                    ranges.emplace_back(stoi(minMax[0]), stoi(minMax[1]));
                }
                rangesMap.emplace(field, ranges);

            } else {
                auto stringValues = splitLine(line, ",");
                if (filePart == 1) {
                    transform(stringValues.begin(), stringValues.end(), back_inserter(myTicket),
                              [](const string &s) {return stoi(s);});
                } else {
                    vector<int> otherTicket{};
                    transform(stringValues.begin(), stringValues.end(), back_inserter(otherTicket),
                              [](const string &s) {return stoi(s);});
                    otherTickets.push_back(otherTicket);
                }
            }
        }
    }


    cout << rangesMap << '\n';
    cout << myTicket << '\n';
    cout << otherTickets << '\n';

    vector<set<string>> possibleFields{};

    for (auto &&value: myTicket) {
        possibleFields.emplace_back();
        for (auto &&[name, constraints] : rangesMap) {
            possibleFields.back().emplace(name);
        }
    }

    cout << possibleFields << '\n';

    int invalidCount = 0;
    for (auto &&values:otherTickets) {
        cout << "ticket " << values << '\n';
        for (int i = 0; i < values.size(); i++) {
            int val = values[i];
            set<string> possibleFieldsForVal{};
            for (auto &&[name, ranges] : rangesMap) {
                for (auto &&[min, max] : ranges) {
                    if (val >= min && val <= max) {
                        possibleFieldsForVal.insert(name);
                    }
                }
            }
            cout << "  " << possibleFieldsForVal << " possible for value " << val << '\n';
            if (possibleFieldsForVal.empty()) {
                cout << val << " is invalid\n";
                invalidCount += val;
            } else {
                set<string> resultSet{};
                set_intersection(possibleFields[i].begin(), possibleFields[i].end(),
                                 possibleFieldsForVal.begin(), possibleFieldsForVal.end(),
                                 inserter(resultSet, resultSet.begin()));
                possibleFields[i] = resultSet;
                if (possibleFields[i].size() == 1) {
                    eraseFromAllSets(possibleFields, *possibleFields[i].begin());
                }
            }
        }
    }


    cout << invalidCount << '\n';
    cout << possibleFields << '\n';

    long long ret = 1;
    for (int i = 0; i < possibleFields.size(); ++i) {
        const string &name = *possibleFields[i].begin();
        if (name.substr(0, 9) == "departure") {
            ret *= (long long) myTicket[i];
        }
    }

    cout << ret << '\n';


    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
