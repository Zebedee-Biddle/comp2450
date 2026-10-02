Transcript:
> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
  (found in bestiary)
> search Iron key
  Iron key  (wt 0.1, val 0)
  (found in inventory)
> search began
  began session as "Zebedee C Biddle"
  (found in event log)
> log
   1.  search began ΓÇö found in event log
   2.  search Iron key ΓÇö found in inventory
   3.  search Goblin ΓÇö found in bestiary
   4.  began session as "Zebedee C Biddle"
  (newest first; chain length 4)
> log --oldest 3
   1.  began session as "Zebedee C Biddle"
   2.  search Goblin ΓÇö found in bestiary
   3.  search Iron key ΓÇö found in inventory
  (oldest first; chain length 4)
> selftest iterator
  range-for over Chain<int>: OK
  std::find(Chain<int>, 42): OK
  std::distance(begin, end): OK
  range-for over const Chain<int>&: OK
  std::reverse(Chain<int>) ΓÇö first now == 9: OK
  all phases OK
> quit
The lens dims. The lens does not remember what it saw ΓÇö only how it moved.

2. Transcript:
> search Wraith
Wraith   HP 14   ATK 4   weakness: holy
  (found in bestiary)
> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
  (found in bestiary)
> search Zombie
No such creature, item, or past event under that name.
> log
   1.  search Zombie ΓÇö not found
  (newest first; chain length 4)

3. Transcript:
> search Goblin
Goblin   HP 8   ATK 2   weakness: fire
  (found in bestiary)
> search Wraith
Wraith   HP 14   ATK 4   weakness: holy
  (found in bestiary)
> log
  (the chain is empty ΓÇö nothing to remember yet)
> selftest iterator
  range-for over Chain<int>: FAIL ΓÇö begin == end (stub returns true) ΓÇö implement operator++ and operator==
  std::find(Chain<int>, 42): FAIL ΓÇö std::find returned end() ΓÇö likely operator++ stub or operator== stub
  std::distance(begin, end): FAIL ΓÇö expected 100 ΓÇö got 0 means begin == end immediately (operator== stub)
  range-for over const Chain<int>&: OK
  std::reverse(Chain<int>) ΓÇö first now == 9: FAIL ΓÇö operator-- not yet wired (Friday) ΓÇö std::reverse can't walk back
  (see FAILs above)
The broken end() violated the contract by removing the sentinel value; instead of going to nullptr after the tail and properly indicating the
end of the Chain, it jumped straight to the beginning.

4. Error C2676 binary '-': 'const _InIt' does not define this operator or a conversion to a type acceptable to the predefined operator	C:\Users\zebbi\OneDrive\Pictures\3512725078921426\GitHub\comp2450\project\floor-05-starter\out\build\x64-Debug\floor-05-starter	C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\include\algorithm	9085		
The std::sort is asking for random access, because it needs that to efficiently make comparison operations.

5. I can already tell that the range-based for loop will be easier to maintain, because the iterator syntax does not need to be changed whenever
the container changes (e.g., Chain<std::string>::const_iterator to Dechain<std::string>::const_iterator).

6. The reverse_iterator is the abstraction that lets printLog() work with only one logic path. This is because the rbegin() and rend() functions
_themselves_ contain the logic for reversing, quietly working backwards instead of forwards when using the increment operator.