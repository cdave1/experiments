/*
 * Copyright David Petrie 2008
 *
 * Register machine implementation: see end comment for sample 
 * register machine programs.
 */


/* Finds the value stored in a given register.
 * lookup(I, V, R) will succeed if register I of the registers R has value V.
 * 
 * Lookup recurses through each register and unifies if both the register number
 * and value match the terms I and V.
 */
lookup(I, V, [(I,V)|_]).
lookup(I, V, [(N,R)|T]) :- lookup(I, V, T).


/*
 * Updates the value stored in a given register. update(I, V, R) 
 * will succeed with register I of the registers R having value V.
 *
 * Tests:
 * update(2, 4, [(1,0),(2,3)], X).
 * update(3, 4, [(1,0),(2,3)], X).
 * 
 * Builds a new register list with the updated value if I matches.
 */
update(I, V, [], []).
update(I, V, [(I,N)|T], [(I,V)|New]) :- update(I, V, T, New).
update(I, V, [(N,R)|T], [(N,R)|New]) :- update(I, V, T, New).


/*
 * Finds the instruction referred to by a given label. fetch(L, I, P) 
 * will succeed if instruction I in the program RP has label L.
*/
fetch(L, I, [(I, L)|_]).
fetch(L, I, [(N, _)|T]) :- fetch(L, I, T).  


/*
 * Takes an instruction zero, succ, trans, jump and updates the state. 
 * executeinstruction(I, Nxt, R, NuNxt, NuR) will succeed if instruction 
 * I causes the program state (Nxt, R,) to be updated to (NuNxt, NuR).
 * 
 * There are different predicates for each instruction.
 */

/* 
 * zero(N) puts a zero into register N 
 */
executeinstruction(zero(N), Nxt, R, NuNxt, NuR) :- 
    update(N, 0, R, NuR), 
    NuNxt is Nxt + 1.


/* 
 * succ(N) increments the regester at N by one 
 */
executeinstruction(succ(N), Nxt, R, NuNxt, NuR) :- 
    lookup(N, V, R), 
    SV is V+1, 
    update(N, SV, R, NuR), 
    NuNxt is Nxt + 1.


/* 
 * transfers value at m into register n 
 */
executeinstruction(trans(M,N), Nxt, R, NuNxt, NuR) :- 
    lookup(M, V, R),  
    update(N, V, R, NuR), 
    NuNxt is Nxt + 1.


/* 
 * jump (m,n,q). jump to q if values at registers m and n are equal
 * HACK HACK HACK - need to make the reference to NuR = R, do this with update 
 */
executeinstruction(jump(M,N,Q), Nxt, R, NuNxt, NuR) :- 
    lookup(M, First, R), 
    lookup(N, Second, R), 
    First == Second,
       update(0,0,R,NuR), 
       NuNxt is Q;
    lookup(M, First, R), 
    lookup(N, Second, R),
    First \= Second, 
       update(0,0,R,NuR), 
       NuNxt is Nxt + 1.


/* 
 * stop - next instruction set is 0 
 */
executeinstruction(stop, Nxt, R, NuNxt, NuR) :- 
    update(0,0,R,NuR), 
    NuNxt is 0.



/*
 * Takes a program, and an initial set of registers and executes the program. 
 *
 * executeprogram(P, Regs, Result) will succeed if Result is the value in register 
 * 1 when the program P is run with the registers set as Regs.
 * 
 * Pass in the initial instruction to executeprogram/4. The program will
 * fetch instructions at "NextInstruction" until "NextInstruction" equals
 * zero. At this point Result is set to the value in register 1.
 */
executeprogram(P, Regs, Result) :- 
    executeprogram(P, Regs, Result, 1).

executeprogram(P, Regs, Result, NextInstruction) :-
    NextInstruction == 0, 
       lookup(1, Result, Regs);
    fetch(L, NextInstruction, P), 
       executeinstruction(L, NextInstruction, Regs, NuNxt, NuR), 
       executeprogram(P, NuR, Result, NuNxt).


/*

Test programs:

 *
 * Tests:
 * lookup(2, 3, [(1,0),(2,3)]).
 * lookup(2, X, [(1,0),(2,3)]).

From assignment handout, addition:

Program = [(1, zero(3)),
 (2, trans(1, 4)),
 (3, jump(2, 3, 7)),
 (4, succ(4)),
 (5, succ(3)),
 (6, jump(3, 3, 3)),
 (7, trans(4, 1)),
 (8,  stop)], executeprogram(Program, [(1,3),(2,5),(3,0),(4,0),(5,0)], X).


Jump test:
Program = [(1, jump(1,2,3)), (2, stop), (3, succ(1)), (4, jump(1,1,1))], executeprogram(Program, [(1,0),(2,0)], X).

Multiplication Program:

Program = [(1, zero(3)),
(2, zero(4)),
(3, jump(2, 3, 16)),
(4, jump(3, 3, 7)),
(5, succ(3)),
(6, jump(3, 3, 3)),
(7, trans(4, 5)),
(8, trans(1, 6)),
(9, zero(7)),
(10, jump(6, 7, 14)),
(11, succ(5)),
(12, succ(7)),
(13, jump(7, 7, 10)),
(14, trans(5, 4)), 
(15, jump(7, 7, 5)),
(16, trans(4, 1)), 
(17, stop)],  executeprogram(Program, [(1,13),(2,7),(3,0),(4,0),(5,0),(6,0),(7,0)], X).


*/
