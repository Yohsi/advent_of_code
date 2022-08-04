#include "lib.h"

long step(long val, long subjectNumber) {
    return (val * subjectNumber) % 20201227l;
}

long transform(long subjectNumber, long loopSize) {
    long val = 1;
    for (int i = 0; i < loopSize; ++i) {
        val = step(val, subjectNumber);
    }
    return val;
}

long findLoopSize(long subjectNumber, long val) {
    long calcVal = 1;
    long iter = 0;
    while (calcVal != val) {
        calcVal = step(calcVal, subjectNumber);
        iter++;
    }
    return iter;
}

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();


    ifstream file;
    file.open("../input25.txt");

    if (file.fail()) {
        throw runtime_error("Impossible d'ouvrir le fichier");
    }

    long cardPublicKey;
    long doorPublicKey;

    file >> cardPublicKey;
    file >> doorPublicKey;

    long cardLoopSize = findLoopSize(7, cardPublicKey);
    long doorLoopSize = findLoopSize(7, doorPublicKey);

    cout << "Card's loop size: " << cardLoopSize << '\n';
    cout << "Door's loop size: " << doorLoopSize << '\n';

    long encryptionKeyDoor = transform(cardPublicKey, doorLoopSize);
    cout << "Encryption key according to the door: " << encryptionKeyDoor << '\n';
    long encryptionKeyCard = transform(doorPublicKey, cardLoopSize);
    cout << "Encryption key according to the card: " << encryptionKeyCard << '\n';

    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
