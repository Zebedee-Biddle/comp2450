// COMP 2450 — Floor 1 starter
// bestiary/Search.cpp — YOU implement this file.
//
// Four functions to write. Read Search.h for their contracts.
//
// The big idea this week: the same question ("is X in the list?") has
// three implementations with three VERY different Big-O costs. You will
// write all three, race them in `benchmark`, and then argue — in a commit
// message — which one the rest of the game should use. The code is easy.
// The *thinking* is the point.
//
// Tips for the Unsorted Lich:
//   * binarySearch (and binarySearchRecursive) only work if the bestiary
//     is sorted by name.
//   * main.cpp already calls sortBestiary() right after loading,
//     so you can assume the precondition holds when these run.
//   * If you ever doubt, scan the vector and assert it is sorted.
//
// Submit when:  `search Goblin` returns the goblin's stats and `search Ghost`
//               reports "no such creature." Then run `benchmark` and capture
//               the output for your lab-notes.md.

#include "Search.h"

namespace dungeon {

const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    for (const auto& n : bestiary) {
       if (n.name == name) return &n;
    } return nullptr;

    (void)bestiary;
    (void)name;
}

const Monster* binarySearch(const std::vector<Monster>& bestiary, const std::string& name) {
   size_t lo = 0, hi = bestiary.size() - 1;
   if (name < bestiary[0].name || name > bestiary[hi].name) return nullptr;
   size_t mid = hi >> 1;
   while (hi != lo) {
      if (name == bestiary[mid].name) return &bestiary[mid];
      else if (name > bestiary[mid].name) lo = mid;
      else hi = mid;
      mid = lo + (hi - lo) >> 1;
   } if (name == bestiary[lo].name) return &bestiary[mid];
     else return nullptr;
}

const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary, const std::string& name, size_t lo, size_t hi) {
   size_t mid = lo + (hi - lo) >> 1;
   if (lo == hi) {
      if (name == bestiary[mid].name) return &bestiary[mid];
      else return nullptr;
   } else if (name > bestiary[mid].name) return binarySearchRecursive(bestiary, name, mid, hi);
   else return binarySearchRecursive(bestiary, name, lo, mid);
}

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    return linearSearch(bestiary, name);
}

}
