# Trial III — *The Encounter* — Notes

*Warden of the Foundations · Midterm 1 · 60 %*

This file is the grader's map of your battle code. Help us find each piece. Be specific — file and line numbers are encouraged.

I forced the RNG to reseed itself every turn using time; comment out line 218 for deterministic RNG.

In menu option switch():
220-233: Attack case (Added crits; to disable, replace line 206 with if (0).
234-236: Flee case
237-239: Use case
240-242: Inspect case

In Warden attacks (lines 243-316): I got carried away adding many attacks. To just see the normal one, replace the switch on 228 with switch (17).

Use case's helper function: 110-159 (Slight changes made to healing behaviour to support limited overheal, and Rusty sword implementation to buff player's attack for round due to game balance).

## AI declaration for Trial III

(One or two sentences. *Which* tool, *what* you asked it for, *what* you did with the result. "Did not use" is fine — but the line must be there.)

I used the Copilot app to check the correspondance of opening and closing braces in Battle.cpp, because I was getting peculiar syntax errors.
I learned the lambda function syntax from Gemini when I was working on a previous attempt of the project which I had to scrap due to incomprehensible errors; this memory I used to write out the comparators in trial-1-brief.md.

## Two functions I wrote without AI assistance

These are the functions Friday's quiz can ask me to modify on paper. I wrote each of these by hand, with no autocomplete on the body. Pick functions you know cold — three of the four quiz questions pull from these.

1. tickWormhole in `Battle.cpp:173-181`
2. <Free code> (case 9 of Warden attack) in `Battle.cpp:268-272`

---

## Floor 0–3 ties — where to find them in my code

Help the grader find each of the four required ties.

**Floor 0 (ADT).** The available menu actions are stored in:

- Container type: `Bag<MenuOption>`
- Declared at: `battle/Battle.cpp:196`
- ADT name + one-sentence defence: (paste your comment here, or quote the comment in the code): A Bag is versatile, since it allows menu options
  to be updated on the fly, which could be useful in the future due to, e.g., adding or removing options based on scripted events or debuffs.
  If a mistake is made in these cases, a Bag will simply have an unnecessary duplicate, which is better than the great variety of errors possible
  with a single-instance set. Since menu options should not change in great abundance, performance overhead is not of great concern; rather,
  a variable-size vector-style Bag is superior to an array because it needs not always allocate the maximum amount of memory. It is also more usable.

**Floor 1 (search).** `findByName<Item>` is called at:

- `battle/Battle.cpp:138` (Use-item branch)

**Floor 2 (sort).** The at-display sort (`std::sort` with a comparator, or your Floor 2 `sortInventory`) is called at:

- `battle/Battle.cpp:117` (before displaying the items menu)
- Sort criterion: `value desc` (e.g., descending value — the healing-power stand-in — or ascending weight)
- One-sentence reason: The criterion is trivial to change if another criterion becomes superior, so the best criterion right now should be focused on. Right now, the Healing potion is not
- consumed (and if it were consumed, this would be problematic for the current game balance), and so value desc putsis appropriate since it puts the Healing potion near the top.

**Floor 3 (templates + exceptions).** `BattleException` (or `BagException` where a bad index is the fault) is:

- Thrown at: `battle/Battle.cpp:86,101,142` In 86 when handling stoi exception from 79; in 101 when Bag is asked for nonexistant index; and
  when attempting to use a nonexistant item in useItem().
- Caught at: `battle/Battle.cpp:307-308` (must be **inside** the battle loop, not in main) (it is).

---

## Reflection  *( ≤ 200 words )*

Which Floor's tie was hardest to wire in, and why? Was anything obvious in hindsight that wasn't obvious when you started?

Although all of the ties appear to have been premade, F0 was gruesome in the first (scrapped) attempt due to a lack of me being able to find
conceptual need for an ADT (the best regime I could think of was something like input name -> use name to get that same name as an enum from an
ADT -> get item/action. This did not make much sense in my mind compared to just going straight from input name to item/action). On the other hand,
the exceptions from F3 just naturally worked their way out even in the first attempt. The F2 sort became utterly incomprehensible and forced me
to scrap the project after many hours, but this appeared to be a problem with MSVC and not real project difficulty.

---

## Tuning notes (optional)

If you changed any of the tunables (`kPlayerStartHP`, `kWardenStartHP`, `kPlayerAttackDmg`, `kWardenAttackDmg`), or added new ones (item healing amounts, etc.), list them and their values here so the grader can play through your battle without surprise:

There is now overheal of 5HP + 1HP for every use of the Potion (this was not implemented as a tunable, but is still catalouged to avoid surprise). This
is to alleviate the fact that, with normal tuning, a single Potion is available every turn to heal 40%, but the Warden can also deal a large amount
of damage in some cases, thus creating a lot of health rubberbanding that could otherwise be awkward.

The player's and Warden's attack damages can also change throughout the game (the Warden's can go up and the player's can go in either direction).
The Warden was given unlimited overheal, so that the player cannot spam Rusty sword (this also is why the three 1/28 "doom attacks" are included,
i.e.: Summon a magical Warden copy to get an extra attack for the rest of the battle; Summon a wormhole to deal status damage and steal all player
items; instantly kill the player).

I changed the player and Warden starting HP to 60 and 90; more HP lets the game run a bit longer and a slightly higher ratio is fairer with the (perhaps
excessively powereful) Warden.
