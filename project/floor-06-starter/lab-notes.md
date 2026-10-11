1. Transcript:
> selftest stack
  empty Stack<int>: size==0 && empty(): OK
  push(7): size==1 && top()==7: OK
  push 1,2,3: size==3 && top()==3 (LIFO): OK
  after pop(): size==2 && top()==2: OK
  drain to empty: size==0 && empty(): OK
  all phases OK
> lint ({a}, {b}, {c})
  balanced
> lint ({)}
  not balanced
> lint hello (world
  not balanced
> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
> take Lantern
  Taken: Lantern  (wt 0.1, val 1)
> sort inventory by weight
   1.  Iron key          (wt 0.1, val 0)
   2.  Loaf of bread     (wt 0.1, val 1)
   3.  Lantern           (wt 0.1, val 1)
   4.  Healing potion    (wt 0.5, val 12)
   5.  Cloak of shadows  (wt 1.5, val 80)
   6.  Rusty sword       (wt 4.0, val 5)
> undo
Undid: sort inventory by weight.
> undo
Undid: take Lantern.
> undo
The Vault does not remember any actions of yours to reverse.
> quit
The cantor falls silent. The last echo decays. The hall is quiet.

2. An ADT is a _promise_ to fulfill certain obligations (e.g., this structure will have a pop() function and a push() function, which pops off
and pushes onto the same direction in O(1) time). A data structure is the _implementation_; the Chain is a data structure whilst a stack is an
ADT.

3. I already know that the benchmark for a std::vector-based stack would be much slower than a Chain-based stack; a vector has O(n) push_back
and pop_back, whilst a Chain has O(1) (thus for expanding stacks, a Chain implementation is O(n) whilst a vector implementation is O(n^2)).

4. Transcript of output after loading:
The thread 105896 has exited with code 0 (0x0).
Exception thrown: read access violation.
**dungeon::Stack<int>::top**(...) returned nullptr.

Transcript of output after guarding:
about to call top() on an empty stack...
stack is empty
survived

The stack ADT does not promise that the program _itself_ will be able to handle the empty-stack error; rather, it only promises that the error
will be signaled in _some capacity_ and not, e.g., silently ignored and garbage data returned. Thus, the caller should check to verify that the
stack contains something before calling top().

5. Beyond the four one-line methods I wrote in class, the remainder of class was spent writing the lint() tester and the undo() operation. A
good ratio this was; for this code was actually doing things and not reimplementing the mundane details which have already been done (such as
the destructor and the 374 different convoluted duplication methods).