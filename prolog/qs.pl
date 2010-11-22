/*
 * Copyright David Petrie 2008
 */


/* Finds permutations of lists.
 * 
 * A list is a permutation of another if and only if both list are the same 
 * size and each element in first is also in the second.
 * 
 * We could also say that the permutation of the tail of the first list is 
 * a permutation of the result of removing the head out of the second list.
 * 
 * remove\3 pulls the head off the list and returns the resulting list.
 */

remove(X,[X|R],R).
remove(X,[H|R],[H|S]) :- remove(X,R,S).

perm([X|Xs],Y) :- perm(Xs,Z), remove(X,Y,Z).
perm([],[]).


/*
 * Returns true if list is in ascending order.
 * 
 * Simply recurse through the list checking if head is less than or 
 * equal to its neighbour. If we get to the end, the list is ordered.
*/

ordered([_|[]]).
ordered([A,B|T]) :- A =< B, ordered([B|T]).


/*
 * Inserts a value into an ordered list, making an ordered list.
 * 
 * insert(Val, List, Vlist) will succeed with VList an ordered permutation of 
 * [Val|List], where it is assumed that List is ordered.
*/
insert(Val, List, VList) :- perm([Val|List], VList), ordered(VList).



/*
 * Quicksort partitions the list using the head as the pivot. It then runs 
 * quicksort again on the lower and upper numbers produced by the partition.
 * These lists come back sorted and are appended to the pivot to get the 
 * fully sorted list.
*/
partition(Pivot, [], [], []).
partition(Pivot, [X|Xs], [X|Ls], U) :- X =< Pivot, partition(Pivot, Xs, Ls, U).
partition(Pivot, [X|Xs], L, [X|Us]) :- X > Pivot, partition(Pivot, Xs, L, Us).

append([], X, X).
append([W|X], Y, [W|Z]) :- append(X, Y, Z).

quicksort([], []).
quicksort([H|T], Out) :- partition(H, T, Lower, Upper), 
		 quicksort(Lower, SortedLower), 
		 quicksort(Upper, SortedUpper), 
		 append(SortedLower, [H|SortedUpper], Out).
