import random

for _ in range(9):
    input()

P = 0.2
M = 0.45
I = 0.35

M_4 = M / 4
I_4 = I / 4

l = ['P', 'M\nU', 'M\nD', 'M\nL', 'M\nR', 'I\nU', 'I\nD', 'I\nL', 'I\nR']
w = [P, M_4, M_4, M_4, M_4, I_4, I_4, I_4, I_4]

print(*random.choices(l, w, k=1))
