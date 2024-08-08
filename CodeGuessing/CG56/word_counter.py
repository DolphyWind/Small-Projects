import string
import sys

def main():
    argv = sys.argv
    argc = len(argv)
    if argc == 1:
        print(f"Usage {argv[0]} filenme")
        exit(1)

    filename = argv[1]
    remove_punct_table = str.maketrans('', '', string.punctuation)
    lowercase_table = str.maketrans(string.ascii_uppercase, string.ascii_lowercase)
    text = ''
    with open(filename, 'r') as f:
        text = f.read()

    if text.endswith('\n'):
        text = text[:-1]

    text = text.translate(remove_punct_table).translate(lowercase_table)

    words = {}
    for w in text.split():
        try:
            words[w] += 1
        except KeyError:
            words[w] = 1

    names = sorted(list(words.keys()))
    for w in names:
        print(f'{w}: {words[w]}')


if __name__ == "__main__":
    main()
