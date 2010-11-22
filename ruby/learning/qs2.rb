=begin
this method is much faster than the one in qs.rb
=end

def qs (list)
  if list.empty?
    return list
  else
    l = list.select { |v| v < list.first }
    r = list.select { |v| v > list.first }
    return (qs(l) << list.first) + qs(r)
  end
end

tosort = []
count = 3000
while count > 0
  tosort << rand(10000)
  count -= 1
end

ts = tosort.clone()

puts qs(tosort)
