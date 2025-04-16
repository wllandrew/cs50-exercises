from cs50 import get_int
from sys import exit

while True:
    height = get_int("Height: ")
    if height < 9 and height > 0:
        break

for n in range(1, height + 1):
    print((height - n) * " " + n * "#" + "  " + n * "#")

