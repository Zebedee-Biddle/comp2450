1. Transcript:
> selftest balance
  balanceFactor: empty==0, bushy==0, left-leaning==1: OK
  isBalanced: bushy {5,3,8,2,4,7,9} balanced (true): OK
  degenerate: sorted stick {1..6} is NOT balanced (height 5): OK
  rotate: rotateRootLeft on {1,2,3} -> root 2, height 1, order kept: FAIL ΓÇö rotateLeft must lift n->right to the root; a rotation must NOT change the in-order order
  balanceRoot: right- and left-heavy sticks both rebalance (order kept): FAIL ΓÇö rebalanceAt must rotate left when bf<-1 and right when bf>1
  (see FAILs above)
> benchmark balance

-- Same sorted input: your plain BST vs std::set (1000 iterations per cell) --
  N=     10  plain BST: height=     9  find=    0.609 us    std::set: find=   0.659 us
  N=    100  plain BST: height=    99  find=    4.098 us    std::set: find=   0.577 us
  N=   1000  plain BST: height=   999  find=   43.050 us    std::set: find=   0.806 us

Same data, same order. Inserted in sorted order, your plain BST
degenerated into a stick of height N-1 ΓÇö lookup is back to O(n).
std::set saw the identical input and stayed shallow: it is a
balanced tree (a red-black tree) that rotates on every insert to
hold its height near log2 N. You do not hand-write that balancing ΓÇö
you reach for std::set / std::map. That is the next floor, the
Hall of Names.

> quit
McCown notes your departure. "You leave in order."

2. 1
    2
     3
      4
       5
        6
         7 (Old)

   4
 2   6
1 3 5 7 (New)

The order of the new tree is the same; only the depth varies, and the shorter depth reduces the worst-case search length by a factor of 2.33.

 5
3 8

The in-order walk before was this, right, right. The in-order walk now is left, back, right. Because both of these produce 3 5 8, the order is unchanged. Because the order
is unchanged, this means that the rule of all lessers being left and all greaters being right must hold and the BST thus must be valid.

4. A left-heavy root would have the second left entry (root is zeroth) replace the root, the root being on the right now. This would be reversed for a right-heavy root. A zigzagged
root could not be rotated like this because the following thing would happen:

1      1       1
  3 --> 2 --> 2 3
 2       3

After just one rotation, the stick is just as lanky as it was at the beginning. To fix the issue either requires multiple iterations, or passing 2 straight up to 1 somehow, not inter-
cepting 3 (3 would then have to be programmed to consider this possibility, potentially difficult).

5. The table was already pasted in the transcript, but the std::set was balancing the tree on every insert. My tree just rotates around in the hopes that it does something valid.

6. If the std library was unavailable in some embedded context, I could not use the std::set and would need to roll my own (or trust another person's, which I may not). I suppose if I
discovered (or discovered that someone online discovered) some new sorting algorithm that is more efficient than what was previously known, or was optimized for certain eccentric con-
ditions which my project encountered, and the std library had not yet/at all implemented these optimizations, I would need to do so myself.