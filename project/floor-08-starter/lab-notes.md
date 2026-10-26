1. Transcript:
1. > selftest tree
  empty Tree<int>: empty() && size==0 && height==0: OK
  addChild: root==1 with 3 children: OK
  size()==7 over the sample tree: OK
  height()==2 (longest path 1->2->5): OK
  preOrder == {1,2,5,6,3,4,7} (depth-first): OK
  levelOrder == {1,2,3,4,5,6,7} (breadth-first): OK
  lineage(6)=={1,2,6} and lineage(404) empty: OK
  all phases OK
> taxonomy
Creatures of the Hold
    - Undead
        - Corporeal
            - Skeleton
            - Necrothrall
            - Lich
        - Incorporeal
            - Wraith
            - Wisp
    - Beasts
        - Mammals
            - Shadow Hound
            - Ironclaw Bear
            - Cinder Bat
        - Giants
            - Cave Troll
            - Frostmaw
    - Vermin
        - Bone Spider
        - Ratking
        - Marsh Lurker
    - Constructs
        - Stone Sentinel
  (23 creatures and categories; height 3)
> taxonomy --bfs
  breadth-first (level-order):
    Creatures of the Hold
    Undead
    Beasts
    Vermin
    Constructs
    Corporeal
    Incorporeal
    Mammals
    Giants
    Bone Spider
    Ratking
    Marsh Lurker
    Stone Sentinel
    Skeleton
    Necrothrall
    Lich
    Wraith
    Wisp
    Shadow Hound
    Ironclaw Bear
    Cinder Bat
    Cave Troll
    Frostmaw
> lineage Lich
  Creatures of the Hold > Undead > Corporeal > Lich
> lineage Goblin
  'Goblin' is not in McCown's catalogue.
> quit
McCown notes your departure on the chart. "You leave classified."

2. The _one_ difference between countFrom(), heightFrom(), and preOrderInto() is what is done in the main body on each node; all recurse and use a nullptr exceptional case. The first
two listed functions are almost identical except for the direction that they recurse; preOrderInto() is the non-Synoptic recursive method.

3. I have enough points buffer to not have to deal with _that_, but I can logically intuit that shallow, far-right nodes will be the most separated - with breadth-first search, these
will be very early; but with depth-first search, these will be very late.

4. Adding Goblin did... nothing. But if it hypothetically _did_ work, it would make more sense to add to the .txt asset than to the .cpp file. The primary reason for this is _not_ for
the compilability (at least in projects this size - although this becomes more important in larger projects...) but for the ability to have a dedicated place to edit and to not need to
give code access to users just to change some minor configuration details. I first attempted to put the Goblin in a new branch, Creatures of the Keep/Sapient/Humanoid/Goblin, because
neither Beasts nor Undead makes a significant quantity of sense, and other categories such as Automotons and Vermin even less. I then attempted to put the Goblins in the Corporeal
Undead, because they could theoretically be undead in some mythologies (but near-universally possess at least within a standard deviation of human intelligence) (but are _definitely_
not incorporeal).

5. Uncommented recursive code is murder on the eyes, but I am fairly certain that this change would cause the leaves and root to swap their order of placement on the vector. But I do
not believe that it would be a clean reversal, either, and branches would simply be screwed up.

6. You have unduly punished range-based for loop implementations of findFrom() by configuring a pre-C++20 runtime in CMake and then asking for a counter well after it was written. But
as an aside to this, it still takes twenty-three iterations to look over twenty-three entries, because a normal Tree searches each node linearly. A binary search would be required to
reduce the number of iterations, but this would require ranking, which would require a binary tree (_generally_ speaking).

7. This notes file is getting very long, but the question has (fortunately) largely answered itself; the _branching_ part of the Tree was new, specifically each Node having a Bag of
pointers to subsequent entries instead of just one. Similarly, additional methods were required to manipulate the container in novel ways. However, the underlying concept of traversing
through pointers to pointers to pointers has become familiar enough that the Tree was somewhat comfortable to implement, far better than I would have been at the start of the semester.