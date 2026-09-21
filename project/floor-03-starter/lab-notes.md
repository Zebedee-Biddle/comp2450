1. Transcript:

> inventory
   1.  Rusty sword       (wt 4.0, val 5)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Iron key          (wt 0.1, val 0)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Cloak of shadows  (wt 1.5, val 80)
> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
> search Iron key
  Iron key   (wt0.1, val 0).
> inspect 99
No such item exists:
> sort inventory by weight
   1.  Iron key          (wt 0.1, val 0)
   2.  Loaf of bread     (wt 0.1, val 1)
   3.  Healing potion    (wt 0.5, val 12)
   4.  Cloak of shadows  (wt 1.5, val 80)
   5.  Rusty sword       (wt 4.0, val 5)
>

2. Error message: C2228 "left of '.name' must have class/struct/union", in Search.h line 71, despite the erroneous findByName() being in main().
  I presume this has something to do with the fact that the template just assuming that whatever Bag<T> is passed in is valid at the point of
  call, not considering whether this T has the necessary member .name. The error does not appear until the Search.h. The difficulty from this is
  that I as a human programmer can conceptualize that the template is only _designed_ to take in T's with a .name member and that the error is the
  caller's "fault", but the compiler cannot have these abstract notions of "fault". This difficulty comes when you can assume the possibility that
  any arbitrary T has any member, which is necessary for any templated functions which use T's with members to work at all... but in the event
  of a misconstructed T being passed in, not definitively knowing "fault" makes peculiar errors more likely - so risk of weird errors is the tradeoff
  for portability, because errors cannot be tested when making the function.

3. When I replaced .at() with the [] accessor, the program would give an assertion failure from inside the vector library whenever I attempted
  to inspect an invalid index. I might want to use an [] in a development context if I already did my own bounds-checking at a previous point,
  since bounds-checking twice is unnecessary. If [] is throwing an assertion failure, this seems to indicate that there is an assert, which would
  appear to mean that [] is not actually faster in this current environment... but I looked through the vector library where the assert was
  called, and it was surrounded by the check #if _MSVC_STL_HARDENING_VECTOR || _ITERATOR_DEBUG_LEVEL != 0; the first, judging by its name, is
  exclusive to MSVC, whilst the second, judging by its name, is exclusive to debugging. In fact, on my default compiler settings in VS2022 (this
  information all comes from MSVC default settings on VS2026), the [] accessor on a vector simply returns garbage in the event of an invalid index,
  excepting large overshoots which cause the program to be shut down by some security process. So from these facts, I can observe that [] is, in fact,
  usually faster than .at() and thus desirable for use when bounds-checking has already been performed and is thus irrelevant.

4. This was printed to the terminal:
  Exception thrown at 0x00007FFEA82041CA in the_descent.exe: Microsoft C++ exception: dungeon::BagException at memory location 0x00000040660FF4E0.
  Unhandled exception at 0x00007FFEA82041CA in the_descent.exe: Microsoft C++ exception: dungeon::BagException at memory location 0x00000040660FF4E0.

  Description: The error messages are (peculiarly, considering track record) self-evident; an exception was thrown and then not caught, so the
  program aborted. The more specific narrative account: I input index 99, which got passed into the inventory's .at() accessor. The accessor
  found 99 to be out of range, so it threw an error, and without anything to catch it, the program aborted.
   
  When replacing the catch with the Grix catch block, the program is frustratingly opaque; though the program does not appear to be corrupting
  itself, any invalid commands silently fail, which provides no (potentially necessary for beginners) information on how they should be corrected.

5. Nothing compiles if BagException does not inherit from std::exception, because it what() is an override function and the code will not compile
  if there is an override function in a non-derived class. But if this is corrected, the program still crashes when inputting an invalid index,
  because the catch(std::exception) only works when it actually catches an std::exception. Throwing exceptions which do not inherit is problematic
  for people who expect catch(std::exception) to catch exceptions.

Unrelated info: C++ making inheritance protected by default is definitely intuitive and definitely did not cost me twelve minutes of debugging.