# Trial I — *The Foundations Brief*

*Warden of the Foundations · Midterm 1 · 15 %*

Four short answers, one from each floor above. **50–100 words per answer.** No code on this trial (except the lambdas in Q3); just clear thinking.

AI is welcome to *check* your answers; it is not welcome to *write your sentences*. These answers are short enough that an LLM voice is recognisable.

---

## 1. Floor 0 — ADT

> Your battle's "Use item" menu shows the *currently usable* items in your hero's inventory on this turn. Name the right ADT for that menu. Defend the choice against its closest neighbour (e.g., why `bag` instead of `set`, or `list` instead of `bag`).

The right ADT for the "Use item" menu is a bag. A bag is superior to a set because of the possibility that there might be multiple of a single item (e.g., two consumable healing items).

---

## 2. Floor 1 — search & Big-O

> Your inventory is kept sorted by healing power (in this codebase an item's `value` measures its potency, so `value` plays the healing-power role). The player types `use Healing potion`. Linear or binary search to find it by name? Justify, and give the Big-O for each.

Linear search to find healing potion by name. This is because since the inventory is sorted by value, it will need to be resorted to do a binary search. Thus, while the binary search itself is only O(log n), the full procedure is O(nlog n), worse than a linear search.

---

## 3. Floor 2 — sort & comparators

> Your "Use item" menu must be displayable sorted *either* by healing power (meaning `value`) *or* by weight. Show a one-line comparator (lambda) for each. One sentence on what language feature makes one `std::sort` call serve both orders.


```cpp
// by healing power — i.e. by value
auto byValue  = [&](const Item& a, const Item& b) { return a.value > b.value; };

// by weight
auto byWeight = [&](const Item& a, const Item& b) { return a.weight > b.weight; };
```

(one-sentence answer — what language feature?)

The comparator is what allows a single std::sort to perform either sorting method, by allowing a comparison operator to be passed to a function.

## 4. Floor 3 — templates & exceptions

> Why does `Bag<T>` live in `Bag.h` instead of `Bag.cpp`? And: when the player types `9` for a 4-option menu, where in your code should the validation **throw**, and where should it **catch**?

Bag<T> is a template class. Template classes are messy to deal with; they cannot be split between .h and .cpp files as easily as regular classes. Potentially more importantly, since Bag<T> is essentially a coat of paint over vector<T>, most of its functions are very simple pass-through functions. Since these functions are so simple and formulaic, it is a waste of error-prone typing to put them in a template class declaration, then a separate template class definition, then a separate template function definition. Since they are simple pass-through functions, at the very least, these specific functions induce no concern that they will endlessly accumulate and cause pollution.
Calling the .at() function on the Bag whenever it naturally needs to be accessed for the first time in a given selection is elegant, because it abstracts away the bounds-check. Thus, the code does not need to throw at all outside of the already-written Bag. The throw will still ultimately come from this. The catch should be placed immediately inside the main loop, with everything being in a try block. This will allow the user to return to the main menu by inputting an invalid item number, which may be desirable in the event of, e.g., accidentally entering the item menu.