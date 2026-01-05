#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include "solution.hpp"

using namespace std;

Solution::Solution(int nbSommets){
    classe = vector<int>(nbSommets, -1);
}

int& Solution::operator[](int i){
    return classe[i];
}

ostream& operator<<(ostream& os, const Solution& solution){
    for (int i = 0; i < solution.classe.size(); i++){
        os << "Sommet " << i + 1 << ", classe " << solution.classe[i] << endl;
    }
    return os;
}

int Solution::calculerCout(const Graphe& graphe) const{
    int cout = 0;
    for (int i = 0; i < graphe.getNbSommets(); i++){
        for (int j = i + 1; j < graphe.getNbSommets(); j++){
            if (classe[i] != classe[j]){
                cout += graphe(i, j);
            }
        }
    }
    return cout;
}  

int Solution::calculerGain(const Graphe& graphe, int sommet1, int sommet2) const{
    int gain = 0;
    for (int i = 0; i < graphe.getNbSommets(); i++){
        if (classe[sommet1] != classe[i] && i != sommet2){ //on enlève les arcs qui partent de sommet1 et qui vont dans une autre classe
            gain -= graphe(sommet1, i);
        }
        if (classe[sommet2] != classe[i] && i != sommet1){ //on enlève les arcs qui partent de sommet2 et qui vont dans une autre classe
            gain -= graphe(sommet2, i);
        }
        if (classe[sommet2] != classe[i] && i != sommet2){ //on ajoute les arcs qui partent de sommet1 et qui vont dans la même classe que sommet2
            gain += graphe(sommet1, i);
        }
        if (classe[sommet1] != classe[i] && i != sommet1){ //on ajoute les arcs qui partent de sommet2 et qui vont dans la même classe que sommet1
            gain += graphe(sommet2, i);
        }
    }
    return gain;
}

void Solution::swap(int sommet1, int sommet2){
    int temp = classe[sommet1];
    classe[sommet1] = classe[sommet2];
    classe[sommet2] = temp;
}

void Solution::initialiser(int nbClasses){
    int nbSommetsParClasse = floor(classe.size() / nbClasses);
    int nbExceptions = classe.size() % nbClasses;

    int sommet = 0;
    for (int i = 0; i < nbClasses; i++){
        if (i < nbExceptions) {
            for (int j = 0; j < nbSommetsParClasse + 1; j++){
                classe[sommet] = i;
                sommet++;
            }
        } else {
            for (int j = 0; j < nbSommetsParClasse; j++){
                classe[sommet] = i;
                sommet++;
            }
        }
    }
}


void Solution::initialiserAleatoire(int nbClasses){
    srand(time(NULL));
    int nbSommetsParClasse = floor(classe.size() / nbClasses);
    int nbExceptions = classe.size() % nbClasses;

    unordered_set<int> sommets;
    for (int i = 0; i < classe.size(); i++){
        sommets.insert(i);
    }

    for (int i = 0; i < nbClasses; i++){
        if (i < nbExceptions) {
            for (int j = 0; j < nbSommetsParClasse + 1; j++){
                int position = rand() % sommets.size();
                int compteur = 0;
                for (int element : sommets){
                    if (compteur == position){
                        classe[element] = i;
                        sommets.erase(element);
                        break;
                    }
                    compteur++;
                }
            }
        } else {
            for (int j = 0; j < nbSommetsParClasse; j++){
                int position = rand() % sommets.size();
                int compteur = 0;
                for (int element : sommets){
                    if (compteur == position){
                        classe[element] = i;
                        sommets.erase(element);
                        break;
                    }
                    compteur++;
                }
            }
        }
    }
}

void Solution::initaliserVector(vector<int> vecteur){
    classe = vecteur;
}

