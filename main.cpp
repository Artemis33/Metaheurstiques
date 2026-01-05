#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <string>
#include <fstream>
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

chrono::duration<double> descenteGradient(const Graphe &graphe, Solution &solution, int nbClasses){
    auto start = chrono::steady_clock::now();
    solution.initialiserAleatoire(nbClasses);
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
    auto stop = chrono::steady_clock::now();
    chrono::duration<double> temps = stop - start;
    return temps;
    //std::cout << "Temps d'exécution : " << temps.count() << " secondes" << endl;
}

void testerDescenteGradient(const Graphe &graphe, Solution &meilleureSolution, int nbClasses, int nbTests){
    meilleureSolution.initialiserAleatoire(nbClasses);
    int meilleurCout = meilleureSolution.calculerCout(graphe);
    int pireCout = meilleurCout;
    vector<int> listeCouts;
    listeCouts.push_back(meilleurCout);

    Solution solution(graphe.getNbSommets());
    for (int i = 0; i < nbTests; i++){
        descenteGradient(graphe, solution, nbClasses);
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
    std::cout << "Meilleur cout : " << meilleurCout << endl;
    std::cout << "Pire cout : " << pireCout << endl;
    std::cout << "Moyenne des couts : " << moyenne(listeCouts) << endl;
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

chrono::duration<double> enumeration(Graphe &graphe, Solution &meilleureSolution, int nbClasses){
    auto start = chrono::steady_clock::now();
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
        // cout << "Solution : " << i << " : ";
        // for (int k = 0; k < graphe.getNbSommets(); k++){
        //     cout << listeSommets[k] << " ";
        // }

        if (isValide(listeSommets, nbClasses)){
            int val = calculerCoutVector(graphe, listeSommets);
            // cout << "Cout : " << val << endl;
            if (val < meilleurCout){
                meilleurCout = val;
                meilleureSolution.initaliserVector(listeSommets);
            }
        }
        // else {
        //     cout << "Solution invalide" << endl;
        // }
    }
    auto stop = chrono::steady_clock::now();
    chrono::duration<double> temps = stop - start;
    return temps;
}

int main(int argc, char* argv[]) {
    ofstream outfile;
	ifstream infile;
	infile.open("filenames.txt");
    string filename;
    outfile.open("results.csv");
    outfile << "Filename \t Coût de la solution \t Temps (en s)\n";
    for (int i=0; i < 17; i++) {
        infile >> filename;
        Graphe graphe("data/" + filename + ".txt");
        Solution meilleureSolution(graphe.getNbSommets());
        meilleureSolution.initialiser(2);
        chrono::duration<double> temps;
        enumeration(graphe, meilleureSolution, 2);
        cout << meilleureSolution << endl;
        int cost = meilleureSolution.calculerCout(graphe);
        outfile << filename << "\t" << cost << "\t" << temps.count() << "\n";
        infile.close();
    }
    outfile.close();
    return 0;
}
