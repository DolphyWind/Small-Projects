def entry():
    seq = set()
    n = 0
    prev = 0
    while True:
        if n == 0:
            seq.add(0)
            prev = 0
        elif prev - n > 0 and prev - n not in seq:
            seq.add(prev - n)
            prev = prev - n
        else:
            seq.add(prev + n)
            prev = prev + n
        yield prev
        n += 1
