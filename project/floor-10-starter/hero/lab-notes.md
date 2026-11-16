1. Transcript:
> selftest registry
  index: size==5, distinct weaknesses==3, count[holy]==3: OK
  knows: Lich present, Drake absent: OK
  recall: Wraith->hp==14, Drake==nullptr: OK
  hasWeakness: holy present, water absent: OK
  countWeakness: holy==3, water==0: OK
  forget: drop a shared weakness (count--), drop a last weakness (erased), absent==false: OK
  all phases OK
> knows Lich
  McCown knows 'Lich'.
> knows Drake
  McCown does not know 'Drake'.
> weaknesses
  blunt  (2 monsters)
  fire  (6 monsters)
  holy  (3 monsters)
  light  (2 monsters)
  slashing  (1 monsters)
  water  (1 monsters)
  (6 distinct weaknesses)
> lookup Lich
Lich   HP 35   ATK 6   weakness: holy
  (looked up in the Registry)
> weakness fire
  6 monsters fear fire.
> names
Bone Spider   HP 6   ATK 2   weakness: fire
Cave Troll   HP 28   ATK 7   weakness: fire
Cinder Bat   HP 4   ATK 1   weakness: water
Frostmaw   HP 22   ATK 6   weakness: fire
Goblin   HP 8   ATK 2   weakness: fire
Ironclaw Bear   HP 24   ATK 5   weakness: slashing
Lich   HP 35   ATK 6   weakness: holy
Marsh Lurker   HP 18   ATK 3   weakness: fire
Necrothrall   HP 16   ATK 4   weakness: light
Ratking   HP 9   ATK 2   weakness: fire
Shadow Hound   HP 12   ATK 4   weakness: light
Skeleton   HP 10   ATK 3   weakness: blunt
Stone Sentinel   HP 30   ATK 5   weakness: blunt
Wisp   HP 3   ATK 1   weakness: holy
Wraith   HP 14   ATK 4   weakness: holy
  (15 names, alphabetical ΓÇö std::map keeps them ordered)
> strike Goblin
  Struck 'Goblin' from the Hall of Names. (14 remain.)
> weakness fire
  5 monsters fear fire.
> recall Goblin
Goblin   HP 8   ATK 2   weakness: fire
  (recalled from the tree)
> benchmark names

-- The Hall of Names: linear scan vs BST vs std::map (1000 iterations per cell) --
  N=    100  query=last    linear=     1.696 us  BST=   0.227 us  std::map=   0.617 us  (tree height 13)
  N=    100  query=absent  linear=     0.932 us  BST=   0.197 us  std::map=   0.568 us  (tree height 13)
  N=   1000  query=last    linear=    16.112 us  BST=   0.512 us  std::map=   0.838 us  (tree height 22)
  N=   1000  query=absent  linear=    10.796 us  BST=   0.447 us  std::map=   0.890 us  (tree height 22)
  N=  10000  query=last    linear=   146.035 us  BST=   0.604 us  std::map=   1.149 us  (tree height 30)
  N=  10000  query=absent  linear=    95.804 us  BST=   0.522 us  std::map=   1.036 us  (tree height 30)
  N= 100000  query=last    linear=  2388.093 us  BST=   0.624 us  std::map=   2.162 us  (tree height 40)
  N= 100000  query=absent  linear=  1772.954 us  BST=   1.003 us  std::map=   2.768 us  (tree height 40)

std::map::find tracks the BST ΓÇö both grow with log N, both crush
the linear scan. The difference is the code: the BST was a class you
wrote across a whole floor; std::map was one declaration. And unlike
your plain BST, std::map is balanced (a red-black tree), so it NEVER
degenerates into a stick ΓÇö the library gives you the balanced tree of
Floor 9┬╜ and a key->value index on top of it, for free.

> quit
McCown notes your departure. "You leave in order."

2a: byName_.count("Wisp") // Or (byName_.count(Wisp) != 0) if it must be sanitized to a true one-or-zero bool (but this is not necessary in most cases)
2b: byName_.at("Wisp")->second.weakness // Or byName.find("Wisp")->second.weakness iff it is certainly known that Wisp exists in the container
2c: weaknesses_.count("light")
2d: weaknessCounts_.at("holy")->second

A std::set is superiour to a std::map because a set does not allow one to keep around multiple, possibly different copies of the same monster or weakness, or (yuck) weakness count,
likely causing errors.

3: The monsters came out in alphabetical order because the container already sorted it itself (it may as well have anyways, since it had to verify at every insertion that there was no
duplicate, and it would actually be faster to keep it sorted since this check is only O[log n] instead of O[n]). I have not designed a set, but I did have to implement autosort for the
BST on floor 9.5, to prevent it from becoming an O[n] stick.

The real Zombie, upon renaming the Bone Spider to Zombie, would still remain found after the faux Bone Spider; find(Zombie) simply returns after finding the facsimile and so the real
Zombie is never found.

4: The one thing I gained on account of switching from findFrom to map::find was the freedom of no longer needing to maintain findFrom() or be held accountable if it breaks; the thing
that I lost was being able to fix problems with it myself (since I am making a terminal app, the perfomance gain of std::find over findFrom() is irrelevant).

5: I already know that using operator[] in countWeakness will fail, because operator[] is not a const function and thus cannot be used on a const container/context; the reason it can-
not be a const function is because it creates a new instance of the requested key if it does not already exist.

6: It is correct that striking Goblin still lets it be recalled even though it is removed from names, because the Tree is supposed to represent monsters that _do_ exist, and the Set
monsters that _could_.

7: Asking "Is this name here?" is a question that is asked very frequently, implied by other questions; it is asked by at(), find(), emplace(), and operator[], and so it is very im-
portant that it have O[log n] penalty and not O[n]; the former requires sorting so it can use binary search instead of linear (it's in the name). This is because (assuming that the
container is in a valid, sorted state, which requires continuous sorting) the search can simply be quit after O[log n] attempts, folding in the towel with the confident knowledge that
the desired entry does not exist if it has not been found already.