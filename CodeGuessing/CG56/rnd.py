import sys
import string
import random

def main():
    n = int(sys.argv[1])
    print(''.join(random.choices(string.ascii_uppercase, k=n)),end='')

if __name__ == "__main__":
    main()
