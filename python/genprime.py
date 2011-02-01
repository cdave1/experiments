def genPrimes(n):
    primes = [2]
    for i in range(3,n,2):
        addprime = 1
        for prime in primes:
            if (i % prime == 0):
                addprime = 0
                break
        if addprime == 1:
            primes.append(i)

    for i in primes:
        print i

genPrimes(100)
genPrimes(1000)
genPrimes(10000)
genPrimes(100000)

# would be faster by iterating through a range and calling Miller-Rabin
