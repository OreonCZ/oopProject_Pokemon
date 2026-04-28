# Pokédex (Simulace Pokémonů)

Tento projekt implementuje objektový systém pro správu a interakci s Pokémony a jejich trenéry (OOP && C++).

## Architektura projektu

#### `Pokedex` (třída v roli objektu)
Funguje jako centrální statický registr (**memory manager**). Nemá žádné instance, obsahuje pouze statická data (pole pointerů na všechny vytvořené Pokémony) a metody. 

Využívá se zde návrhový vzor k automatickému mazání všech dynamicky vytvořených objektů skrze metodu `DestroyAllPokemon()`, čímž nedojde k memory leaku.

#### `Statistics` (komponenta pro kompozici)
Zapouzdřuje bojové atributy, jako jsou health (hp), attack, defense, spAttack, spDefense, speed. Tato třída je fyzicky vložena (**kompozice**) do abstraktní třídy `Pokemon`, což zamezuje zahlcení předka zbytečným množstvím proměnných a zlepšuje modularitu.

#### `Pokemon` (čistě abstraktní třída)
Obsahuje společný stav (`name`, `type`, `level`, `isWild`), vložený objekt `Statistics` a definuje čistě virtuální metody (`BattleCry`, `AttackPower`). Dále obsahuje přetíženou metodu `DisplayInfo` (s parametrem i bez něj) pro různou úroveň detailu výpisu. V konstruktoru se každý objekt automaticky hlásí statické třídě `Pokedex`.

#### `BasicPokemon` (odvozená třída)
Dědí z třídy `Pokemon`. Rozšiřuje společný stav o unikátní proměnnou `isShiny` (vzácnější vzhled) a mění chování přepsáním virtuálních metod s využitím pozdní vazby.

#### `LegendaryPokemon` (odvozená třída)
Dědí z třídy `Pokemon`. Rozšiřuje předka o textovou proměnnou `meaning` (význam legendy) a implementuje vlastní bojový pokřik se sílou `meaning` navíc.

#### `MythicalPokemon` (odvozená třída)
Dědí z třídy `Pokemon`. Rozšiřuje stav o číselnou proměnnou `rarity` (vzácnost) a opět definuje vlastní chování pro čistě virtuální metody předka.

#### `Trainer` (správce objektů)
Reprezentuje trenéra, který Pokémony chytá. Obsahuje **polymorfní datovou strukturu** - dynamicky alokované pole pointerů na základní třídu `Pokemon**`. 

Díky **Liskové substitučnímu principu** může trenér do svého batohu chytit jakoukoliv z odvozených tříd (polymorfní přiřazení) a jednotně s nimi komunikovat pomocí metody `TrainerCommand`. Ta díky pozdní vazbě zavolá vždy správný útok konkrétního typu Pokémona. 
Využívá se zde **agregace** - trenér má k Pokémonům přístup a používá je, ale o jejich finální smazání z paměti se stará statický `Pokedex`.
