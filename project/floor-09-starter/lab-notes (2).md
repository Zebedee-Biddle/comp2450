Transcript:
> selftest bst
  empty BST<int>: empty() && size==0 && height==0: OK
  insert: root==5 and size==7 over the sample: OK
  contains: 7 and 2 present, 6 and 404 absent: OK
  inOrder == {2,3,4,5,7,8,9} (ascending): OK
  degenerate: sorted inserts give height==5, not balanced: OK
  deep copy: mutating the original leaves the copy unchanged: OK
  remove: leaf(2), one-child(3), two-children(5) all correct: OK
  all phases OK
> catalogue
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
  (15 creatures, alphabetical; tree height 6)
> recall Lich
Lich   HP 35   ATK 6   weakness: holy
  (recalled from the tree)
> recall Drake
  'Drake' is not in McCown's catalogue.
> remove Goblin
McCown does not recognize 'remove'. Try `help`.
> forget Goblin
  McCown strikes 'Goblin' from the tree. (14 remain.)
> benchmark bst

-- Bestiary: linear scan vs BST lookup (1000 iterations per cell) --
  N=    100  query=last    linear=     5.100 us  BST=     0.734 us  (tree height 13)
  N=    100  query=absent  linear=     3.080 us  BST=     0.639 us  (tree height 13)
  N=   1000  query=last    linear=    24.086 us  BST=     0.700 us  (tree height 22)
  N=   1000  query=absent  linear=    15.128 us  BST=     0.638 us  (tree height 22)
  N=  10000  query=last    linear=   172.894 us  BST=     0.665 us  (tree height 30)
  N=  10000  query=absent  linear=   116.782 us  BST=     0.586 us  (tree height 30)
  N= 100000  query=last    linear=  1818.446 us  BST=     0.450 us  (tree height 40)
  N= 100000  query=absent  linear=  1195.623 us  BST=     0.459 us  (tree height 40)

Linear scan grows with N; BST lookup grows with log N ΓÇö the same
win binary search gave you on Floor 1, but now the structure STAYS
searchable as you insert and remove, with no re-sort step.
One catch: this tree was built from SHUFFLED data. Insert the
monsters already sorted and the BST degenerates into a stick
(height ~N), and lookup drops right back to linear ΓÇö the problem
the next floor's self-balancing tree exists to solve.

> quit
McCown notes your departure. "You leave in order."

2. When I tried to write an optimal BST by hand, it started at 6, went right to 10; and left to 7. It did not need to search 67% of the tree, making it efficient.
3. "Left, node, right" is unclear and lacks context.
4. Tree:			13
				 3   14
			   1 7
The tree haad to be completely rebuilt, anyways, after the last step (the removal of the root).

5. Selftest bst is in the transcript, but if an ordered list of monsters had been input instead, it would make a long stick extending to the bottom-right corner; searching would thus
be O[n] like a regular linear search through a vector but with more memory consumption from the pointers and a higher coefficient k O[kn] due to needing to chase these pointers, which
might possibly be fragmented.

6. The ordering rule in a BST bought me fast searching, and avoided overhead that comes with having to make every node's pointers a vector of pointers instead of... pointers. The longer
chains ultimately did make remove() slower in many cases by creating more difficulty in the reassignment of children pointers.