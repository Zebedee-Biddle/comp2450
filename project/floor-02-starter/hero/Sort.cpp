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
#include <algorithm>  // you will want std::sort in sortInventory

namespace dungeon {

namespace {
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
   Comparator byWeight = [](const Item& a, const Item& b) { return a.weight < b.weight; };
   mergeSortImpl(inventory, 0, inventory.size(), cmp);
}

// ---- 2. Quicksort -------------------------------------------------------

void quicksort(std::vector<Item>& inventory, const Comparator& cmp) {
    // TODO Floor 2 (Wed): implement quicksort.
    //
    // Think before you type:
    //   - Quicksort's whole performance story depends on the PIVOT. If
    //     the pivot splits the range roughly in half each time, you get
    //     O(n log n). If the pivot always ends up at one end (everything
    //     goes to one side), you get O(n^2). Why does the FIRST element
    //     cause that on sorted input? Sketch it on paper for [1,2,3,4,5].
    //   - Your fix is the MIDDLE element. It's not bulletproof — an
    //     adversary could still construct a worst-case input — but it
    //     kills the most common pathology (sorted / reverse-sorted data),
    //     which is exactly the shape real users produce.
    //   - `std::size_t` is unsigned. When `p == 0`, what is `p - 1`?
    //     That wrap-around will send your left-side recursion to index
    //     18 quintillion. Guard it.
    //   - Is quicksort stable? (Answer: no — and that is why production
    //     std::sort is ALSO not stable. If you need stability, reach for
    //     std::stable_sort or your mergeSort.)
    //
    // If you need structural hints — helpers in an anonymous namespace:
    //
    //   static std::size_t partition  (std::vector<Item>& v,
    //                                  std::size_t lo, std::size_t hi,
    //                                  const Comparator& cmp);
    //   static void        quicksortImpl(std::vector<Item>& v,
    //                                    std::size_t lo, std::size_t hi,
    //                                    const Comparator& cmp);
    //
    // Closed range convention for quicksort: [lo, hi] — both inclusive.
    // Textbooks use this for Lomuto partition; it is fine here. Guard
    // the recursive call `quicksortImpl(v, lo, p - 1, cmp)` with
    // `if (p > lo) ...` so you do not underflow when p == 0.
    //
    // PIVOT: use the middle element — `lo + (hi - lo) / 2`. Move it to
    //        the end (swap it with v[hi]) and then do the standard
    //        Lomuto scan with the pivot now at v[hi].
    //
    // If you are curious what the FIRST-element pivot looks like: the
    // benchmark harness has a `--bad-pivot` option that runs exactly
    // that. You do NOT need to implement it yourself; the harness
    // ships its own copy for Lab purposes.
    (void)inventory;
    (void)cmp;
}

// ---- 3. sortInventory (the seam) ----------------------------------------

bool sortInventory(Hero& hero, const std::string& criterion) {
    // TODO Floor 2 (Fri): parse criterion, build the right comparator,
    // dispatch to a sort.
    //
    // Think before you type:
    //   - Three decisions to make: WHICH key, ASC or DESC, WHICH sort.
    //     Don't tangle them. Parse first, then build a comparator, then
    //     hand it to exactly one sort call.
    //   - Building a DESCENDING comparator from an ASCENDING one: you
    //     don't need a whole second comparator. Wrap the ascending one
    //     and swap its arguments. (Two-line lambda. Elegant.)
    //   - Which sort? std::sort wins on speed. Your mergeSort wins on
    //     stability (and it's YOUR code — the instructor hand-wrote
    //     std::sort's ancestor forty years ago and still refers to
    //     Sedgewick). Pick one. The *choice* is the assignment.
    //   - The "desc" case for `sort by weight`: does Iron key still come
    //     before Loaf of bread on ties? That answer tells you whether
    //     your chosen sort is stable — and whether stability is the
    //     right thing for this command. (Reasonable people disagree.)
    //
    // If you need structural hints — parse with std::istringstream:
    //
    //     std::istringstream in(criterion);
    //     std::string key, dir;
    //     in >> key >> dir;       // dir is "" if absent
    //
    // Build an ascending Comparator for each key ("name", "weight",
    // "value"). If dir == "desc", wrap it in a Comparator that swaps
    // the arguments of the ascending one.
    //
    // Return false on an unknown key (main.cpp will print an error).
    //
    // Dispatch: for this week std::sort is the right production choice.
    // Your mergeSort and quicksort are correct too — pick one and
    // defend it in your commit message.
    (void)hero;
    (void)criterion;
    return false;
}

}  // namespace dungeon
