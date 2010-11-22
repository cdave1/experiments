-- Copyright David Petrie 2008
--
-- Haskell queue implementation - using a single list

module Q1 (Queue, emptyQueue, isEmptyQueue, enqueue, dequeue)
where

newtype Queue a = Qu [a]

-- Print the queue, with the head of the queue first.
instance Show a => Show (Queue a) where
	 show (Qu l) = pp l
pp [] = ""
pp (h:t) = (show h) ++ " " ++ (pp t)

-- What happens when we check whether two queues are equal.
instance Eq a => Eq (Queue a) where
	 (Qu []) == (Qu []) = True
	 (Qu []) == _	    = False
	 _   	 == (Qu []) = False
	 q1	 == q2	    = (dequeue q1) == (dequeue q2)

-- Initialise the queue
emptyQueue :: (Queue a)
emptyQueue = Qu []

-- Queue is empty if its list is empty.
isEmptyQueue :: (Queue a) -> Bool
isEmptyQueue (Qu []) = True
isEmptyQueue _ = False

-- Add the new value to the end of the queue.
--
-- For the head of the list to represent the head of the queue, recurse through
-- the queue, inserting the new item next to the empty list created by emptyQueue.
enqueue :: a -> (Queue a) -> (Queue a)
enqueue val (Qu []) = Qu (val:[])
enqueue val (Qu (h:t)) = Qu (h : q)
	    where (Qu q) = enqueue val (Qu t)

-- Return the value at the head of the list with dequeued queue.
dequeue :: (Queue a) -> (a, Queue(a))
dequeue (Qu []) = error "Can't dequeue an empty queue!"
dequeue (Qu (h:t)) = (h, Qu t)



