def imsorry(really, sorry):
    i = sorry
    while really[i] == 0 and i >= 0:
        really[i] = 2
        i -= 1
    i = sorry + 1
    while really[i] == 0 and i < len(really):
        really[i] = 2
        i += 1
    return really
