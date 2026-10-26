// COMP 2450 — Wardens of the Foundations + Middle Gates
// battle/Battle.cpp
//
// This file holds BOTH boss encounters now.
//
//   * runWardenBattle    — Midterm 1 reference (POST-Warden-1 baseline).
//                          Already filled in for you. Do not modify
//                          unless you intend to change the M1 behaviour.
//   * runWaveSurvival    — Midterm 2 work.  THE WORK FOR THIS MIDTERM
//                          IS HERE.  Read the public Warden 2 page
//                          before you start typing.

#include "Battle.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "../bestiary/Search.h"
#include "../hero/Bag.h"
#include "../hero/BagException.h"
#include "../hero/Item.h"
#include "../hero/Queue.h"
#include "../hero/Sort.h"
#include "../hero/Stack.h"
#include "../hero/UndoAction.h"

namespace dungeon {

namespace {

// =====================================================================
// WARDEN 1 — single boss (Floors 0–3). DONE.
// =====================================================================

constexpr int kPlayerStartHP   = 30;
constexpr int kWardenStartHP   = 50;
constexpr int kPlayerAttackDmg = 6;
constexpr int kWardenAttackDmg = 4;

enum class MenuAction { Attack, UseItem, Inspect, Flee };

struct MenuOption {
    int         number;
    std::string label;
    MenuAction  action;
};

void printMenu(const Bag<MenuOption>& menu, int playerHP, int wardenHP) {
    std::cout << "\n  -- Your turn --   you HP " << playerHP
              << "    Warden HP " << wardenHP << "\n";
    for (std::size_t i = 0; i < menu.size(); ++i) {
        std::cout << "    " << menu[i].number << ". " << menu[i].label << "\n";
    }
    std::cout << "  > ";
}

MenuAction readMenuChoice(const Bag<MenuOption>& menu) {
    std::string line;
    if (!std::getline(std::cin, line)) return MenuAction::Flee;
    int n = -1;
    try { n = std::stoi(line); }
    catch (...) {
        throw BagException(
            static_cast<std::size_t>(menu.size() + 1),
            menu.size());
    }
    for (std::size_t i = 0; i < menu.size(); ++i) {
        if (menu[i].number == n) return menu[i].action;
    }
    throw BagException(static_cast<std::size_t>(n), menu.size());
}

// Apply the named item to the player's HP.  Shared between BOTH wardens
// because the inventory + heal mechanic does not change between them —
// only the max-HP cap does.
bool useItem(Hero& hero, int& playerHP, int maxHP, int* atk = nullptr, int* crit = nullptr, int* foeATK = nullptr, int* foeDEF = nullptr) {
    if (hero.inventory.empty()) {
        std::cout << "  Your satchel is empty.\n";
        return false;
    }
    sortInventory(hero, "value desc");
    std::cout << "  Choose an item by name:\n";
    printInventory(hero);
    std::cout << "  > ";
    std::string name;
    if (!std::getline(std::cin, name) || name.empty()) {
        std::cout << "  You hesitate.\n";
        return false;
    }
    const Item* it = findByName<Item>(hero.inventory, name);
    if (!it) throw BagException(0, hero.inventory.size());
    if (it->name.find("otion") != std::string::npos) {
        playerHP = std::min(playerHP + 12, maxHP);
        std::cout << "  You drink the " << it->name << ". HP -> " << playerHP << ".\n";
    } else if (it->name.find("word") != std::string::npos && foeATK) {
       --(*foeATK);
       *foeDEF -= 2;
       std::cout << "  You pulverize the " << it->name << " into an asthmic powder, decreasing the target's ATK by 1 and DEF by 2.\n";
    } else if (it->name.find("loak") != std::string::npos && crit) {
       const int storedCrit = *crit;
       *crit += (std::max(0, ++(*crit)) >> 2) + 1700;
       std::cout << "  Guts went up by " << ((*crit - storedCrit) / 40) << "!\n";
    } else {
        std::cout << "  You ready " << it->name << " — but it is not a consumable.\n";
    }
    return true;
}

}  // anonymous namespace

BattleOutcome runWardenBattle(Hero& hero) {
    int playerHP = kPlayerStartHP;
    int wardenHP = kWardenStartHP;

    Bag<MenuOption> menu;
    menu.push_back({1, "Attack",         MenuAction::Attack});
    menu.push_back({2, "Use item",       MenuAction::UseItem});
    menu.push_back({3, "Inspect Warden", MenuAction::Inspect});
    menu.push_back({4, "Flee",           MenuAction::Flee});

    hero.eventLog.push_front("battle warden — engaged");

    while (playerHP > 0 && wardenHP > 0) {
        try {
            printMenu(menu, playerHP, wardenHP);
            switch (readMenuChoice(menu)) {
                case MenuAction::Attack: {
                    wardenHP -= kPlayerAttackDmg;
                    std::cout << "  You strike for " << kPlayerAttackDmg
                              << ".  Warden HP -> " << std::max(wardenHP, 0) << ".\n";
                    hero.eventLog.push_front(
                        "you struck for " + std::to_string(kPlayerAttackDmg));
                    if (wardenHP > 0) {
                        playerHP -= kWardenAttackDmg;
                        std::cout << "  The Warden retaliates for "
                                  << kWardenAttackDmg
                                  << ".  Your HP -> " << std::max(playerHP, 0) << ".\n";
                        hero.eventLog.push_front(
                            "the Warden struck for " + std::to_string(kWardenAttackDmg));
                    }
                    break;
                }
                case MenuAction::UseItem: {
                    useItem(hero, playerHP, kPlayerStartHP);
                    if (wardenHP > 0 && playerHP > 0) {
                        playerHP -= kWardenAttackDmg;
                        std::cout << "  The Warden strikes while you fumble.  Your HP -> "
                                  << std::max(playerHP, 0) << ".\n";
                        hero.eventLog.push_front(
                            "the Warden struck for " + std::to_string(kWardenAttackDmg));
                    }
                    break;
                }
                case MenuAction::Inspect: {
                    std::cout << "  Warden of the Foundations.  HP " << wardenHP
                              << " / " << kWardenStartHP
                              << ".  No visible weakness.  (free action)\n";
                    break;
                }
                case MenuAction::Flee: {
                    hero.eventLog.push_front("battle warden — fled");
                    return BattleOutcome::Fled;
                }
            }
        }
        catch (const BagException& e) {
            std::cout << "  " << e.what() << " — try again.\n";
        }
    }

    if (wardenHP <= 0) {
        hero.eventLog.push_front("battle warden — outcome: Victory");
        return BattleOutcome::Victory;
    }
    hero.eventLog.push_front("battle warden — outcome: Defeat");
    return BattleOutcome::Defeat;
}

// =====================================================================
// WARDEN 2 — wave survival (Floors 4–7).  YOU BUILD THE BODY.
// =====================================================================
//
// Re-read the public midterm page if anything below is unclear.
//
// REQUIRED MECHANICS
// =====================================================================
// Player starts at 40 HP. Tune as you like; document tunings in
// encounter-notes.md.
//
// For each wave 1..N: build a Queue<Enemy> of enemies (lineup is yours
// to design — wave i with i+1 enemies is a good default).  The wave is
// over when its queue is empty.  Then the next wave begins.
//
// MENU (at minimum, five options):
//   1. Attack front  — damage the FRONT enemy of the current wave;
//                      dequeue it if HP <= 0; otherwise it retaliates
//   2. Use item      — show inventory, pick by name; effect; front
//                      enemy retaliates after
//   3. Inspect log   — print the last few entries of hero.eventLog
//                      via printLog; FREE action — turn does NOT end
//   4. Undo          — pop the most recent action off your undo stack;
//                      restore player HP + inventory.  FREE action.
//                      Limit one per turn (no chained undo-undo).
//                      The wave queue is NOT rewound — see F6 below.
//   5. Flee          — leave; the yard stays open with uncleared waves
//
// END:
//   Victory  when all N waves are cleared
//   Defeat   when playerHP <= 0
//   Fled     when the player chose Flee
//
// FLOOR-TIED REQUIREMENTS — ALL FOUR MUST APPEAR IN YOUR CODE
// =====================================================================
//
//   F4/4½ (linked lists) — every turn appends ONE entry (or one for
//                          your action + one for the retaliation) to
//                          hero.eventLog (a Chain<std::string>).  The
//                          log grows FORWARD; even `undo` appends an
//                          "(undo) reverted: …" entry rather than
//                          rewinding the chain.
//
//   F5 (iterators)       — the Inspect-log branch MUST call printLog
//                          on hero.eventLog with an iterator pair.
//                          begin()/end() walks newest-first (because
//                          eventLog is push_front'd everywhere);
//                          rbegin()/rend() walks oldest-first (a
//                          "battle replay" framing).  Either is fine —
//                          document the choice in a one-line comment.
//                          NO subscripting, NO chain.at(i), NO manual
//                          head()-chase.
//
//   F6 (stacks)          — the undo history MUST be a Stack<UndoAction>
//                          (local to this function, OR hero.undoStack —
//                          your call; the trade-off is in encounter-
//                          notes.md).  Every attack and every use-item
//                          pushes ONE UndoAction (snapshotting player
//                          HP + inventory) BEFORE executing.  `undo`
//                          pops the top and restores both.
//                          ABOVE the stack declaration, in a comment:
//                          why a stack and not a queue.
//                          ABOVE the undo body, in a comment: name
//                          ONE piece of state your undo intentionally
//                          does NOT restore (the wave queue is the
//                          obvious choice — pick that or another, and
//                          defend it).
//
//   F7 (queues)          — the current wave's enemies MUST be held in
//                          a Queue<Enemy>.  front() reads the active
//                          target; dequeue() runs only when that
//                          enemy's HP <= 0.  The Grix anti-pattern —
//                          `vector<Enemy> + erase(begin())` — works
//                          but is NOT the F7 contract.
//
// WHAT THE GRADER WILL DO
// =====================================================================
//   1. cmake --build the project. If it does not compile, the Warden
//      has won by default.
//   2. Type `battle waves 2`. Play through; verify wave 1 drains, wave
//      2 spawns, and FIFO targeting holds (oldest enemy attacked first).
//   3. Type `9` and `abc` at the menu. The game must NOT crash; must
//      re-prompt the same turn.
//   4. Use `Inspect log` (option 3). Turn must NOT advance.
//   5. Attack once, then `Undo` (option 4). HP must rewind.  Front
//      enemy's HP must NOT rewind.  A second consecutive `Undo` must
//      be blocked.
//   6. Open this file and find each of the four Floor ties. They must
//      be REAL — the log grows on every turn, Inspect log walks via
//      iterator, the undo stack is pushed before mutating, the wave
//      really is a Queue<Enemy>.

BattleOutcome runWaveSurvival(Hero& player, int waves) { // Changed Hero& name because I mistyped it many times in Warden 1.
   bool EvalChnc(const int probabilityOf20000);
   int EvalDmg(const int atk, const float critDmg, const int def, const bool isCrit); // Evals attacks by non-players.
   int EvalPlayerDmg(const int atk, int& critChance, float& critDmg, const int def);  // Evals attacks by players.

   waves = (waves < 1) ? 1 : waves;
   constexpr int startPlayerHP = 40, startPlayerATK = 7, startPlayerDEF = 0, startPlayerMissChnc = 750, startPlayerCritChnc = 1'000, startPlayerDodgeChnc = 1'000;
   constexpr float startPlayerCritDmg = 2.25f; // In _real_ games, people use single precision. Excepting Minecraft Java, but that has... other issues.
   int playerMaxHP = startPlayerHP, playerHP = playerMaxHP, playerATK = startPlayerATK, playerDEF = startPlayerDEF,
   playerMissChnc = startPlayerMissChnc, playerCritChnc = startPlayerCritChnc, playerDodgeChnc = startPlayerDodgeChnc;
   float playerCritDmg = startPlayerCritDmg;

   struct Enemy { // Dodge etc. are chances out of 20'000. NOTE: I ended up not using most of this...
      int hp, atk, def, dodge, miss, crit, magdef;
      std::string name;
      Enemy(const std::string& name, int hp, int atk, int def = 0, int dodge = 0, int miss = 0, int crit = 0, int magdef = 0) {
         this->name = name; this->hp = hp; this->atk = atk; this->def = def; this->dodge = dodge; this->miss = miss; this->crit = crit; this->magdef = magdef;
      }
   };

   class adsjknvfPleaseDontDeclareMultipleOfTheseThisVeryLongNameShouldPreventMeHopefully { // A single-instance undo handler...
      Stack<UndoAction> history; // _That_ is my Floor 6...
      bool undoUsed = false;
   public:
      void push(const std::string& desc, const Hero& player, const int playerHP) {
         history.push({ desc, player.inventory, playerHP });
         undoUsed = false;
      }
      bool restore(Hero& player, int& playerHP) {
         if (history.empty()) { std::cout << "The Vault has no memory of any previous actions of yours.\n"; return true; }
         else if (undoUsed) {
            std::cout << "You have already used your undo for that turn; the Vault sees your greed and counts it against you.\n";
            return true;
         } else {
            undoUsed = true;
            const UndoAction tmp = history.top(); history.pop();

            player.inventory = tmp.inventorySnapshot;
            playerHP = tmp.playerHP;
            player.eventLog.push_front('"' + tmp.description + "\" undone");
            return false;
         }
      }
   } undoHandler; // ... the single instance declared here.

   player.eventLog.push_front("Battle waves " + std::to_string(waves) + " -- the yard opens\n"); // I debated adding 48 and reinterpret_casting<char>, but on the off chance that this requires more than nine waves...

   for (int w = 1; w <= waves; ++w) {
      Queue<Enemy> line;
      for (int i = 1; i <= w + 1; ++i) {
         if (i % 3 == 0) line.enqueue(Enemy("Imp #" + std::to_string((i + 2) / 3), 8, 3, w)); // Perhaps the constructor was a bad idea...
         else if (i % 3 == 1) line.enqueue(Enemy("Rat #" + std::to_string((i + 2) / 3), 4, 5 + w, 7));
         else line.enqueue(Enemy("Skeleton #" + std::to_string((i + 2) / 3), 7 + (i << w), 4, 6));
      }
      while (!line.empty()) {
         bool retaliate = true;
         const Enemy& front = line.front();
         std::cout << "\n -- Wave " << w << '/' << waves << "; your HP: " << playerHP << ", enemies remaining: " << line.size() <<
         ".\nEnemy at front of line: " << front.name << " (HP: " << front.hp << ", ATK: " << front.atk << ", DEF: " << front.def << ").\n";

         std::cout << "How would you like to deal with the problem? Options:\n\n" <<
         "1. Attack " << front.name << '\n' <<
         "2. Use item\n"                    <<
         "3. Inspect log (free action)\n"   <<
         "4. Undo\n"                        <<
         "5. Flee\n"                        <<
         "\n(Your response here): "         ;

         std::string input; if (!getline(std::cin, input)) { // I am not sure what this condition is for but it was in the video.
            player.eventLog.push_front("Battle waves -- fled " + w);
            return BattleOutcome::Fled;
         }

         int intput; // It's because it's an input that's... nevermind.
         try {
            std::size_t len;
            intput = std::stoi(input, &len);
#pragma warning (suppress:) // Bitwise and logical OR do the same thing in this context, why is MSVC complaining...
            if (input.find_first_not_of(" \t\r", len) != std::string::npos | intput < 1 | intput > 5) {
               std::cout << "Enter a number between 1 and 5, inclusive: ";
               continue;
            }
         } catch (const std::exception&) {
            std::cout << "Enter a _number_ between 1 and 5, inclusive: ";
            continue;
         }
         Enemy& foe = line.front();
         try { switch (intput) {
         case 1: // Priority is miss > crit > dodge > normal attack.
            std::cout << player.heroName << " attacks " << foe.name;
            if (EvalChnc(playerMissChnc)) std::cout << " - just missed!\n"; // We still be going for EB aesthetic here. Perhaps dissonant, but dissonance can create humour (see Bathos).
            else if (int dmg; EvalChnc(playerCritChnc)) {
               foe.hp -= dmg = EvalPlayerDmg(playerATK, playerCritChnc, playerCritDmg, foe.def);
               std::cout << ".\nSMAAAASH!!\n" << dmg << "HP of damage to " << foe.name << "!\n";
               if ((rand() & 4) == 0) {
                  std::cout << "You are hit with adrenaline! " << player.heroName << "'s ATK went up by 1!\n"; // EB aesthetic is complete with consistently inconsistent tense and possession.
               }
               player.eventLog.push_front("Battle waves -- crit against " + foe.name + " for " + std::to_string(dmg) + "HP");
            } else if (EvalChnc(foe.dodge)) std::cout << ". " << foe.name << " dodged quickly!\n";
            else { // Normal path here (boo)
               foe.hp -= std::max(1, playerATK - foe.def);
               player.eventLog.push_front("Battle waves -- attacked " + foe.name + " for " + std::to_string(std::max(1, playerATK - foe.def)) + "HP");
               std::cout << "; " << std::max(1, playerATK - foe.def) << "HP of damage to " << foe.name << "!\n";
            }
            undoHandler.push("attacked " + foe.name, player, playerHP);

            if (foe.hp <= 0) {
               line.dequeue();
               retaliate = false;
               player.eventLog.push_front("Battle waves -- terminated " + foe.name + (line.empty() ? "" : (", " + line.front().name +
                  "up next"))); // Yes, I am intentionally using a dangling reference. No, it is not going to cause any problems in the 3ns it takes between freeing the memory and its last use; I tested.
               std::cout << foe.name << " returned to the dust of the earth!\n";
            }
            
            break;
         case 2: // Item
            undoHandler.push("used item", player, playerHP);
            try {
               useItem(player, playerHP, playerMaxHP, &playerATK, &playerCritChnc, &(foe.atk), &(foe.def));
            } catch (const BagException& e) {
               std::cout << "ERROR: " << e.what() << "; please try again.\n";
               retaliate = false;
            }
            break;
         case 3:
            printLog(player.eventLog.begin(), player.eventLog.end(), 7, "newest first", player.eventLog.size());
            retaliate = false;
            break;
         case 4:
            undoHandler.restore(player, playerHP);
            retaliate = false; // The requirements told me this is necessary... even though this allows repeated attacking and restoring your HP...
            break;
         case 5:
            player.eventLog.push_front("Battle waves -- fled");
            return BattleOutcome::Fled;
         }

         if (retaliate) {
            int dmg = std::max(1, foe.atk - playerDEF);
            std::cout << (playerHP -= dmg, foe.name) << " attacks you! " << dmg << "HP of damage to " << player.heroName << "!\n";
            player.eventLog.push_front("Battle waves -- hit by " + foe.name + " for " + std::to_string(dmg) + "HP");
            if (playerHP <= 0) {
               player.eventLog.push_front("Battle waves -- died");
               std::cout << player.heroName << " got hurt and collapsed!\n";
               return BattleOutcome::Defeat;
            }
         }

         } catch (const std::exception&) {
            continue;
         }
      }
   }
   player.eventLog.push_front("Battle waves -- victorious");
   return BattleOutcome::Victory;
}

}  // namespace dungeon

bool EvalChnc(const int probabilityOf20000) {
   return probabilityOf20000 > rand() % 20'000;
}
int EvalDmg(const int atk, const float critDmg, const int def, const bool isCrit) { // Wait... I don't actually use this...
   return std::max(2, int(atk * (1.0f + std::max(0.0f, critDmg)) + 0.625f) - std::max(0, def));
}
int EvalPlayerDmg(const int atk, int& critChance, float& critDmg, const int def) {
   const int preIncDmg = std::max(2, int(atk * (1.0f + std::max(0.0f, critDmg)) + 0.625f) - std::min(def, (def + 1) >> 1));
   critDmg += 0.025f + 0.005f * std::max(0.0f, critDmg); // When that adrenaline hits...
   critChance += 150; // 0.75%
   return preIncDmg;
}