-- Copyright David Petrie 2008
--
module Stack(Stack, pop, push, isEmptyStack, emptyStack) 
where

newtype Stack a = St [a]

instance Show a => Show (Stack a) where
	 show (St l) = pp l

pp [] = ""
pp (h:t) = (show h) ++ " " ++ (pp t)

instance Eq a => Eq (Stack a) where
	 (St []) == (St []) = True
	 (St []) == _	    = False
	 _   	 == (St []) = False
	 s1	 == s2	    = (pop s1) == (pop s2)

emptyStack :: (Stack a)
emptyStack = St []

isEmptyStack :: (Stack a) -> Bool
isEmptyStack (St []) = True
isEmptyStack _ = False

push :: a -> (Stack a) -> (Stack a)
push val (St st) = St (val:st)

pop :: (Stack a) -> (a, Stack(a))
pop (St []) = error "Can't pop an empty stack!"
pop (St (h:t)) = (h, St t)