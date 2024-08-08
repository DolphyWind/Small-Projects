import random

def main():
    filename = "olus2000.h"
    with open(filename, "r") as file:
        lines = file.readlines()

    random.shuffle(lines)
    print(''.join(lines))

if __name__ == "__main__":
    main()
