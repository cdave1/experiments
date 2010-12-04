start = 1.0
end = 0.2
iterations = 5
distance = end-start

for i in range(iterations):
	#start = start + (start * (distance/start/iterations))
	#start = start + (start * ((distance/start)/iterations))
	scaleAmount = (distance/iterations)
	start += scaleAmount
	print start, scaleAmount
