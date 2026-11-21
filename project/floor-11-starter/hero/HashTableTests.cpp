// COMP 2450 — Floor 11 starter
// hero/HashTableTests.cpp — the `selftest whispers` harness. Provided. Do not edit.
//
// Six phases over small FIXED inline samples, built from in-test Monsters so the
// expected values are pinned and never depend on data/monsters.txt (exactly as
// BSTTests builds {5,3,8,2,4,7,9} and RegistryTests builds its five-monster bag).
//
// Testing a hash table deterministically — the design rule (read before editing
// anything, though you should not edit this file): the BUCKET an item lands in
// depends on the hash function the student writes, so this test must NOT assert
// exact bucket indices (that would pin the test to one specific hash). Instead it
// asserts the OBSERVABLE, hash-independent contract: items are findable, counts
// are right, duplicates do not double-count, erase removes only the one item, and
// rehash preserves every item while raising the bucket count. Phase 1 is the one
// place a hash value is touched, and it checks a PROPERTY of hashKey/bucketIndex
// (deterministic, distinct, in range) — not a magic number.
//
// Where collisions must be guaranteed regardless of the student's hash (phases 4
// and 6), the test forces them by using a TINY bucket count (1 or 2) so every
// item necessarily shares buckets — pigeonhole, not luck.
//
// Fixed sample A (do NOT change — phases depend on it). The Floor 10 set, reused
// for cross-floor consistency:
//     { "Goblin",   8, 2, "fire"  }
//     { "Wraith",  14, 4, "holy"  }
//     { "Skeleton",10, 3, "blunt" }
//     { "Lich",    35, 6, "holy"  }
//     { "Wisp",     3, 1, "holy"  }
//
// MWF: Monday's hashKey + bucketIndex + basic insert green phase 1;
// Wednesday's no-dup insert + find + erase green phases 2–5; Friday's
// rehashIfNeeded greens phase 6.

#include "HashTableTests.h"

#include <iostream>

#include "Bag.h"
#include "HashTable.h"
#include "../bestiary/Monster.h"

namespace dungeon {

namespace {

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

// Build the fixed five-monster sample bag, in this exact order.
Bag<Monster> buildSampleBag() {
    Bag<Monster> b;
    b.push_back({"Goblin",    8, 2, "fire"});
    b.push_back({"Wraith",   14, 4, "holy"});
    b.push_back({"Skeleton", 10, 3, "blunt"});
    b.push_back({"Lich",     35, 6, "holy"});
    b.push_back({"Wisp",      3, 1, "holy"});
    return b;
}

}  // anonymous namespace

bool runHashTableSelfTest() {
    bool all = true;

    // --- Phase 1: hashKey / bucketIndex are deterministic, distinct, in range ---
    // No magic numbers: we assert PROPERTIES a sane hash must have. Same input
    // gives the same code (deterministic); different inputs give different codes
    // (a hash that scatters); and bucketIndex always lands inside [0, bucketCount).
    {
        HashTable t;   // default bucket count, empty
        bool deterministic = (t.hashKey("Lich") == t.hashKey("Lich"));
        bool distinct      = (t.hashKey("Lich") != t.hashKey("Wisp"));
        bool inRange = true;
        for (const char* n : {"Goblin", "Wraith", "Skeleton", "Lich", "Wisp"}) {
            if (t.bucketIndex(n) >= t.bucketCount()) inRange = false;
        }
        const bool ok = deterministic && distinct && inRange;
        result("hashKey/bucketIndex: deterministic, distinct, in range", ok,
               "hashKey must be a deterministic function of the string (multiplicative hash); "
               "bucketIndex must reduce mod bucketCount and stay in range");
        all = all && ok;
    }

    // --- Phase 2: insert places, find retrieves, count is right ---
    {
        HashTable t(buildSampleBag());
        bool ok = (t.size() == 5);
        if (ok) {
            const Monster* w = t.find("Wraith");
            const Monster* l = t.find("Lich");
            ok = (w != nullptr && w->hp == 14 && l != nullptr && l->hp == 35);
        }
        result("insert+find: size==5, Wraith->hp==14, Lich->hp==35", ok,
               "insert must place each monster in bucketIndex(name)'s chain and ++count_; "
               "find must scan that one bucket and return the match");
        all = all && ok;
    }

    // --- Phase 3: no duplicates / update in place ---
    {
        HashTable t(buildSampleBag());
        // Re-insert Lich with a changed hp: size stays 5, the record updates,
        // and the second insert reports false (it did not add a new item).
        const bool added = t.insert({"Lich", 99, 6, "holy"});
        const Monster* l = t.find("Lich");
        const bool ok = (!added && t.size() == 5 && l != nullptr && l->hp == 99);
        result("no-dup insert: re-insert Lich updates in place, size stays 5, returns false", ok,
               "insert must not double-count an existing name; re-inserting updates in place and returns false");
        all = all && ok;
    }

    // --- Phase 4: find absent -> nullptr, even amid a forced collision ---
    // Use a ONE-bucket table so EVERY name collides into bucket 0 regardless of
    // the student's hash. find must compare the NAME (not just the bucket), so an
    // absent name returns nullptr even though its bucket is full, and present
    // names are still found among their colliding neighbors.
    {
        HashTable t(1);   // one bucket: everything collides
        t.insert({"Goblin",   8, 2, "fire"});
        t.insert({"Wraith",  14, 4, "holy"});
        t.insert({"Skeleton",10, 3, "blunt"});
        const bool ok = (t.find("Drake") == nullptr
                         && t.find("Goblin")   != nullptr
                         && t.find("Skeleton") != nullptr);
        result("collision find: absent==nullptr in a full bucket; present names still found", ok,
               "find must return nullptr for an absent name even when it collides into a non-empty "
               "bucket — compare the name, do not return the first item in the bucket");
        all = all && ok;
    }

    // --- Phase 5: erase removes only the one item from its chain ---
    {
        HashTable t(buildSampleBag());
        const bool removedWisp = t.erase("Wisp");
        // Wisp is one of three holy monsters; erasing it must not disturb the
        // others that may share its bucket.
        const bool ok = (removedWisp
                         && t.size() == 4
                         && t.find("Wisp")   == nullptr
                         && t.find("Wraith") != nullptr
                         && t.find("Lich")   != nullptr
                         && !t.erase("Drake"));
        result("erase: Wisp removed (size 4), Wraith/Lich intact, absent==false", ok,
               "erase must remove only the matching item from its bucket's chain and --count_; "
               "absent name returns false; do not clear the whole bucket");
        all = all && ok;
    }

    // --- Phase 6: rehash grows the table, preserves all items, lowers load ---
    // Start with a tiny bucket count so a handful of inserts crosses the
    // threshold and forces a resize. Assert the bucket count GREW, size is
    // unchanged by the rehash, EVERY name is still findable afterward (rehash
    // re-filed them all into their new homes), and the load factor is back under
    // the threshold.
    {
        HashTable t(2);   // two buckets: fills fast
        t.insert({"Goblin",       8, 2, "fire"});
        t.insert({"Wraith",      14, 4, "holy"});
        t.insert({"Skeleton",    10, 3, "blunt"});
        t.insert({"Lich",        35, 6, "holy"});
        t.insert({"Wisp",         3, 1, "holy"});
        t.insert({"Frostmaw",    22, 6, "fire"});
        t.insert({"Cave Troll",  28, 7, "fire"});   // 7 items: well past 0.75 of 2
        bool ok = (t.bucketCount() > 2 && t.size() == 7);
        if (ok) {
            for (const char* n : {"Goblin", "Wraith", "Skeleton", "Lich",
                                  "Wisp", "Frostmaw", "Cave Troll"}) {
                if (t.find(n) == nullptr) ok = false;
            }
        }
        ok = ok && (t.loadFactor() <= HashTable::kMaxLoadFactor); // Temporarily commented out to find the specific problem with rehashIfNeeded/rehash.
        result("rehash: bucketCount grew, all 7 still findable, loadFactor<=threshold", ok,
               "rehashIfNeeded must grow buckets when loadFactor exceeds the threshold; rehash must "
               "re-file ALL existing items (recompute bucketIndex) and keep count_/findability intact");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
