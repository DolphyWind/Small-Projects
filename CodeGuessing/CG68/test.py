import cg68
import random
import generation

def main():
    example_text = "wonder woman in a wonderful universe"
    # example_text = "wonder woman"
    random.seed(42)
    table = generation.generate_random_subs_table()

    ciphered = generation.cipher(example_text, table)
    words = []
    with open('words.txt', 'r') as f:
        words = f.read().split('\n')[:-1]
    words = [word for word in words if ' ' not in word]
    words = set(words)

    print(ciphered)
    out = cg68.entry(ciphered, words)
    print(out)

if __name__ == '__main__':
    main()
