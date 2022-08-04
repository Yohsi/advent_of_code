#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

const int MAX_LINE_SIZE = 100;
using namespace std;

vector<string> splitLine(const string &input, const string &delimiter) {
    size_t begin = 0;
    size_t end;
    vector<string> ret{};

    while ((end = input.find(delimiter, begin)) != string::npos) {
        ret.push_back(input.substr(begin, end - begin));
        begin = end + delimiter.size();
    }
    ret.push_back(input.substr(begin));

    return ret;
}

bool isValid(const pair<string, string> &prop) {
    string key = prop.first;
    string val = prop.second;

    if (key == "byr") {
        return val.size() == 4 &&
               all_of(val.begin(), val.end(), [](char c) {return isdigit(c);}) &&
               1920 <= stoi(val) && stoi(val) <= 2002;
    }
    if (key == "iyr") {
        return val.size() == 4 &&
               all_of(val.begin(), val.end(), [](char c) {return isdigit(c);}) &&
               2010 <= stoi(val) && stoi(val) <= 2020;
    }
    if (key == "eyr") {
        return val.size() == 4 &&
               all_of(val.begin(), val.end(), [](char c) {return isdigit(c);}) &&
               2020 <= stoi(val) && stoi(val) <= 2030;
    }
    if (key == "hgt") {
        string nb = val.substr(0, val.size() - 2);
        string unit = val.substr(val.size() - 2, 2);
        return all_of(nb.begin(), nb.end(), [](char c) {return isdigit(c);}) &&
               ((unit == "cm" && 150 <= stoi(nb) && stoi(nb) <= 193) ||
                (unit == "in" && 59 <= stoi(nb) && stoi(nb) <= 76));
    }
    if (key == "hcl") {
        string nb = val.substr(1, val.size() - 1);
        return val[0] == '#' &&
               nb.size() == 6 &&
               all_of(nb.begin(), nb.end(), [](char c) {return isxdigit(c);});
    }
    if (key == "ecl") {
        const vector<string> validEcl{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        return find(validEcl.begin(), validEcl.end(), val) != validEcl.end();
    }
    if (key == "pid") {
        return val.size() == 9 &&
               all_of(val.begin(), val.end(), [](char c) {return isdigit(c);});
    }
    if (key == "cid")
        return true;

    cout << "key inconnue : " << key << endl;
    return false;
}

bool isValid(const map<string, string> &props) {
    return all_of(props.begin(), props.end(), [](const pair<string, string> &prop) {return isValid(prop);});
}

int main() {
    cout << "processing..." << endl;

    ifstream file;
    file.open("../input4.txt");

    if (file.fail()) {
        cerr << "Impossible d'ouvrir le fichier" << endl;
        return 1;
    }

    vector<string> list;


    char fileLine[MAX_LINE_SIZE];
    while (file.getline(fileLine, MAX_LINE_SIZE)) {
        list.emplace_back(fileLine);
    }

    vector<vector<string>> passports{{}};

    int i = 0;

    for (auto &&line : list) {
        if (line.empty()) {
            i++;
            passports.emplace_back();
        } else {
            auto prop = splitLine(line, " ");
            move(prop.begin(), prop.end(), back_inserter(passports[i]));
        }
    }

    const set<string> expectedFields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    int nbValid = 0;

    for (auto &&passport : passports) {
        set<string> keys{};
        map<string, string> props;
        for (auto &&prop : passport) {
            auto keyVal = splitLine(prop, ":");
            string key = keyVal[0];
            string val = keyVal[1];
            keys.insert(key);
            props.insert({key, val});
        }
        if (includes(keys.begin(), keys.end(), expectedFields.begin(), expectedFields.end()) && isValid(props)) {
            nbValid++;
        }
    }


    cout << nbValid << endl;
    return 0;
}
