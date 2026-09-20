// Battle.h contains the declarations associated with this implementation,
// including Hero, BattleOutcome, BattleException, and runWardenBattle().
#include "Battle.h"

// Standard-library headers used in this file.
#include <algorithm>  // std::min and std::max
#include <iostream>   // std::cout and std::cin
#include <string>     // std::string, std::getline, std::stoi

// Project-specific headers from neighboring directories.
#include "../bestiary/Search.h"     // findByName<T>()
#include "../hero/Bag.h"            // Bag<T>
#include "../hero/BagException.h"   // BagException
#include "../hero/Item.h"           // Item
#include "../hero/Sort.h"           // sortInventory()

namespace dungeon {


    namespace {
        // constexpr means these values are compile-time constants
        // k prefix --> means constant
        int kPlayerStartHP = 30;
        constexpr int kWardenStartHP = 50;
        int kPlayerAttackDmg = 6;
        int kWardenAttackDmg = 4;

        // enum creates a set of named choices
        // enum class, keeps our names scoped
        enum class MenuAction {
            Attack,
            UseItem,
            Inspect,
            Flee
        };
        // {1, "Attack", MenuAction::Attack}

        struct MenuOption {
            int number; // number typed by the player
            std::string label; // text displayed by the menu
            MenuAction action; // action performed by our program
        };

        void printMenu(
            Bag<MenuOption>& menu,
            int playerHP,
            int wardenHP
        ) {
            std::cout << "\n -- Your turn -- your HP: " << playerHP
                << ";    Warden's HP: " << wardenHP << "\n";

            for (std::size_t i = 0; i < menu.size(); ++i) {
                // bag overload our operator[], allow menu[i]
                // to retrieve our menuoption at index i
                std::cout << "     "
                    << menu[i].number
                    << ". "
                    << menu[i].label
                    << "\n";
            }
            std::cout << " > ";
        }
        // read the user's input and convert that into a MenuAction
        MenuAction readMenuChoice(const Bag<MenuOption>& menu) {
            std::string line;
            // getline will get the entire line up to the enter key
            // if getline fails, standard input may have been closed
            // if problem... flee!
            if (!std::getline(std::cin, line)) {
                return MenuAction::Flee;
            }

            int n = -1;

            try {
                // "2" --> 2
                // if the string can't be converted...
                // stoi throw an excp
                n = std::stoi(line);
            }
            catch (...) {
                // catch any exception type
                // we will replace our low-level stoi
                // exception with a domain-specific
                // BattleException
                throw BattleException(
                    "'" + line + "' is not a menu number (enter 1 to "
                    + std::to_string(menu.size()) + ")"
                );
            }
            // search our menu for an option whose displayed number
            // matches the number entered by the player
            for (std::size_t i = 0; i < menu.size(); ++i) {
                if (menu[i].number == n) {
                    // return associated matching option
                    return menu[i].action;
                }
            }

            // the input was numeric but it did not match a menu option
            throw BagException(
                static_cast<std::size_t>(n),
                menu.size()
            );
        }

        // handle the player's "use item" action
        // Hero& will give the function access to the
        // original hero object instead of a copy
        void useItem(Hero& hero, int& playerHP) {
            // handle empty inventory case
            if (hero.inventory.empty()) {
                std::cout << "Your satchel is empty.\n";
                return;
            }
            // sort hero's inventory from highest to lowest
            sortInventory(hero, "value desc");

            std::cout << "Choose an item by name:\n";
            printInventory(hero);
            std::cout << " > ";

            std::string name;

            // || short-circuit
            // 1. try to read the line
            // 2. if that succeeds, then i will check whether the
            // line is empty
            // if either condition is true, the player does nothing
            if (!std::getline(std::cin, name) || name.empty()) {
                std::cout << "you hesitated.\n";
                return;
            }

            // findByName<Item> <-- function-template specialization
            // <Item> will tell the compiler this search will operate
            // on Item objects
            const Item* it = findByName<Item>(hero.inventory, name);

            // a nullptr converts to false
            if (!it) {
                throw BattleException(
                    "no item found '" + name + "' in your satchel"
                );
            }

            // if you say Potion, Healing potion
            if (it->name.find("otion") != std::string::npos) {
                // heal 12 hp, but we also don't need our healing
                // to exceed our max health
                playerHP = std::min(
                    playerHP + 12,
                    ++kPlayerStartHP + 5 // Some overheal and gradual capacity enhancement may be desirable; otherwise, healing by 40% of your health every turn is a bit janky...
                );
                std::cout << " You drink "
                    << it->name
                    << ". HP -> "
                    << playerHP
                    << ".\n";
            }
            else if (it->name.find("sword") != std::string::npos) {
                std::cout << "You transfer Tetanus to your primary weapon using your Rusty sword! " << hero.heroName << "'s ATK increases by 3!\n";
                kPlayerAttackDmg += 3;
            }
            else {
                std::cout << " You ready "
                    << it->name
                    << " - but it is not a consumable.\n";
            }
        }
    }


    BattleOutcome runWardenBattle(Hero& hero) {
        int qtyWardens = 0;
        int wormholeTicks = 0;
        // create two variables for the player and
        // warden health; represent the state
        int playerHP = kPlayerStartHP;
        int wardenHP = kWardenStartHP;

        // create a bag specialized to store
        // menuoption objects
        Bag<MenuOption> menu;
        menu.push_back({ 1, "Attack", MenuAction::Attack });
        menu.push_back({ 2, "Use item", MenuAction::UseItem });
        menu.push_back({ 3, "Inspect warden", MenuAction::Inspect });
        menu.push_back({ 4, "Flee", MenuAction::Flee });

        // continue the battle ONLY while both participants
        // are alive
        while (playerHP > 0 && wardenHP > 0) {
            try {
                printMenu(menu, playerHP, wardenHP);
                bool wardenATK = true;

                // readMenuChoice returns a MenuAction
                // switch statement to select the
                // corresp block of code
                srand(rand() ^ time(0));
                switch (readMenuChoice(menu)) {
                case MenuAction::Attack:
                    // subtract player's damage from warden's hp
                    wardenHP -= kPlayerAttackDmg;

                    if (!(rand() & 3)) {
                        std::cout << "CRITICAL HIT!\n";
                        wardenHP -= ++kPlayerAttackDmg + (rand() & 7); // Improved motivation improves performance (reason for ++).
                    }
                    else {
                        std::cout << "You strike for " << kPlayerAttackDmg << ". ";
                    }
                    std::cout << "Warden's HP -> " << std::max(wardenHP, 0) << ".\n";
                    if (wardenHP <= 0) return BattleOutcome::Victory; // In testing I lost the battle by having the Warden kill me after I already killed him (on the same turn), so this should prevent that.
                    break;
                case MenuAction::Flee:
                    std::cout << hero.heroName << " tried to get away... and did!"; // Did you get the reference? Probably not, EB run chances are _abysmal_...
                    return BattleOutcome::Fled; // Here we just hardcode 100%... the pendulum perhaps swung too far, but oh, well.
                case MenuAction::UseItem: // No break necessary after a return.
                    useItem(hero, playerHP);
                    break;
                case MenuAction::Inspect:
                    std::cout << "The Warden is an extremly wrathful being; once a human, yet now distorted beyond hope of recognition by MSVC function parsing bugs and cryptic LINK errors, he began to practice the dark arts - but deep down inside, to this day, he is really compensating out of his bitterness; for even after all of his other accomplishments, he could never master MSVC.\n";
                    wardenATK = false;
                }    if (wardenATK) {
                    for (int i = 0; i <= qtyWardens; ++i) {
                        std::cout << "The Warden "; if (i) std::cout << "#" << i + 1 << ' ';
                        switch (rand() % 28) {
                        case 0:
                        case 16:
                            std::cout << "shouts taunts; " << hero.heroName << "'s ATK decreases by 6!\n";
                            kPlayerAttackDmg -= 3;
                            --playerHP;
                            if (kPlayerAttackDmg < 0) kPlayerAttackDmg = 0;
                            break;
                        case 1:
                        case 2:
                            std::cout << "impales your face! CRITICAL HIT! ";
                            playerHP -= kWardenAttackDmg << 1;
                            break;
                        case 4:
                            std::cout << "slits your throat with a glowing sickle! INSTANT DEATH!";
                            return BattleOutcome::Defeat;
                        case 5:
                        case 6:
                            std::cout << "tosses a mysterious purple vial! Warden's HP -> " << wardenHP - ((kWardenAttackDmg + 1) >> 1) << "; ";
                            playerHP -= kWardenAttackDmg << 1;
                            wardenHP -= (kWardenAttackDmg + 1) >> 1;
                            break;
                        case 7:
                        case 8:
                            std::cout << "syphons your blood! Warden's HP -> " << wardenHP + kWardenAttackDmg << "; ";
                            playerHP -= kWardenAttackDmg;
                            wardenHP += kWardenAttackDmg;
                            break;
                        case 9:
                            std::cout << "steals half of your soul! Warden's HP -> " << wardenHP + ((playerHP + 1) >> 1) << ", ATK -> " << kWardenAttackDmg + (kPlayerAttackDmg >> 1) << "; " << hero.heroName << "'s ATK -> " << ((kPlayerAttackDmg + 1) >> 1) << ".\n";
                            wardenHP += (playerHP + 1) >> 1; playerHP -= (playerHP + 1) >> 1;
                            kWardenAttackDmg += kPlayerAttackDmg >> 1; kPlayerAttackDmg -= kPlayerAttackDmg >> 1;
                            break;
                        case 10:
                        case 11:
                            wardenHP += 10 + (rand() & 7) + (rand() & 7);
                            std::cout << "drinks a _stiff_ healing potion! Warden's HP -> " << wardenHP << "; ";
                            break;
                        case 12:
                        case 13:
                            std::cout << "casts an ominous incantation! ";
                            wardenHP += 8 + (rand() & 3) + (rand() & 3);
                            kWardenAttackDmg += 5 + (rand() & 3) + (rand() & 1);
                            break;
                        case 14:
                            std::cout << "summons a clone of himself! ";
                            ++qtyWardens;
                            break;
                        case 15:
                            std::cout << "summons a wormhole to the interior of your cloak! ";
                            if (!hero.inventory.empty()) {
                                std::cout << "All of your items are stolen! ";
                                wormholeTicks -= 4; // A shorter sentence is appropriate for the rather nasty co-occurrence of having all items stolen.
                            }
                            hero.inventory.clear();
                            wormholeTicks += 7 + (rand() & 3); // We want a longer sentence if the items are already stolen.
                            break;
                        default:
                            std::cout << "strikes your torso. ";
                            playerHP -= kWardenAttackDmg;
                            break;
                        }
                        std::cout << hero.heroName << "'s HP -> " << (playerHP = std::max(playerHP, 0)) << ".\n";
                    } if (wormholeTicks > 0) {
                        std::cout << "The wormhole in your cloak eats away at your flesh! ";
                        --wormholeTicks;
                        playerHP -= 5 + (rand() & 3) + (rand() & 1) + ((15 + (rand() & 7) * (wormholeTicks / 7)));
                        std::cout << hero.heroName << "'s HP -> " << std::max(playerHP, 0) << ".\n";
                        if (playerHP <= 0) return BattleOutcome::Defeat;
                        if (!wormholeTicks) std::cout << "The wormhole in your cloak evaporates.\n";
                    }
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "; please try again.";
            }
        }
        return BattleOutcome::Fled;
    }

}  // End namespace dungeon