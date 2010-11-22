=begin
this is a comment block

keeps on going and going and going...
=end

mylist = [1,2,2,3,5,6,7]

# my comment
def myfunc(name)
  print "hello %s\n" % name
end

# pretty print
def pp(list)
  print list.join("\n")
  print ("\n")
end

myfunc("world")

# pass by value or pass by reference?
# a: reference
def myNextFunc(list, last)
  list << last
end

#pp(mylist)

myNextFunc(mylist, 10)
#pp(mylist)

=begin
mylist.each do |item|
  print "%d\n" % (item * 3)
end
=end

tosort = []
count = 3000
while count > 0
  tosort << rand(1000)

  count -= 1
end

f = File.new("out", "w")

f.write(tosort.join("\n"))

f2 = File.new("out", "r")
fileList = f2.readlines()

puts fileList

#puts qs(tosort)
