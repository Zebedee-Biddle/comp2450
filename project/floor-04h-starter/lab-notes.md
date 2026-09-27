1. Transcript:
1. > search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
> inspect 99
No such item. (index 98 out of bounds for size 5)
> log --oldest 5
  1. began session as "Zebedee C Biddle"
  2. search Goblin ΓÇö found in bestiary
  3. inventory ΓÇö listed 5 items
  4. error: index 98 out of bounds for size 5
> clone hero
  -- original log (newest first) --
   1.  error: index 98 out of bounds for size 5
   2.  inventory ΓÇö listed 5 items
   3.  search Goblin ΓÇö found in bestiary
   4.  began session as "Zebedee C Biddle"
  (newest first; chain length 4)
  -- cloned log (newest first) --
   1.  error: index 98 out of bounds for size 5
   2.  inventory ΓÇö listed 5 items
   3.  search Goblin ΓÇö found in bestiary
   4.  began session as "Zebedee C Biddle"
  (newest first; chain length 4)
  (clone is being destroyed now)
  (clone destroyed; original event log still has 4 entries ΓÇö try `log 3`)
> log 3
   1.  clone hero ΓÇö copy lived and died
   2.  error: index 98 out of bounds for size 5
   3.  inventory ΓÇö listed 5 items
  (newest first; chain length 5)
> selftest chain
  Phase 1 (single chain)
    allocations:  1000   deallocations:  1000   leaked:     0   OK
  Phase 2 (deep copy)
    original after copy died ΓÇö forward walk:  1000   backward walk:  1000
    copy before death        ΓÇö forward walk:  1000   backward walk:  1000
    allocations:  2000   deallocations:  2000   leaked:     0   OK
> quit

2. The new node's tail is set inside the node on the constructor, so push_back cannot be changed to skip this step.

3. [Exception thrown at 0x00007FF60C5515D9 in the_descent.exe: 0xC0000005: Access violation reading location 0xFFFFFFFFFFFFFFFF.]
Line five is where the second delete occurs, as evidenced by the fact that it is the only delete in the destructor.
More descriptively stating the place in the _program_ where the second delete occurred: The shallow copy made a second Chain object which had
the first Chain's pointers, so when the first Chain was deleted, this deleted the nodes pointed to by the second Chain. The second Chain im-
mediately after its first delete in its destructor, is what then caused the crash.

4. The operator= using the swap() member function would require me to first design a swap() member function. Too many nested sub-tasks cause
my brain to experience a stack overflow and forget the initial required task, so the more verbose operator=, which uses an explicit copy, would
be better for me to write on an exam.

5. A singly-linked list cannot provide O(1) pop_back because although the tail can be deleted, the proper value of the new tail_ (and, corre-
spondingly, the location of the new tail (as to set _its_ next pointer to nullptr)) cannot be found.

6. The Rule of Three: If there is a destructor; copy-constructor; or assignment operator, then all three are necessary in any classes/structs
with pointers. This because the default C++ operations for these three tasks will simply delete, copy, or assign the pointer data _itself_, &
the underlying data will be the same (this is a shallow copy). A deep copy is needed to properly handle this, which must be manually created...
UNLESS there are no pointers, due to the exclusive use of STL containers or other containers which already perform deep copies (vector<T>, etc.),
which is where the Rule of Zero comes into play: don't reassign any of these. However, there is no std::chain, so we need to create our own Chain,
which requires pointers; as such, they use the Rule of Three.