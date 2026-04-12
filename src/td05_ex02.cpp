#include <iostream>
#include <vector>
#include <istream>
#include <fstream>
#include <unordered_set>

/************************************************************* Q1 - 1 ***************************************************************************/
enum Direction {
  Haut, 
  Droite, 
  Bas, 
  Gauche
};

/************************************************************* Q1 - 2 ***************************************************************************/

struct Position {
    int x;
    int y;

    //Q1 - 5
    Position& operator+=(Position const& p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    //Q1 - 6 => friend permet de le mettre dans la struct, en signalant qu'il pourrait être en dehors (vu avec Jules)
    friend Position operator+(Position  a, Position const& b){
        a+=b;
        return a;
    }
};

Position vecteurPosition(Direction direction) {
    switch(direction) {
        case Direction::Haut:  
            return {0, -1};

        case Direction::Droite:  
            return {1, 0};

        case Direction::Bas:  
            return {0, 1};

        case Direction::Gauche:  
            return {-1, 0};

    }
    return {0, 0}; //Au cas où ça bug
}

/************************************************************* Q1 - 3 ***************************************************************************/

bool operator==(Position const& position1, Position const& position2) {
    return position1.x == position2.x && position1.y == position2.y;
}

/************************************************************* Q1 - 4 ***************************************************************************/

std::ostream& operator<<(std::ostream& os, Position const& position) {
    return os << '(' << position.x << ", " << position.y << ')';
}

/************************************************************* Q1 - 6 ***************************************************************************/

Position operator+(Position const& position, Direction const& direction) {
    Position deplacement = vecteurPosition(direction);
    return {position + deplacement};
}

Position& operator+=(Position& position, Direction const& direction) {
    Position deplacement = vecteurPosition(direction);
    position = position + deplacement;
    return position;
}

/************************************************************* Q1 - 7 ***************************************************************************/

Direction turn_right(Direction const& directionActuelle){
     switch(directionActuelle) {
        case Direction::Haut:  
            return Direction::Droite;

        case Direction::Droite:  
            return Direction::Bas;

        case Direction::Bas:  
            return Direction::Gauche;

        case Direction::Gauche:  
            return Direction::Haut;

    }
    return directionActuelle; //Au cas où ça bug
}

/************************************************************* Q3 - 1 ***************************************************************************/

//A définir avant l'utilisation de unordered_set
namespace std {
    template <>
    struct hash<Position> {
        std::size_t operator()(const Position& pos) const {
            std::hash<int> hash;
            size_t hX = hash(pos.x); //hash de x
            size_t hY = hash(pos.y); //hash de y

            return hX ^ (hY * 2); //XOR est une opération bit par bit, quand les bits on les mêmes valeurs il vaut 0, sinon il vaut 1
            // /!\ Le XOR est commutatif ! Comme on bosse avec des positions il faut que le hash de hX et hY soient différents 
            //On peut juste multiplier l'un des deux pour créer un décalage dans le hashage
        }
    };
}

/************************************************************* Q2 ***************************************************************************/

struct Input_Structure {
    Position positionIniale {};
    Direction directionInitiale;
    std::vector<std::string> carte;
    std::unordered_set<Position> obstacle;
};

Input_Structure parse_input(std::istream& input_stream) {
    Input_Structure carteActuel;
    int ligne = 0;

    for (std::string line{}; std::getline(input_stream, line) and line != "";) {
        //std::cout << line << '\n'; //Vérification que l'import fonctionne

        for (size_t colonne = 0; colonne < line.size(); ++colonne) {
            if (line[colonne] == '^') {
                carteActuel.positionIniale = {static_cast<int>(colonne), ligne};
                carteActuel.directionInitiale = Direction::Haut;
            }
            else if (line[colonne] == '<') {
                carteActuel.positionIniale = {static_cast<int>(colonne), ligne};
                carteActuel.directionInitiale = Direction::Gauche;
            }
            else if (line[colonne] == '>') {
                carteActuel.positionIniale = {static_cast<int>(colonne), ligne};
                carteActuel.directionInitiale = Direction::Droite;
            }
            else if (line[colonne] == 'v') {
                carteActuel.positionIniale = {static_cast<int>(colonne), ligne};
                carteActuel.directionInitiale = Direction::Bas;
            }
            else if (line[colonne] == '#') {
                Position obstacle = {static_cast<int>(colonne), ligne};
                carteActuel.obstacle.insert(obstacle);
            } 
        }
        carteActuel.carte.push_back(line);
        ligne++;
    }
    
    return carteActuel;
}

/************************************************************* Q3 - 1 ***************************************************************************/

struct WalkResult {
    Position final_position;
    size_t steps_taken;
    std::unordered_set<Position> visited_positions;
};

/************************************************************* Q3 - 2 ***************************************************************************/

bool interieurCarte(Position positionActuelle, const std::vector<std::string>& carte) {
    return positionActuelle.y >= 0 && positionActuelle.y < (int)carte.size()
        && positionActuelle.x >= 0 && positionActuelle.x < (int)carte[0].size(); //nombre de colonnes de cla ligne 0
}

WalkResult mouvementGarde(Input_Structure carteActuel) {
    WalkResult resultatFinal;

    while (interieurCarte(carteActuel.positionIniale, carteActuel.carte)) { //Tant que le garde est dans la carte
        if (!interieurCarte(carteActuel.positionIniale, carteActuel.carte)) break;
        Position positionProvisoire = carteActuel.positionIniale;

        //Droite
        if (carteActuel.directionInitiale == Direction::Droite){
            positionProvisoire += Direction::Droite;
            if (carteActuel.obstacle.find(positionProvisoire) != carteActuel.obstacle.end()) {
                carteActuel.directionInitiale = turn_right(carteActuel.directionInitiale);
            } 
            else {
                carteActuel.positionIniale += Direction::Droite;
                resultatFinal.steps_taken++;
                if (resultatFinal.visited_positions.find(positionProvisoire) == resultatFinal.visited_positions.end()) { //Si find ne retrouve rien, ça renvoie end
                        resultatFinal.visited_positions.insert(positionProvisoire);
                }
            }
        }

        //Bas
        else if (carteActuel.directionInitiale == Direction::Bas){
            positionProvisoire += Direction::Bas;
            if (carteActuel.obstacle.find(positionProvisoire) != carteActuel.obstacle.end()) {
                carteActuel.directionInitiale = turn_right(carteActuel.directionInitiale);
            } 
            else {
                carteActuel.positionIniale += Direction::Bas;
                resultatFinal.steps_taken++;
                if (resultatFinal.visited_positions.find(positionProvisoire) == resultatFinal.visited_positions.end()) { //Si find ne retrouve rien, ça renvoie end
                        resultatFinal.visited_positions.insert(positionProvisoire);
                }
            }
        }
        
        //Gauche
        else if (carteActuel.directionInitiale == Direction::Gauche){
            positionProvisoire += Direction::Gauche;
            if (carteActuel.obstacle.find(positionProvisoire) != carteActuel.obstacle.end()) {
                carteActuel.directionInitiale = turn_right(carteActuel.directionInitiale);
            } 
            else {
                carteActuel.positionIniale += Direction::Gauche;
                resultatFinal.steps_taken++;
                if (resultatFinal.visited_positions.find(positionProvisoire) == resultatFinal.visited_positions.end()) { //Si find ne retrouve rien, ça renvoie end
                        resultatFinal.visited_positions.insert(positionProvisoire);
                }
            }
        }

        
        //Haut
        else if (carteActuel.directionInitiale == Direction::Haut){
            positionProvisoire += Direction::Haut;
            if (carteActuel.obstacle.find(positionProvisoire) != carteActuel.obstacle.end()) {
                carteActuel.directionInitiale = turn_right(carteActuel.directionInitiale);
            } 
            else {
                carteActuel.positionIniale += Direction::Haut;
                resultatFinal.steps_taken++;
                if (resultatFinal.visited_positions.find(positionProvisoire) == resultatFinal.visited_positions.end()) { //Si find ne retrouve rien, ça renvoie end
                        resultatFinal.visited_positions.insert(positionProvisoire);
                }
            }
        }
    }

    resultatFinal.final_position = carteActuel.positionIniale;

    return resultatFinal;
}


int main(){
    //Vérifications
    /* Position positionIniale = {4,6};
    std::cout << "La position initatiale est :" << positionIniale << std::endl;
    Direction deplacement =  Direction::Haut;
    positionIniale += deplacement;
    std::cout << "Le garde va en haut, la nouvelle position est :" << positionIniale << std::endl;
    positionIniale += turn_right(deplacement);
    std::cout << "Le garde tourne à droite, la nouvelle position est :" << positionIniale << std::endl; */

    std::ifstream file;
    file.open("../../src/carte.txt");
    if (!file.is_open()) {
        std::cout << "Fichier introuvable" << std::endl;
         return 1;
    }

    auto input_structure = parse_input(file);
    WalkResult resultatFinal = mouvementGarde(input_structure);
    std::cout << "La position finale est " << resultatFinal.final_position << std::endl;
    std::cout << "Le nombre de positions différentes visitées est " << resultatFinal.visited_positions.size()-1 << std::endl;
    std::cout << "Le nombre de pas total est " << resultatFinal.steps_taken << std::endl;
}