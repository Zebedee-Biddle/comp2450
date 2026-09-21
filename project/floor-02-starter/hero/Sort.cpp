// COMP 2450 — Floor 2 starter
// hero/Sort.cpp — YOU implement this file.
//
// Three functions to write. Read Sort.h for their contracts.
//
// The big idea this week: sorting is not one algorithm, it is a family
// of tradeoffs. Merge sort is predictable but copies. Quicksort is fast
// on average but betrays you on bad inputs. std::sort is what you
// actually ship. You will write the first two, race all three in
// `benchmark sort`, and argue — in a commit message — which one the
// game should call.
//
// Tips for the Pivot Wraith:
//   * If you pick the FIRST element as your quicksort pivot, a sorted
//     input becomes O(n^2). The `benchmark sort --bad-pivot --sorted`
//     harness exists to show you exactly that.
//   * The middle element is the cheapest defense. Good enough for this
//     week. Real production code (std::sort) does median-of-three and
//     switches algorithms on bad recursion depth.
//
// Submit when:  `sort inventory by weight` produces ascending weight,
//               `sort inventory by name desc` produces reverse alphabetical,
//               and `benchmark sort` gives three timing columns on every row.

#include "Sort.h"
#include "Hero.h"
#include <algorithm>  // you will want std::sort in sortInventory
#include <sstream>
#include <fstream>
//#include <stringstream>

namespace dungeon {

namespace {
   Comparator makeComparator(const std::string& key, const bool descending) {
      Comparator cmp;
      if (key == "name") cmp = [](const Item& a, const Item& b) { return a.name < b.name; };
      else if (key == "wt") cmp = [](const Item& a, const Item& b) { return a.weight < b.weight; };
      else if (key == "value") cmp = [](const Item& a, const Item& b) { return a.value < b.value; };
      else return nullptr;

      if (descending) {
         Comparator asc = cmp;
         cmp = [asc](const Item& a, const Item& b) { return asc(b, a); };
      }

      return cmp;
   }
   static void merge(std::vector<Item>& v, std::size_t lo, const std::size_t mid,const std::size_t hi, const Comparator& cmp) {
      std::vector<Item> scratch; scratch.reserve(hi - lo);
      size_t i = lo, j = mid;

      while (i < mid && j < hi) if (! cmp(v[j], v[i])) scratch.push_back(v[i++]);
                                else scratch.push_back(v[j++]);
      while (i < mid) scratch.push_back(v[i++]);
      while (j < mid) scratch.push_back(v[j++]);
      for (const auto& n : scratch) v[lo++] = std::move(n); // We are about to trash lo, anyways.
   }

   static void mergeSortImpl(std::vector<Item>& v, const std::size_t lo, const std::size_t hi, const Comparator& cmp) {
      if ((hi - lo) < 2) return;
      size_t mid = lo + ((hi - lo) >> 1);
      mergeSortImpl(v, lo, mid, cmp);
      mergeSortImpl(v, mid, hi, cmp);
      merge(v, lo, mid, hi, cmp);
   }

}

void mergeSort(std::vector<Item>& inventory, const Comparator& cmp) {
    mergeSortImpl(inventory, 0, inventory.size() - 1, cmp);
}

// ---- 2. Quicksort -------------------------------------------------------

std::size_t partition(std::vector<Item>& v, std::size_t lo, std::size_t hi, const Comparator cmp) {
   std::size_t mid = lo + ((hi - lo) >> 1);
   std::swap(mid, hi);
   const Item pivot = v[hi];

   std::size_t store = lo;
   for (std::size_t i = 0; i < hi; ++i) if (cmp(v[i], pivot)) {
      std::swap(v[store], v[i]);
      ++store;
   }

   std::swap(v[store], v[hi]);
   return store;
}

void quickSortImpl(std::vector<Item> v, std::size_t lo, std::size_t hi, const Comparator& cmp) {
   if (lo >= hi) return;
   std::size_t p = partition(v, lo, hi, cmp);
   if (p > lo) quickSortImpl(v, lo, hi, cmp);
   quickSortImpl(v, lo, p - 1, cmp);
}

void quicksort(std::vector<Item>& inventory, const Comparator& cmp) {
    quickSortImpl(inventory, 0, inventory.size() - 1, cmp);
}

bool sortInventory(dungeon::Hero& hero, const std::string& criterion)
{
    std::istringstream in(criterion);
    std::string key;
    std::string dir;
    in >> key >> dir;

    bool descending = (dir == "desc");
    dungeon::Comparator cmp = dungeon::makeComparator(key, descending);
    if (!cmp) return false;
    else std::stable_sort(hero.inventory.begin(), hero.inventory.end(), cmp);
    return true;
}
}