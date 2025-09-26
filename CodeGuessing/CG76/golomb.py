table = {1:1}
def entry(x):
    if x in table:
        return table[x]
    
    table[x] = 1 + entry(x - entry(entry(x - 1)))
    return table[x]
