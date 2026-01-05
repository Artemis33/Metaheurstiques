#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>
#include "graphe.hpp"
#include "solution.hpp"

using namespace std;
//using namespace std::chrono;

double moyenne(const vector<int> &liste){
    double somme = 0;
    for (int i = 0; i < liste.size(); i++){
        somme += liste[i];
    }
    return somme / liste.size();
}

double ecartType(const vector<int> &liste){
    double moy = moyenne(liste);
    double somme = 0;
    for (int i = 0; i < liste.size(); i++){
        somme += pow(liste[i] - moy, 2);
    }
    return sqrt(somme / liste.size());
}

void trier(vector<int> &liste){
    for (int i = 0; i < liste.size(); i++){
        for (int j = i + 1; j < liste.size(); j++){
            if (liste[i] > liste[j]){
                int temp = liste[i];
                liste[i] = liste[j];
                liste[j] = temp;
            }
        }
    }
}

double mediane(vector<int> &liste){
    trier(liste);
    if (liste.size() % 2 == 0){
        return (liste[liste.size() / 2] + liste[liste.size() / 2 - 1]) / 2;
    } else {
        return liste[liste.size() / 2];
    }
}

void descenteGradient(const Graphe &graphe, Solution &solution, int nbClasses, int seed ){
    solution.initialiserAleatoire(nbClasses, seed);
    int meilleurCout = solution.calculerCout(graphe);
    
    bool fini = false;
    while (!fini){
        int ibest = -1;
        int jbest = -1;
        int meilleurGain = 0;
        for (int i = 0; i < graphe.getNbSommets(); i++){
            for (int j = i + 1; j < graphe.getNbSommets(); j++){
                if (solution[i] == solution[j]){
                    continue;
                }
                int gain = solution.calculerGain(graphe, i, j);
                if (gain < meilleurGain){
                    meilleurGain = gain;
                    ibest = i;
                    jbest = j;
                }
            }
        }
        if (meilleurGain < 0){
            solution.swap(ibest, jbest);
            meilleurCout += meilleurGain;
        } else {
            fini = true;
        }
    }
}

void testerDescenteGradient(const Graphe &graphe, Solution &meilleureSolution, int nbClasses, int nbTests){
    auto start = chrono::steady_clock::now();

    meilleureSolution.initialiserAleatoire(nbClasses, 0);
    int meilleurCout = meilleureSolution.calculerCout(graphe);
    int pireCout = 0;
    vector<int> listeCouts;

    Solution solution(graphe.getNbSommets());
    for (int i = 0; i < nbTests; i++){
        descenteGradient(graphe, solution, nbClasses, i);
        int val = solution.calculerCout(graphe);
        listeCouts.push_back(val);
        if (val < meilleurCout){
            meilleurCout = val;
            meilleureSolution = solution;
        }
        if (val > pireCout){
            pireCout =  val;
        }
    }
    auto stop = chrono::steady_clock::now();
    chrono::duration<double> temps = chrono::duration_cast<chrono::duration<double>>(stop - start);
    std::cout << "Temps d'exécution : " << temps.count() << " secondes" << endl;
    std::cout << "Meilleur cout : " << meilleurCout << endl;
    std::cout << "Pire cout : " << pireCout << endl;
    std::cout << "Moyenne des couts : " << moyenne(listeCouts) << endl;
    std::cout << "Ecart type des couts : " << ecartType(listeCouts) << endl;
    std::cout << "Mediane des couts : " << mediane(listeCouts) << endl;
}

bool isValide(vector<int> &listeSommets, int nbClasses){
    int nbSommetsParClasse = floor(listeSommets.size() / nbClasses);
    int nbExceptions = listeSommets.size() % nbClasses;
    vector<int> listeClasses(nbClasses, 0);
    for (int i = 0; i < listeSommets.size(); i++){
        listeClasses[listeSommets[i]]++;
    }
    for (int i = 0; i < listeClasses.size(); i++){
        if (i < nbExceptions){
            if (listeClasses[i] != nbSommetsParClasse + 1){
                return false;
            }
        } else {
            if (listeClasses[i] != nbSommetsParClasse){
                return false;
            }
        }
    }
    return true;
}

int calculerCoutVector(const Graphe &graphe, vector<int> &listeSommets){
    int cout = 0;
    for (int i = 0; i < graphe.getNbSommets(); i++){
        for (int j = i + 1; j < graphe.getNbSommets(); j++){
            if (listeSommets[i] != listeSommets[j]){
                cout += graphe(i, j);
            }
        }
    }
    return cout;
}

void enumeration(Graphe &graphe, Solution &meilleureSolution, int nbClasses){
    int meilleurCout = meilleureSolution.calculerCout(graphe); //faut initialiser
    vector<int> listeSommets(graphe.getNbSommets(), 0);
    int nbSommet = graphe.getNbSommets();
    for (int i = 0; i < pow(nbClasses, nbSommet); i++){
        int j = 0;
        while(listeSommets[j] == nbClasses - 1){
            listeSommets[j] = 0;
            j++;
        }
        listeSommets[j]++;

        /*
        for (int k = 0; k < graphe.getNbSommets(); k++){
            cout << listeSommets[k] << " ";
        }
        cout << endl;
        */
        

        if (isValide(listeSommets, nbClasses)){
            int val = calculerCoutVector(graphe, listeSommets);
            if (val < meilleurCout){
                meilleurCout = val;
                meilleureSolution.initaliserVector(listeSommets);
            }
        }
    }
}

int main(int argc, char** argv) { //argv[1] = nom du fichier, argv[2] = nombre de classes, argv[3] = méthode
    Graphe graphe(argv[1]);
    Solution meilleureSolution(graphe.getNbSommets());
    istringstream iss(argv[2]);
    int nbClasses;
    iss >> nbClasses;
    meilleureSolution.initialiser(nbClasses);


    if (string(argv[3]) == "descente"){
        testerDescenteGradient(graphe, meilleureSolution, nbClasses, 1);
    } else if (string(argv[3]) == "enumeration"){
        auto start = chrono::steady_clock::now();
        enumeration(graphe, meilleureSolution, nbClasses);
        std::cout << "fin de l'enumeration" << endl;
        auto stop = chrono::steady_clock::now();
        chrono::duration<double> temps = chrono::duration_cast<chrono::duration<double>>(stop - start);
        std::cout << "Temps d'exécution : " << temps.count() << " secondes" << endl;
    }

    std::cout << "cout final : " << meilleureSolution.calculerCout(graphe) << endl;
    return 0;
}

