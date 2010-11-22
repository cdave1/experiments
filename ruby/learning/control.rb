tosort = []
count = 3000
while count > 0
  tosort << rand(10000)
  count -= 1
end

puts tosort.sort()
