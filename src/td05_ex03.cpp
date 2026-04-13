#include <iostream>
#include <vector>
#include <istream>
#include <fstream>
#include <unordered_map>

enum class CardKind {
    Heart,
    Diamond,
    Club,
    Spade,
};

enum class CardValue {
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace,
};

/************************************************************* Q1 ***************************************************************************/

struct Card {
    CardKind kind;
    CardValue value;
};

bool operator==(Card const& card1, Card const& card2) {
    return card1.kind == card2.kind && card1.value == card2.value;
}

/************************************************************* Q3 ***************************************************************************/

namespace std {
    template<>
    struct hash<Card> {
        size_t operator()(Card const& card) const {
            std::hash<int> hash;
            size_t hKind = hash(static_cast<int>(card.kind));
            size_t hValue = hash(static_cast<int>(card.value)); 
            return hKind * 13 + hValue; //Chaque motif a 13 nombres possibles, donc pour passer au motif suivant, il faut sauter 13 cases
            //kind * 13 permet d'être au début du motif 
        }
    };
}

std::vector<Card> get_cards(size_t const size) {
    std::vector<Card> cards {};
    cards.reserve(size);
    for (size_t i {0}; i < size; ++i) {
        cards.push_back({static_cast<CardKind>(rand() % 4), static_cast<CardValue>(rand() % 13)});
    }
    return cards;
}

/************************************************************* Q4 ***************************************************************************/

std::string card_name(Card const& card) {
    std::string name {};

    unsigned int card_value {(static_cast<unsigned int>(card.value)+2) % 14};

    if (card_value < 10) {
        name += '0' + std::to_string(card_value);
    }else if (card_value == 10) {
        name += "10";
    }else if (card_value == 11) {
        name += 'J';
    }else if (card_value == 12) {
        name += 'Q';
    }else if (card_value == 13) {
        name += 'K';
    }

    name += " of ";

    if (card.kind == CardKind::Heart) {
        name += "Heart";
    }else if (card.kind == CardKind::Diamond) {
        name += "Diamond";
    }else if (card.kind == CardKind::Club) {
        name += "Club";
    }else if (card.kind == CardKind::Spade) {
        name += "Spade";
    }
    return name;
}

int main(){
    std::vector<Card> cartes = get_cards(100);

    std::unordered_map<Card, int> nbCartes; //Contient la carte et le nombre de fois où elle apparait

    for (const Card& carteActuel : cartes) {
        nbCartes[carteActuel]++; //unordered_map : Compte le nombre de fois qu'une même valeur apparait
    }

    for (const auto& carteActuel : nbCartes) {
        Card carte = carteActuel.first; //La carte
        int nombre = carteActuel.second; //Nombre de fois qu'elle apparait

        std::cout << card_name(carte) << " : " << nombre << std::endl;
    }

}