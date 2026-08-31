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

const Monster* binSearch(const std::vector<Monster>& bestiary, const std::string& name, size_t l, size_t h) {
    size_t mid = l + ((h - l) >> 1);
    std::string here = bestiary[mid].name;
	while (here != name) {if (l >= h) return nullptr; // Window is empty, you are done/protect against inverted range.
	size_t mid = l + ((h - l) >> 1); // This one I modified from the nonrecursive one.
	here = bestiary[mid].name;
    if (here < name) l = ++mid;
    else h = mid;
    } return &bestiary[mid];
}

const Monster* binSearchRecurs(const std::vector<Monster>& bestiary, const std::string& name, size_t l, size_t h) {
	if (l >= h) return nullptr; // Window is empty, you are done/protect against inverted range.
	size_t mid = l + ((h - l) >> 1); // This one I made in class.
	const std::string& here = bestiary[mid].name;
	if (here == name) return &bestiary[mid];
	else if (here < name) return binSearchRecurs(bestiary, name, ++mid, h);
	else return binSearchRecurs(bestiary, name, l, mid);
}

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    return linearSearch(bestiary, name);
}

}
