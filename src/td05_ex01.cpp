#include <iostream>

//Hashing simple
size_t folding_string_hash(std::string const& s, size_t max){
    size_t hash {0};
    for(size_t i {0}; i < s.size(); i++){
        //hash += s[i]; //J'ajoute la valeur ASCII du caractére et je fais un hash à ce moment là
        //hash %= max; //Impossible que ce soit plus grand que le max + 255 au tour suivant
        //Evite que le nombre à stocker soit trop grand pour size_t

        hash = (hash + s[i]) % max; //Les deux lignes au dessus regroupées
    }

    return hash;
}

//Hashing Ordonné
size_t folding_string_ordered_hash(std::string const& s, size_t max){
    size_t hash {0};
    for(size_t i {0}; i < s.size(); i++){
       hash = (hash + s[i]*(i+1)) % max; //On fait le modulo sur la valeur hachée
    }

    return hash;
}

int main() {
    std::string str {0};
    std::cout << "Entrez une phrase : " << std::endl;
    std::cin >> str;

    size_t HashingSimpleResultat = folding_string_hash(str, 4096);
    std::cout << "La somme des valeurs ASCII des caractères est " <<  HashingSimpleResultat << std::endl;
    //Teste avec coucou (99, 111, 117) x 3 = 654

    size_t HashingOrdonneResultat = folding_string_ordered_hash(str, 4096);
    std::cout << "La somme des valeurs ASCII des caractères en prenant en compte leur position est " <<  HashingOrdonneResultat << std::endl;
    //Teste avec coucou 99*1 + 111*2 + 117*3 + 99*4 + 111*5 + 117*6 = 2325
    return 0;
}