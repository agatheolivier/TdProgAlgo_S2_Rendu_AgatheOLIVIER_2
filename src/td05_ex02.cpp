#include <iostream>

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

/************************************************************* Q1 - 5 ***************************************************************************/

