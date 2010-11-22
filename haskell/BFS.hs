-- Copyright David Petrie 2008
--
-- Breadth first search of a list of items
module BFS (bfsearch) where
import Q1

-- Performs the breadth first search.
-- If queue is empty, means there is nothing left to visit
-- Otherwise, dequeue to get the node, and enqueue adjacent nodes to the queue. 
-- Put the dequeued node on visited list and repeat the BFS.
--
-- Once finished, return a queue of nodes in the order they were visited.
bfs :: (Eq a) => (a -> [a]) -> (Queue a) -> [a] -> (Queue a)
bfs graph queue visited = if (isEmptyQueue next) then (putlist emptyQueue (reverse visited))
	   	else bfs graph (putlist rest (graph dequeued)) (dequeued:visited) where 
		next = findNext queue visited
		(dequeued, rest) = dequeue next

-- Finds the first queue with an unvisited head.
-- Recurse on tail of the queue if head has already been visited.
findNext :: (Eq a) => (Queue a) -> [a] -> (Queue a)
findNext queue visited | isEmptyQueue queue = queue
findNext queue visited | (dq `member` visited) = findNext rest visited where 
		      	      (dq, rest) = dequeue queue
findNext queue visited | otherwise = queue

-- Puts a list onto the queue
putlist :: (Queue a) -> [a] -> (Queue a)
putlist q [] = q
putlist q (h:t) = putlist (enqueue h q) t

-- Returns true if the item is in the specified list
member :: (Eq a) => a -> [a] -> Bool
s `member` l = elem s l

-- Entry point for starting the search
bfsearch :: (Eq a) => (a -> [a]) -> a -> (Queue a)
bfsearch graph node = bfs graph (enqueue node emptyQueue) []