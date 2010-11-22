-- Copyright David Petrie 2008
--
-- Depth first search
module DFS (dfsearch) where
import Stack

-- Depth first search starts here.
--
-- If stack is empty, means there is nothing left to visit
-- Otherwise, pop the top of the stack, and add adjacent nodes to the stack. 
-- Put the popped node on visited list and repeat the DFS
dfs :: (Eq a) => (a -> [a]) -> (Stack a) -> [a] -> (Stack a)
dfs graph stack visited = if (isEmptyStack next) then (putlist emptyStack visited)
	   	else dfs graph (putlist rest (graph popped)) (popped:visited) where
		next = getnext stack visited
		(popped, rest) = pop next

-- Find the stack with an unvisited head.
-- An empty stack means we've visited every node reachable from initial node.
getnext :: (Eq a) => (Stack a) -> [a] -> (Stack a)
getnext stack visited | isEmptyStack stack = stack
getnext stack visited | (popped `member` visited) = getnext rest visited where 
		      	      (popped, rest) = pop stack
getnext stack visited | otherwise = stack

-- Pushes a list onto a stack
putlist::(Stack a) -> [a] -> (Stack a)
putlist st [] = st
putlist st (h:t) = putlist (push h st) t

-- Returns true if the item is in the specified list
member :: (Eq a) => a -> [a] -> Bool
s `member` l = elem s l

-- Entry point for starting the search
dfsearch :: (Eq a) => (a -> [a]) -> a -> (Stack a)
dfsearch graph node = dfs graph (push node emptyStack) []

