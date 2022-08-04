#include "lib.h"
#include <chrono>

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();

    vector<string> lines = readFileLines("../input21.txt");

    map<string, set<string>> ingrByAllergen{};
    map<string, int> ingCount{};
    set<string> ingredientsSet{};

    for (auto &&line : lines) {
        auto split1 = splitLine(line, " (contains ");
        vector<string> ingredients = splitLine(split1[0], " ");
        vector<string> allergens = splitLine(split1[1].substr(0, split1[1].size() - 1), ", ");

        for (auto &&i:ingredients) {
            ingCount[i]++;
            ingredientsSet.insert(i);
        }

        for (auto &&a: allergens) {
            auto &ingrSet = ingrByAllergen[a];
            set<string> newSet{ingredients.begin(), ingredients.end()};
            if (ingrSet.empty()) {
                ingrSet = move(newSet);
            } else {
                set<string> ingrSetCopy{ingrSet.begin(), ingrSet.end()};
                ingrSet.clear();
                set_intersection(ingrSetCopy.begin(), ingrSetCopy.end(), newSet.begin(), newSet.end(),
                                 inserter(ingrSet, ingrSet.begin()));
            }
        }
    }
    cout << ingrByAllergen << '\n';

    set<string> allergenIngredients{};
    bool modified = true;

    while (modified) {
        modified = false;
        for (auto &&[al, ing] : ingrByAllergen) {
            if (ing.size() == 1) {
                auto onlyIng = *ing.begin();
                allergenIngredients.insert(onlyIng);
                for (auto &&[innerAl, innerIng] : ingrByAllergen) {
                    if (innerAl != al)
                        modified |= innerIng.erase(onlyIng) > 0;
                }
            }
        }
    }
    cout << ingrByAllergen << '\n';

    set<string> noAllergenIngredients{};
    set_difference(ingredientsSet.begin(), ingredientsSet.end(), allergenIngredients.begin(), allergenIngredients.end(),
                   inserter(noAllergenIngredients, noAllergenIngredients.begin()));

    size_t nb = 0;
    for (auto &&i: noAllergenIngredients) {
        nb += ingCount[i];
    }

    cout << nb << endl;

    for (auto &&[al, ing] : ingrByAllergen) {
       cout << *ing.begin() << ",";
    }

    cout << '\n';

    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "temps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
