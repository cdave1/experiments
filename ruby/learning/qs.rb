def qs (list)
  if list.empty?
    return list
  else
    pivl = piv(list, [], Proc.new{ |val| val < list.first })
    pivg = piv(list, [], Proc.new{ |val| val > list.first })
    return (qs(pivl) << list.first) + qs(pivg)
  end
end

def piv(list, newlist, cond)
  if list.empty?
    return newlist
  else
    if cond.call(list.first)
      newlist << list.first
    end
    t = tail(list)
    piv(t, newlist, cond)
  end
end

def tail(list)
  return list.slice(1...list.length())
end

tosort = []
count = 3000
while count > 0
  tosort << rand(1000)
  count -= 1
end

puts qs(tosort)
