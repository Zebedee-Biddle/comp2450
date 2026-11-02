Transcript:
> selftest bst
  empty BST<int>: empty() && size==0 && height==0: OK
  insert: root==5 and size==7 over the sample: FAIL ΓÇö insertInto is stubbed ΓÇö it must create a node at the null slot and go left/right by value
  contains: 7 and 2 present, 6 and 404 absent: FAIL ΓÇö findFrom must recurse LEFT when value<data, RIGHT when value>data, and stop on equal
  inOrder == {2,3,4,5,7,8,9} (ascending): FAIL ΓÇö inOrderInto must recurse LEFT, push the node, then recurse RIGHT
  degenerate: sorted inserts give height==5, not balanced: FAIL ΓÇö if this fails, insertInto isn't placing nodes by the invariant (see phase 2 first)
  deep copy: mutating the original leaves the copy unchanged: FAIL ΓÇö clone() is stubbed ΓÇö a real deep copy must rebuild every node, not share them
  remove: leaf(2), one-child(3), two-children(5) all correct: FAIL ΓÇö removeFrom must handle all three cases; two children copies the in-order successor up
  (see FAILs above)
> catalouge
McCown does not recognize 'catalouge'. Try `help`.
> catalogue
Ironclaw Bear   HP 24   ATK 5   weakness: slashing
  (1 creatures, alphabetical; tree height 0)
> recall Lich
  'Lich' is not in McCown's catalogue.
> recall Drake
  'Drake' is not in McCown's catalogue.
> forget Goblin
  'Goblin' is not in the tree to forget.
> benchmark bst

-- Bestiary: linear scan vs BST lookup (1000 iterations per cell) --
  N=    100  query=last    linear=     1.692 us  BST=     0.041 us  (tree height 0)
  N=    100  query=absent  linear=     1.004 us  BST=     0.041 us  (tree height 0)
  N=   1000  query=last    linear=    16.246 us  BST=     0.045 us  (tree height 0)
  N=   1000  query=absent  linear=    10.351 us  BST=     0.040 us  (tree height 0)
  N=  10000  query=last    linear=   161.690 us  BST=     0.043 us  (tree height 0)
  N=  10000  query=absent  linear=   102.666 us  BST=     0.045 us  (tree height 0)
  N= 100000  query=last    linear=  1585.797 us  BST=     0.040 us  (tree height 0)
  N= 100000  query=absent  linear=  1028.272 us  BST=     0.040 us  (tree height 0)

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
				3      14
			 1   7
The tree haad to be completely rebuilt, anyways, after the last step (the removal of the root).

5. I cannot run benchmark experiments (see the Git commit message).
6. The ordering rule in a BST bought me fast searching, and avoided overhead that comes with having to make every node's pointers a vector of pointers instead of... pointers. The longer
chains ultimately did make remove() slower in many cases by creating more difficulty in the reassignment of children pointers.