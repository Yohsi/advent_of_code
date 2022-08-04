#include "lib.h"

bool checkValid(const vector<int> &buses, const map<int, int> delays,
                size_t timestamp) {

  for (auto &&bus : buses) {
    if ((timestamp + delays.at(bus)) % bus != 0ull) {
      return false;
    }
  }
  return true;
}

int main() {

  vector<string> lines = readFileLines("input13.txt");

  // Part 1

  int minTimestamp = stoi(lines[0]);
  size_t timestamp = minTimestamp;
  vector<string> buses = splitLine(lines[1], ",");

    bool stop = false;
    int busId;
    int minWait;

    while (!stop) {
      for (auto &&bus : buses) {
        if (bus != "x") {
          int nb = stoi(bus);
          if (timestamp % nb == 0) {
            stop = true;
            busId = nb;
            minWait = timestamp - minTimestamp;
            break;
          }
        }
      }
      timestamp++;
    }
  
    cout << "Part 1: " << busId * minWait << endl;

  // Part 2

  timestamp = 0;
  vector<int> busesNb{};
  map<int, int> delays{};
  int maxBusNb = 0;

  for (auto &&bus : buses) {
    if (bus != "x") {
      int busNb = stoi(bus);
      busesNb.push_back(busNb);
      delays.insert({busNb, timestamp});
      if (busNb > maxBusNb) {
        maxBusNb = busNb;
      }
    }
    timestamp++;
  }

  size_t mul = accumulate(busesNb.begin(), busesNb.end(), 1ull,
                          [](size_t a, size_t b) { return a * b; });

  size_t res = 0;


  for (auto &&bus : busesNb) {
    if (delays[bus] != 0) {
      size_t i = 1;
      while ((mul * i / bus) % bus != 1) {
        i++;
      }
      res += mul * i / bus * (bus-delays[bus]);
    }
  }

  res = res % mul;

  cout << "Part 2: " << res << '\n';

  return 0;
}
