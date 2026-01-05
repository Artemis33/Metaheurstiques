#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <vector>

using namespace std;

class Graphe {
    private :
        vector<vector<int>> graphe;
    public :
        // Constructeur
        Graphe(string nomFichier);

        // Opérateur
        int operator()(int i, int j) const { return graphe[i][j]; }
        vector<int>& operator[](int i) { return graphe[i]; }
        friend ostream& operator<<(ostream& os, const Graphe& graphe);

        // getter
        int getNbSommets() const { return graphe.size(); }
};

#endif