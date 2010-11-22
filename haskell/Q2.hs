-- Copyright David Petrie 2008
--
-- Haskell queue implementation - using two lists

module Q2 (Queue, emptyQueue, isEmptyQueue, enqueue, dequeue)
where

-- Queue is make up of a front and a back.
-- First list is front, second is back.
data Queue a = Qu [a] [a]

-- Show contents of the queue - the head of the queue comes first.
-- Also draw a split between the back and front queues.
instance Show a => Show (Queue a) where
	 show (Qu front back) = pp back ++ " - " ++ pp (reverse front)
pp [] = ""
pp (h:t)  = (show h) ++ " " ++ (pp t)

-- 
instance Eq a => Eq (Queue a) where
	 (Qu [] []) == (Qu [] []) = True
	 (Qu [] []) == _	    = False
	 _   	 == (Qu [] []) = False
	 q1	 == q2	    = (dequeue q1) == (dequeue q2)

-- Initialise an empty queue with empty front and back.
emptyQueue :: (Queue a)
emptyQueue = Qu [] []

-- Empty if both front and back are empty
isEmptyQueue :: (Queue a) -> Bool
isEmptyQueue (Qu [] []) = True
isEmptyQueue _ = False

-- Cons val on to the front queue
enqueue :: a -> (Queue a) -> (Queue a)
enqueue val (Qu front back) = Qu (val:front) back

-- Four situations to deal with:
-- - back is empty, in which case switch back with reverse of front and repeat dequeue
-- - Both full: return head of back, together with a queue made of front,
--   and tail of back.
-- - front empty: return head of back with a queue make of an empty list and
--   the tail of back.
-- - both empty: error
dequeue :: (Queue a) -> (a, Queue(a))
dequeue (Qu (f:fs) []) = dequeue (Qu [] (reverse (f:fs)))
dequeue (Qu (f:fs) (b:bs)) = (b, (Qu (f:fs) bs))
dequeue (Qu [] (b:bs)) = (b, (Qu [] bs))
dequeue (Qu [] []) = error "Can't dequeue an empty queue!"



