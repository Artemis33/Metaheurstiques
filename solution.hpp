#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <iostream>
#include <vector>
#include "graphe.hpp"

using namespace std;

class Solution {
    private :
        vector<int> classe;

    public :
        // Constructeur
        Solution(int nbSommets);

        // Opérateur
        int& operator[](int i);
        friend ostream& operator<<(ostream& os, const Solution& solution);

        // Autres
        void initialiser(int nbClasses);
        void initialiserAleatoire(int nbClasses);
        void initaliserVector(vector<int> vecteur);
        int calculerCout(const Graphe& graphe) const;
        int calculerGain(const Graphe& graphe, int sommet1, int sommet2) const;
        void swap(int sommet1, int sommet2);
};

#endif