#include <iostream>
#include <string>

using namespace std;

#define Max_pokemonCount 100

// upozorneni pro compiler
class Pokemon;

// BOD 5: trida jako objekt
class Pokedex {
private:
    static Pokemon* registry[Max_pokemonCount];

public:
    static int totalRegistered;

    static void registerPokemon(Pokemon* p){
        if (totalRegistered < Max_pokemonCount) registry[totalRegistered++] = p;
    }

    static void TotalRegisteredStatistic(){
        cout << "Total registered Pokemon: " << totalRegistered << endl;
    }

    // deklarace virtualniho destruktoru
    static void DestroyAllPokemon(); 
};

int Pokedex::totalRegistered = 0;
Pokemon* Pokedex::registry[Max_pokemonCount];

// BOD 3: kompozice (Statistics se stane soucasti Pokemon)
class Statistics {
private:
    int hp;
    int attack;
    int defense;
    int spAttack;
    int spDefense;
    int speed;

public:
    Statistics(int hp = 10, int attack = 10, int defense = 10, 
        int spAttack = 10, int spDefense = 10, int speed = 10){

        this->hp = hp;
        this->attack = attack;
        this->defense = defense;
        this->spAttack = spAttack;
        this->spDefense = spDefense;
        this->speed = speed;
    }

    int getHP(){return hp;}
    int getAttack(){return attack;}
    int getDefense(){return defense;}
    int getSpAttack(){return spAttack;}
    int getSpDefense(){return spDefense;}
    int getSpeed(){return speed;}
};

// BOD 8: ciste abstraktni trida
class Pokemon {
protected:
    string name;
    string type;
    int level;
    Statistics stats; // BOD 3: kompozice
    bool isWild;

public:
    Pokemon(bool isWild, string name, string type, int level, int hp, int attack, int defense, int spAttack, int spDefense, int speed)
     : isWild(isWild), name(name), type(type), level(level), stats(hp, attack, defense, spAttack, spDefense, speed){
        Pokedex::registerPokemon(this); // propojeni s tridou v roli objektu
    }

    virtual ~Pokemon(){}
    virtual void BattleCry() = 0; // ciste virtualni metoda
    virtual int AttackPower() = 0; // ciste virtualni metoda

    string getName() const {
        return name;
    }

    bool getIsWild() const {
        return isWild;
    }
    
    void setCaught(){
        isWild = false;
    }

    // BOD 4: pretizene metody (DisplayInfo S a BEZ parametru)
    void DisplayInfo(){
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "Level: " << level << endl;
        cout << "Status: " << (isWild ? "Wild" : "Caught") << endl;
    }

    void DisplayInfo(bool detailed){
        DisplayInfo();
        if(detailed){
            cout << "HP: " << stats.getHP() << endl;
            cout << "Attack: " << stats.getAttack() << endl;
            cout << "Defense: " << stats.getDefense() << endl;
            cout << "Sp. Attack: " << stats.getSpAttack() << endl;
            cout << "Sp. Defense: " << stats.getSpDefense() << endl;
            cout << "Speed: " << stats.getSpeed() << endl;
        }
    }
};

// uz kompiler vi ze class Pokemon existuje a pomoci predem navarene destroy metody se uz daji pokemoni smazat (hazelo to warning)
void Pokedex::DestroyAllPokemon(){
    for(int i = 0; i < totalRegistered; i++){
        if(registry[i] != nullptr){
            delete registry[i];
        }
    }
}

// BOD 6: dedicna hierarchie 3 tridy (Pokemon ... Basic, Legendary, Mythical)
// BOD 7: rozsireni dat (isShiny, meaning, rarity) a zmena chovani pomoci override (pozdni vazba)
class BasicPokemon : public Pokemon {
private:
    bool isShiny; // BOD 7: rozsireni dat

public:    
    BasicPokemon(bool isWild, string name, string type, int level, int hp, int attack, int defense, int spAttack, int spDefense, int speed, bool isShiny)
        : Pokemon(isWild, name, type, level, hp, attack, defense, spAttack, spDefense, speed){
            this->isShiny = isShiny;
        }

    // BOD 7: zmena chovani s vyuzitim pozdni vazby
    void BattleCry() override {
        cout << name << " Pokemon cry!" << endl;
    }

    int AttackPower() override {
        cout << name << " attacks with power: " << stats.getAttack() << " | " << stats.getSpAttack() << endl;
        return stats.getAttack() + stats.getSpAttack();
    }
};

class LegendaryPokemon : public Pokemon {
private:
    string meaning; // BOD 7: rozsireni dat

public:
    LegendaryPokemon(bool isWild, string name, string type, int level, int hp, int attack, int defense, int spAttack, int spDefense, int speed, string meaning)
        : Pokemon(isWild, name, type, level, hp, attack, defense, spAttack, spDefense, speed){
        this->meaning = meaning;
    }

    void BattleCry() override {
        cout << name << " Legendary Pokemon majestic cry! (meaning: " << meaning << ")" << endl;
    }

    int AttackPower() override {
        cout << name << " attacks with ancient power: " << stats.getAttack() << " | " << stats.getSpAttack() << endl;
        return stats.getAttack() + stats.getSpAttack();
    }
};

class MythicalPokemon : public Pokemon {
private:
    int rarity; // BOD 7: rozsireni dat

public:
    MythicalPokemon(bool isWild, string name, string type, int level, int hp, int attack, int defense, int spAttack, int spDefense, int speed, int rarity)
        : Pokemon(isWild, name, type, level, hp, attack, defense, spAttack, spDefense, speed){
        this->rarity = rarity;
    }

    void BattleCry() override {
        cout << name << " Mythical Pokemon cry!" << endl;
    }

    int AttackPower() override {
        cout << name << " attacks with unknown power: " << stats.getAttack() << " | " << stats.getSpAttack() << endl;
        return stats.getAttack() + stats.getSpAttack();
    }
};

class Trainer {
private:
    string name;
    int pokemonCapacity;
    int currentPokemonCount;
    
    // BOD 9: pole pointeru na tridu i polymorfni datova struktura
    Pokemon** trainerPokemons;

public:
    Trainer(string name, int pokemonCapacity, int currentPokemonCount){
        this->name = name;
        this->pokemonCapacity = pokemonCapacity;
        this->currentPokemonCount = currentPokemonCount;
        trainerPokemons = new Pokemon*[pokemonCapacity];

        for (int i = 0; i < pokemonCapacity; i++) trainerPokemons[i] = nullptr;
    }

    ~Trainer(){
        delete[] trainerPokemons; // maze se batoh na pokemony
    }

    // BOD 9: polymorfni prirazeni (prijima se jakykoliv pokemon)
    void CatchPokemon(Pokemon* pokemon){
        if(currentPokemonCount < pokemonCapacity){
            trainerPokemons[currentPokemonCount++] = pokemon;
            pokemon->setCaught();
            cout << name << " caught " << pokemon->getName() << "!" << endl;
        } 
        else cout << name << " cannot catch more Pokemon. Capacity reached!" << endl;
    }

    void TrainerPokemonsInfo(){
        cout << "///////////////////" << endl;
        cout << name << "'s Pokemon:" << endl;
        for (int i = 0; i < currentPokemonCount; i++){
            if (trainerPokemons[i] != nullptr){
                trainerPokemons[i]->DisplayInfo(true);
                cout << "-------------------" << endl;
            }
        }
    }

    void TrainerCommand(Pokemon* pokemon){
        if(pokemon != nullptr){
            cout << name << " commands " << pokemon->getName() << " to attack!" << endl;
            // polymorfismus
            pokemon->BattleCry();
            pokemon->AttackPower();
            cout << "" << endl;
        } 
        else cout << name << " has no Pokemon to command!" << endl;
    }
};

int main(){
    Trainer ash("Ash Ketchum", 6, 0);
    Trainer n("N", 6, 0);
    
    // BOD 1: celkem 7 trid: Pokedex, Statistics, Pokemon, BasicPokemon, LegendaryPokemon, MythicalPokemon, Trainer
    
    BasicPokemon* pikachu = new BasicPokemon(true, "Pikachu", "Electric", 25, 35, 55, 40, 50, 50, 90, false);
    BasicPokemon* wooper = new BasicPokemon(true, "Wooper", "Water/Ground", 25, 55, 45, 45, 35, 35, 15, false);
    
    LegendaryPokemon* lugia = new LegendaryPokemon(true, "Lugia", "Psychic/Flying", 70, 106, 130, 130, 90, 154, 110, "Soul");
    MythicalPokemon* mew = new MythicalPokemon(true, "Mew", "Psychic", 50, 100, 100, 100, 150, 100, 100, 999);
    
    BasicPokemon* zoroark = new BasicPokemon(true, "Zoroark", "Dark", 35, 60, 105, 60, 120, 60, 105, false);
    LegendaryPokemon* reshiram = new LegendaryPokemon(true, "Reshiram", "Fire/Dragon", 70, 100, 120, 100, 150, 120, 120, "Truth");

    // nechyceni
    BasicPokemon* missingno = nullptr;
    // shiny heracross
    BasicPokemon* heracross = new BasicPokemon(true, "Heracross", "Bug/Fighting", 30, 80, 125, 75, 40, 95, 85, true);

    // BOD 10: vytvoreni nekolik desitek objektu
    for (int i = 0; i < 36; i++){
        BasicPokemon* weedle = new BasicPokemon(true, "Weedle", "Bug/Poison", 10, 40, 35, 30, 20, 20, 50, false);
    }
    
    // chytani
    ash.CatchPokemon(pikachu);
    ash.CatchPokemon(wooper);
    ash.CatchPokemon(lugia);
    ash.CatchPokemon(mew);

    n.CatchPokemon(zoroark);
    n.CatchPokemon(reshiram);

    // BOD 10: konzole
    cout << "" << endl;
    Pokedex::TotalRegisteredStatistic();

    ash.TrainerPokemonsInfo();
    cout << "///////////////////" << endl;

    ash.TrainerCommand(pikachu);
    ash.TrainerCommand(wooper);
    ash.TrainerCommand(lugia);
    ash.TrainerCommand(mew);

    n.TrainerCommand(zoroark);
    n.TrainerCommand(reshiram);

    // zde se to vola
    Pokedex::DestroyAllPokemon();

    return 0;
}