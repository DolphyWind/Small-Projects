def longest_seq(arr, block_size):
    rev = arr[::-1]
    block = rev[:block_size]
    size = 1
    while block == rev[block_size * size : block_size * (size + 1)]:
        size += 1
    return size

def elem_next(arr):
    block_len = len(arr) // 2
    
    return max([1] + [
        longest_seq(arr, i)
        for i in range(1, block_len + 1)
    ])

def entry(n):
    arr = [1]
    while len(arr) < n:
        x = elem_next(arr)
        arr.append(x)

    return arr

