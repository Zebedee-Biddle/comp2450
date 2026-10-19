1. Transcript:
1. > selftest queue
  empty Queue<int>: size==0 && empty(): OK
  enqueue(7): size==1 && front()==7: OK
  enqueue 1,2,3: size==3 && front()==1 (FIFO): OK
  after dequeue(): size==2 && front()==2: OK
  drain to empty: size==0 && empty(): OK
  all phases OK
> potato A B C D E 3
C has met their demise.
A has met their demise.
E has met their demise.
B has met their demise.
  Survivor: D
> potato Grix Skrit Quill Vael Resh 2
Skrit has met their demise.
Vael has met their demise.
Grix has met their demise.
Resh has met their demise.
  Survivor: Quill
> potato onlyOne 7
  Survivor: onlyOne
> provoke Goblin
  Goblin takes a number and joins the line.
> provoke Skeleton
  Skeleton takes a number and joins the line.
> provoke Imp
  Imp takes a number and joins the line.
> simulate
Turn 1: Goblin stirs in the dark.
Turn 2: Skeleton stirs in the dark.
Turn 3: Imp stirs in the dark.
> simulate
No enemied in the line. Use provoke <name> first.
> quit
Quill stamps your ticket. The line shuffles forward by one.

2. Changing the Stack to a Queue, the difference was merely that I had to remove and inset on opposite ends (thus, I had to change enqueue,
but I did not have to change dequeue/pop or the accessor - at least, I would not have had to if I had not renamed them and also done it clean
room just because (psst...: the instructions told me to).)

A stack and a queue are different because a follows LIFO, requiring insertion and deletion on the same end, like a physical... stack. A queue
follows FIFO, requiring insertion and deletion on opposite ends, like a physical... queue. (It is a wonder where these names come from).

dungeon::Queue uses a Chain instead of a Bag because a Bag would have O(n) insertion and deletion as opposed to a Chain's O(1). Multiplying by
a given queue complexity, running this queue through a loop would give O(n * n = n^2) complexity for the Bag-Queue and O(n * 1 = 1) complexity
for the Chain-Queue. Using a Chain is exactly what the Stack does, and since generally stacks and queues are _supposed_ to be the same in all
manners except their ordering of results returned, making the Queue have inferior performance to the Stack would be peculiar and susceptible
to violating programmer expectations.

3. Since front() and dequeue() already return the front, changing the push() function to push onto the front would make the queue into a stack
instead. This is demonstrated in the error diagnosis:

> selftest queue
  empty Queue<int>: size==0 && empty(): OK
  enqueue(7): size==1 && front()==7: OK
  enqueue 1,2,3: size==3 && front()==1 (FIFO): FAIL ΓÇö front() did not return the FIRST enqueued value ΓÇö enqueue/front are on the same end (that's a stack, not a queue)
  after dequeue(): size==2 && front()==2: OK
  drain to empty: size==0 && empty(): OK
  (see FAILs above)
> potato A B C D E 3
E has met their demise.
D has met their demise.
C has met their demise.
B has met their demise.
  Survivor: A

Bonus points: In truth, only the _results_ of changing the queue to a stack are the same in question 4; the members are still evicted in dif-
ferent order. In my testing I find that no matter the alleged size of the potato iteration, a stack always produces an effective potato size
of -1, starting at the end and deleting everything until the beginning is all that remains. Thus, since the first item is _always_ last (due to
the LIFO stack breaking the rotation), cases where the first item _should_ be last will by coincidence appear matched. Reversed transcript:
> potato A B C D E 2
D has met their demise.
C has met their demise.
B has met their demise.
  Survivor: A

4. start:            A B C D // overused trope
rotate (A to back):  B C D A
eliminate B:         C D A // I miss it when cars had this; Spotify continuously costs money, radio doesn't let you pick, and both have bad audio fidelity.
rotate:					D A C // I have some ideas involving a better DSD with higher real bit-depth than one but maintain perfect linearity nonetheless (using thermometer coding and dynamic element matching (that is just an anecdote).) But this could probably have real potential in, e.g., producing vanishing digital noise floors well beyond human-perceivable gains for, I don't know, faking a movie from 1965 (this is just a hypothetical here, nothing to see).
eliminate D:			A C // China seems to be more fond of HVDC these days. That is all well and good, but what about the frequency inertia...?
rotate:					C A // Better luck in 2028, maybe? This is what you get for closing that CSP plant and the nuclear plant (the former of which is what you got for not having the foresight to add TES).
eliminate C:			A // another overused trope
Survivor: A	// The letters '', '', and '' no longer exist (I was hoping to emonstrate this more asually but they mysteriously seem rather unommon in my parlance).

And the program matched (including the intermediates):
> potato A B C D 2
B has met their demise.
D has met their demise.
C has met their demise.
  Survivor: A

5. Little or nothing felt easier the second time; there was red tape to counteract experience gains and I furthermore could not decide whether
insertion at front and removal at back, or insertion at back and removal at front, was the canonical option for e.g. the test files, until I ex-
perimented and got it wrong and consequently had to change _all_ of the functions.