# Trial III — *The Marshalling Yard* — Notes

*Warden of the Middle Gates · Midterm 2 · 60 %*

This file is the grader's map of your wave-survival code. Help us find each piece. Be specific — file and line numbers are encouraged.

---

## AI declaration for Trial III

(One or two sentences. *Which* tool, *what* you asked it for, *what* you did with the result. "Did not use" is fine — but the line must be there.)

> *Trial III — used Gemini to figure out syntax concepts (confirmed that assignment returns value assigned instead of bool, and discovered what continue; does).

## Two functions I wrote without AI assistance

These are the functions Friday's quiz can ask me to modify on paper. I wrote each of these by hand, with no autocomplete on the body. Pick functions you know cold — three of the four quiz questions pull from these.

1. `EvalPlayerDmg()` in `Battle.cpp::437-442`
2. `All undoHandler methods` in `Battle.cpp::292-315`, and the class body itself.

---

## Floor 4 – 7 ties — where to find them in my code

Help the grader find each of the four required ties.

**Floor 4 / 4½ (linked lists).** Every turn appends to `hero.eventLog` (a `Chain<std::string>`):

- Engagement entry: `battle/Battle.cpp:317`
- Per-turn attack entry: `battle/Battle.cpp:(normal: 373, crits: 369)`
- Per-turn retaliation entry: `battle/Battle.cpp:(412 for hits, 414 for death)`
- Undo entry (intentional FORWARD push, not a rewind): `battle/Battle.cpp:311`

**Floor 5 (iterators).** Inspect-log branch calls `printLog` with an iterator pair:

- `battle/Battle.cpp:<line>` — `printLog(hero.eventLog.<begin/rbegin>(), hero.eventLog.<end/rend>(), N, ...)`
- Direction choice + defence (one line): Newest first allows one to see the recent past (which is usually more pressing) without displaying an arbitrarily long list; so I picked new-
- est first using begin() and end().

**Floor 6 (stacks).** Undo history stored in:

- Container type: `Stack<UndoAction>`
- Declared at: `battle/Battle.cpp:<line>` (local? or `hero.undoStack`? — say which and why)
Locally, but inside another class which served as the undoHandler (lines 292-315), the one with the extremely long typename that I used to prevent accidental duplicate declarations.
- Pushed at: `battle/Battle.cpp:376` (before Attack), `battle/Battle.cpp:388` (before Use item)
- Popped at: `battle/Battle.cpp:401` (in Undo case)
- One-line defence (stack vs queue/deque): (your comment, or quote the comment in the code): I used a stack because a queue would only allow accessing the end through traversing every-
thing else, being O(N); and a deque would allow accidental use as a queue (which could only create bugs).
- One-line "what undo intentionally does NOT restore": Undo intentionally does not restore the line of enemies, because the instructions told me not to.

**Floor 7 (queues).** Current wave held in:

- Container type: `Queue<Enemy>` (or `Queue<EnemyPtr>` if you used pointers): Queue<Enemy> (If there is no reason to use pointers, see https://www.xkcd.com/703/ and 1310's alt-text).
- Built at: `battle/Battle.cpp:320` (via your `buildWave` helper or inline)
- `front()` called at: `battle/Battle.cpp:328,358,381` (to read the active target; should NOT mutate the queue)
- `dequeue()` called at: `battle/Battle.cpp:379` (only when the target's HP <= 0)

---

## Reflection  *( ≤ 200 words )*

Which Floor's tie was hardest to wire in, and why? Was it picking the iterator direction for Inspect log, the snapshot-and-restore on `Stack<UndoAction>`, the front-of-queue targeting, or deciding what to leave OUT of the undo snapshot?

The front-of-queue targeting was somewhat difficult, because although the references saved some typing, remembering their scope was somewhat finnicky.

---

## Tuning notes (optional)

Starting player HP: 40
Foe HP's increase with higher waves.