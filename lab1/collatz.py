import sys

def collatz(n):
    iterations = 1
    while n != 1:
        iterations += 1
        if n%2 == 0:
            n = n//2

        else:
            n = 3*n+1

    return iterations

print(collatz(int(sys.argv[1])))
