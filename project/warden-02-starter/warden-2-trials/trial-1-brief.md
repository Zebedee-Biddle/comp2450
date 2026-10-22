# Trial I — *The Keepers' Brief*

*Warden of the Middle Gates · Midterm 2 · 15 %*

Four short answers, one for each floor span above. **50–100 words per answer.** No code on this trial (except the one-liner in Q2); just clear thinking.

AI is welcome to *check* your answers; it is not welcome to *write your sentences*. These answers are short enough that an LLM voice is recognisable.

---

## 1. Floors 4 / 4½ — linked lists

> Your encounter's **event log** (every "Aric strikes Imp for 6", every "Imp retaliates for 3") is appended to `hero.eventLog`, which is a `Chain<std::string>`. Defend that against a `Bag<std::string>` (vector-backed) in terms of one operation the battle actually performs — naming the Big-O of that operation in `Chain` and in `Bag`. *Hint: think about what `Bag` has to do the moment its capacity is exceeded.*

A Bag would suffer continuouslly degrading performance as it grew, due to the fact that it has to completely relocate its entire self everytime its size doubles and the fact that either
its insertion or removal _must_ be O(1) (since one of these must occur on the front of the vector, shifting everything).

---

## 2. Floor 5 — iterators

> When the player picks the **Inspect log** menu option, your encounter calls `printLog(hero.eventLog.begin(), hero.eventLog.end(), 5, ...)` — the same function template your Floor 5 iterators brought to life. (Because `eventLog` is built with `push_front` everywhere, the *front* of the chain is the most recent entry; the forward iterator walks newest-to-oldest naturally.) Show **one line** of C++ that does the equivalent walk by hand using an explicit `Chain<std::string>::iterator` loop. Then one sentence: why is this iterator *bidirectional* (and what becomes impossible the moment you ask for random-access on it)?

```cpp
// your one-line walk
```

(one-sentence answer — bidirectional vs random-access)

---

## 3. Floor 6 — stacks

> Your `undo` command pops the most recent action off a `Stack<UndoAction>` and restores its snapshot. Defend the choice of stack against a queue or a deque in one sentence — referencing the *contract* of LIFO (not the implementation). Then one extra sentence: name one piece of battle state that your `undo` *intentionally does not* restore, and explain why the player won't mind.

A queue would only allow undoing the _first_ action, and subsequent undos would bring back more recent actions than that - very cursed. A deque is error-prone due to the risk of acci-
dentally using the wrong operation, and theoretically stacks can be singly-linked to save memory over deques (not that ours does that...)

---

## 4. Floor 7 — queues

> Each wave's enemies sit in a `Queue<Enemy>` in arrival order. Explain — in one sentence — what *visibly* changes for the player if you swap `Queue<Enemy>` for `Stack<Enemy>`: which enemy gets attacked first, and in what order the rest are picked off.

If using a Stack<Enemy>, then the newest enemies added would be the first to attack. This is not necessarily a problem if this fact is accounted for in programming, but it is more error-
prone and my current loop (of increasing the stats of later enemies from a constant baseline) is one of the many examples of things which would be materially more difficult to implement
in reverse.
