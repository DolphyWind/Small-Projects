import string
import random

def generate_random_subs_table() -> dict[int, int]:
    alphabet = string.ascii_lowercase
    target = list(alphabet)
    random.shuffle(target)
    target = ''.join(target)
    return str.maketrans(alphabet, target)

def cipher(text: str, table: dict[int, int]):
    return text.translate(table)

def decipher(text: str, table: dict[int, int]):
    inv_table = {}
    for k, v in table.items():
        inv_table[v] = k

    return cipher(text, inv_table)

