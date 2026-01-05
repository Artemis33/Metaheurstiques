#include <iostream>
#include <fstream>
#include <sstream>
#include "graphe.hpp"

using namespace std;

Graphe::Graphe(string nomFichier){
    ifstream lecture(nomFichier);
    if (lecture){
        string ligne;
        getline(lecture, ligne); // On lit la première ligne (inutile)
        getline(lecture, ligne); // On lit la deuxième ligne
        istringstream ss(ligne);
        int nbSommets;
        int nbArcs;
        ss >> nbSommets;
        graphe = vector<vector<int>>(nbSommets, vector<int>(nbSommets, 0));
        ss >> nbArcs;
        getline(lecture, ligne); // On lit la troisième ligne (inutile)
        getline(lecture, ligne); // On lit la quatrième ligne (inutile)
        getline(lecture, ligne); // On lit la cinquième ligne (inutile)
        for (int i = 0; i < nbArcs; i++){
            getline(lecture, ligne);
            istringstream ss2(ligne);
            int sommet1, sommet2, poids;
            ss2 >> sommet1 >> sommet2 >> poids;
            graphe[sommet1 - 1][sommet2 - 1] = poids; // On soustrait 1 car les sommets sont numérotés à partir de 1
            graphe[sommet2 - 1][sommet1 - 1] = poids; 
        }
    } else {
        cout << "Erreur lors de l'ouverture du fichier" << endl;
    }
}

ostream& operator<<(ostream& os, const Graphe& graphe){
    for (int i = 0; i < graphe.graphe.size(); i++){
        os << "Sommet " << i + 1 << " : ";
        for (int j = 0; j < graphe.graphe[i].size(); j++){
            if (graphe.graphe[i][j] != 0){
                os << j + 1 << " (" << graphe.graphe[i][j] << ") ";
            }
        }
        os << endl;
    }
    return os;
}