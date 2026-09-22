// COMP 2450 — The Descent
// Floor 4 — The Chain Vault — starter
//
// This is the post-Warden reference codebase plus this week's Floor-4
// stubs. Everything Floors 0–3 + the Warden midterm delivered is in
// place: Bag<T>, BagException + at(), findByName<T>, mergeSort,
// quicksort, std::sort, the boss battle.
//
// What is NEW for Floor 4:
//   * hero/Chain.h        — the singly-linked container template (stubs).
//   * Hero owns a Chain<std::string> eventLog (member added in Hero.h).
//   * EVERY command branch in this file calls hero.eventLog.push_front(...)
//     with a short description of what just happened. The chain is
//     being WRITTEN the moment the program starts. It is just not being
//     READ until you implement the `log` command's renderer (printLog).
//   * `log [n]`          — shows the last n entries, newest first.
//   * `selftest chain`   — runs the leak-check harness (ChainTests.cpp).
//   * `benchmark log [N]` — Chain::push_front vs std::vector::insert(begin).
//
// What you do this week (see the Floor 4 page):
//   Mon — push_front, size, head() in Chain.h; printLog in Hero.cpp.
//   Wed — ~Chain() and clear() in Chain.h; verify with `selftest chain`.
//   Fri — `= delete` the copy operations; capture the compiler error.

#include <iostream>
#include <sstream>
#include <string>

#include "battle/Battle.h"
#include "bestiary/Bench.h"
#include "bestiary/Bestiary.h"
#include "bestiary/Search.h"
#include "hero/Bench.h"
#include "hero/ChainBench.h"
#include "hero/ChainTests.h"
#include "hero/Hero.h"
#include "hero/Sort.h"

using namespace dungeon;

namespace {

void splitFirst(const std::string& line, std::string& cmd, std::string& rest) {
    auto sp = line.find(' ');
    if (sp == std::string::npos) { cmd = line; rest.clear(); }
    else { cmd = line.substr(0, sp); rest = line.substr(sp + 1); }
}

void printHelp() {
    std::cout << "(commands:\n"
              << "   search <name>                 — look up by name in bestiary or inventory\n"
              << "   list                          — list the bestiary\n"
              << "   inventory                     — list your inventory\n"
              << "   inspect <n>                   — show the nth item in your inventory\n"
              << "   sort inventory by <key> [asc|desc]\n"
              << "                                 — key is name, weight, or value\n"
              << "   log [n]                       — show the last n events, newest first\n"
              << "   selftest chain                — run the Chain<T> leak-check harness\n"
              << "   benchmark [N]                 — race the search algorithms\n"
              << "   benchmark sort [N] [--sorted] [--bad-pivot]\n"
              << "                                 — race the sorting algorithms\n"
              << "   benchmark log [N]             — race Chain::push_front vs vector insert(begin)\n"
              << "   battle warden                 — face the Warden of the Foundations\n"
              << "   help                          — this screen\n"
              << "   quit                          — leave the dungeon)\n";
}

}  // anonymous namespace

int main() {
//*/Chain<int> a; a.push_front(1); Chain<int> b = a;
    std::cout << "\n=== THE CHAIN VAULT ===\n\n";

    std::cout << "What is your name, adventurer? ";
    Hero hero;
    std::getline(std::cin, hero.heroName);
    if (hero.heroName.empty()) hero.heroName = "Nameless One";

    std::cout << "\nWelcome, " << hero.heroName << ".\n"
              << "The Warden's gate stands open behind you. Mavren waits at the chain.\n"
              << "Type `log` at any time to see what the chain remembers.\n\n";

    Bag<Monster> bestiary = loadBestiary("data/monsters.txt");
    if (bestiary.empty()) {
        std::cerr << "The Bestiary tablet is blank. Cannot proceed.\n";
        return 1;
    }
    sortBestiary(bestiary);

    hero.inventory = loadInventory("data/starter-inventory.txt");
    if (hero.inventory.empty()) {
        std::cerr << "Your satchel is empty. Cannot proceed.\n";
        return 1;
    }

    // First entry in the chain. Once push_front works, this is what
    // shows up at the bottom of `log`.
    hero.eventLog.push_front("began session as \"" + hero.heroName + "\"");

    printHelp();
    std::cout << "\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        try {
            std::string cmd, rest;
            splitFirst(line, cmd, rest);

            if (cmd == "quit" || cmd == "exit") {
                std::cout << "The forge cools. The chain dissolves link by link.\n";
                break;
            }
            else if (cmd == "help") {
                printHelp();
            }
            else if (cmd == "list") {
                for (const auto& m : bestiary) printMonster(m);
                hero.eventLog.push_front("list — bestiary printed");
            }
            else if (cmd == "search") {
                if (rest.empty()) {
                    std::cout << "Speak a name:  search <monster-or-item>\n";
                    continue;
                }
                if (const Monster* m = findByName<Monster>(bestiary, rest)) {
                    printMonster(*m);
                    hero.eventLog.push_front("search " + rest + " — found in bestiary");
                    continue;
                }
                if (const Item* it = findByName<Item>(hero.inventory, rest)) {
                    std::cout << "  " << it->name
                              << "  (wt " << it->weight
                              << ", val " << it->value << ")\n";
                    hero.eventLog.push_front("search " + rest + " — found in inventory");
                    continue;
                }
                std::cout << "No such creature or item under that name.\n";
                hero.eventLog.push_front("search " + rest + " — not found");
            }
            else if (cmd == "inventory") {
                printInventory(hero);
                hero.eventLog.push_front(
                    "inventory — listed " + std::to_string(hero.inventory.size()) + " items");
            }
            else if (cmd == "inspect") {
                if (rest.empty()) {
                    std::cout << "Usage: inspect <n>\n";
                    continue;
                }
                std::size_t n;
                try { n = std::stoull(rest); }
                catch (...) {
                    std::cout << "Usage: inspect <n>\n";
                    continue;
                }
                if (n == 0) {
                    std::cout << "Items are numbered from 1, adventurer.\n";
                    continue;
                }
                const Item& it = hero.inventory.at(n - 1);
                std::cout << "  " << it.name
                          << "  (wt " << it.weight
                          << ", val " << it.value << ")\n";
                hero.eventLog.push_front("inspect " + rest + " — ok");
            }
            else if (cmd == "sort") {
                std::istringstream in(rest);
                std::string what, by, key, dir;
                in >> what >> by >> key >> dir;
                if (what != "inventory" || by != "by" || key.empty()) {
                    std::cout << "Usage: sort inventory by <name|weight|value> [asc|desc]\n";
                    continue;
                }
                std::string criterion = key;
                if (!dir.empty()) criterion += " " + dir;
                if (!sortInventory(hero, criterion)) {
                    std::cout << "Unknown sort key. Try: name, weight, or value.\n";
                    continue;
                }
                printInventory(hero);
                hero.eventLog.push_front("sort inventory by " + criterion);
            }
            else if (cmd == "log") {
                std::size_t n = 10;
                if (!rest.empty()) {
                    try { n = std::stoull(rest); }
                    catch (...) {
                        std::cout << "Usage: log [n]\n";
                        continue;
                    }
                }
                printLog(hero, n);
                // We do NOT log the `log` command itself, otherwise the
                // top entry of every `log` call would be the call itself
                // and the lesson on push_front O(1) would be muddied.
            }
            else if (cmd == "selftest") {
                if (rest != "chain") {
                    std::cout << "Usage: selftest chain\n";
                    continue;
                }
                runChainSelfTest();
            }
            else if (cmd == "benchmark") {
                std::istringstream in(rest);
                std::string first;
                in >> first;
                if (first == "sort") {
                    SortBenchOptions opts;
                    std::size_t n = 0;
                    std::string tok;
                    while (in >> tok) {
                        if      (tok == "--sorted")    opts.presorted = true;
                        else if (tok == "--bad-pivot") opts.badPivot  = true;
                        else {
                            try { n = std::stoull(tok); }
                            catch (...) {
                                std::cout << "Usage: benchmark sort [N] [--sorted] [--bad-pivot]\n";
                                n = static_cast<std::size_t>(-1);
                                break;
                            }
                        }
                    }
                    if (n == static_cast<std::size_t>(-1)) continue;
                    if (n == 0) runSortBenchmarkSweep(opts);
                    else        runSortBenchmark(n, opts);
                }
                else if (first == "log") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark log [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runChainBenchmarkSweep();
                    else        runChainBenchmark(n);
                }
                else if (first.empty()) {
                    runBenchmarkSweep();
                }
                else {
                    try {
                        runBenchmark(std::stoull(first));
                    } catch (const std::exception&) {
                        std::cout << "Usage: benchmark [N]   or   benchmark sort [N] [flags]\n";
                    }
                }
            }
            else if (cmd == "battle") {
                if (rest != "warden") {
                    std::cout << "Usage: battle warden\n";
                    continue;
                }
                BattleOutcome outcome = runWardenBattle(hero);
                switch (outcome) {
                    case BattleOutcome::Victory:
                        std::cout << "\nThe Warden falls. The gate opens.\n"; break;
                    case BattleOutcome::Defeat:
                        std::cout << "\nYou fall. The gate stays shut.\n"; break;
                    case BattleOutcome::Fled:
                        std::cout << "\nYou step back from the gate. It does not chase.\n"; break;
                }
            }
            else {
                std::cout << "The Vault does not understand '" << cmd << "'.\n";
            }
        }
        catch (const BagException& e) {
            std::cout << "No such item. (" << e.what() << ")\n";
            hero.eventLog.push_front(std::string("error: ") + e.what());
        }
        catch (const std::exception& e) {
            std::cout << "Something went wrong: " << e.what() << "\n";
            hero.eventLog.push_front(std::string("error: ") + e.what());
        }
    }

    // When main exits, `hero` goes out of scope. ~Hero() runs, which
    // runs ~Chain<std::string>() on hero.eventLog. By Wednesday, that
    // destructor walks the chain and frees every node. Watch the
    // `selftest chain` output to confirm.
    return 0;
}
