// Mini-Vim, editeur de texte en C++

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// Fonction

void saveFile(const string& fileName, const vector<string>& lines) {
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (const auto& line : lines) {
            outFile << line << endl;
        }
        outFile.close();
        cout << "Fichier sauvegarde avec succes : " << fileName << endl;
    } else {
        cerr << "Erreur lors de l'ouverture du fichier pour la sauvegarde : " << fileName << endl;
    }
}

void loadFile(const string& fileName, vector<string>& lines) {
    ifstream inFile(fileName);
    if (inFile.is_open()) {
        lines.clear();
        string line;
        while (getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();
        cout << "Fichier charge avec succes : " << fileName << endl;
    } else {
        cerr << "Erreur lors de l'ouverture du fichier pour le chargement : " << fileName << endl;
    }
}

void showFile(const vector<string>& lines) {
    for (size_t i = 0; i < lines.size(); ++i) {
        cout << i + 1 << " | " << lines[i] << endl;
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <fileName>" << endl;
        return 1;
    }

    string fileName = argv[1];
    vector<string> lines;

    cout << "Bienvenue dans E-Wan, l'editeur de texte en C++ !" << std::endl;
    cout << "Entrez votre texte ci-dessous (tapez :help pour afficher l'aide)" << std::endl;

    ifstream file(fileName);
    if (file.is_open()) {   // Le fichier existe deja, on le charge
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {   // Le fichier n'existe pas, on le crée
        ofstream newFile(fileName);
        newFile.close();
    }

    cout << lines.size() << " ligne(s) chargee(s) dans le fichier : " << fileName << endl;
    showFile(lines);

    bool isRunning = true;
    string input;

    while (isRunning && getline(cin, input)) {
        if (input == ":help") {
            cout << "Commandes disponibles :" << endl;
            cout << ":help - Affiche l'aide" << endl;
            cout << ":save - Sauvegarde le texte dans un fichier" << endl;
            cout << ":exitS! - Quitte l'editeur avec sauvegarde" << endl;
            cout << ":load - Charge le texte depuis un fichier" << endl;
            cout << ":exit! - Quitte l'editeur sans sauvegarder" << endl;
        } 
        else if (input == ":save") { saveFile(fileName, lines); }
        else if (input == ":exitS!") {
            saveFile(fileName, lines);
            isRunning = false;
        }
        else if (input == ":load") { loadFile(fileName, lines); }
        else if (input == ":exit!") { isRunning = false; }
        else { lines.push_back(input); }
    }

    cout << "Merci d'avoir utilise E-Wan !" << endl;
    return 0;
}