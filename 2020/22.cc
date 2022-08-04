#include "lib.h"
#include <chrono>
#include <sstream>

enum Player {
    PLAYER1, PLAYER2
};

pair<Player, map<Player, deque<u_int>>> recursiveCombat(map<Player, deque<u_int>> deck) {
    set<map<Player, deque<u_int>>> previousRounds{};
    bool won = false;
    Player winner;
    while (!won) {
        if (previousRounds.find(deck) != previousRounds.end()) {
            return {PLAYER1, deck};
        }
        previousRounds.insert(deck);

//        cout << deck << '\n';

        // Draw cards
        map<Player, u_int> cards{};
        cards[PLAYER1] = deck[PLAYER1].front();
        deck[PLAYER1].pop_front();
        cards[PLAYER2] = deck[PLAYER2].front();
        deck[PLAYER2].pop_front();

        Player roundWinner;

        if (cards[PLAYER1] <= deck[PLAYER1].size() && cards[PLAYER2] <= deck[PLAYER2].size()) {
            map<Player, deque<u_int>> _;
            tie(roundWinner, _) = recursiveCombat({
                                                  {PLAYER1, {deck[PLAYER1].begin(), deck[PLAYER1].begin() + cards[PLAYER1]}},
                                                  {PLAYER2, {deck[PLAYER2].begin(), deck[PLAYER2].begin() + cards[PLAYER2]}}
                                          });
        } else {
            roundWinner = cards[PLAYER1] > cards[PLAYER2] ? PLAYER1 : PLAYER2;
        }

        Player roundLoser = roundWinner == PLAYER1 ? PLAYER2 : PLAYER1;
        deck[roundWinner].push_back(cards[roundWinner]);
        deck[roundWinner].push_back(cards[roundLoser]);

        if (deck[PLAYER2].empty()) {
            winner = PLAYER1;
            won = true;
        }
        if (deck[PLAYER1].empty()) {
            winner = PLAYER2;
            won = true;
        }
    }
    return {winner, deck};
}

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();

    vector<string> lines = readFileLines("../input22.txt");

    Player player = PLAYER1;

    map<Player, deque<u_int>> deck;

    for (auto &&line: lines) {
        if (line.empty()) {
            player = PLAYER2;
        } else if (line[0] != 'P') {
            u_int card;
            stringstream ss{line};
            ss >> card;
            deck[player].push_back(card);
        }
    }

    auto &&[winner, finalDeck] = recursiveCombat(deck);

    cout << (winner == PLAYER1 ? "Player 1" : "Player 2 ") << " won the game\n";

    u_long score = 0;
    for (u_int i = 0; i < finalDeck[winner].size(); ++i) {
        score += (finalDeck[winner].size() - i) * finalDeck[winner][i];
    }

    cout << "score = " << score << '\n';

    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
