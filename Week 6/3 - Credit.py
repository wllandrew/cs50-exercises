from cs50 import get_int

number = get_int("Number: ")

def is_valid(number : int) -> bool:
    i = 1
    counter_even = 0
    counter_odd = 0
    while number > 0:
        digit = number % 10
        if i % 2 == 0:
            tmp = digit * 2
            counter_even += tmp if tmp < 10 else (tmp // 10 + tmp % 10)
        else:
            counter_odd += digit
        i += 1
        number = number // 10
    return (counter_even + counter_odd) % 10 == 0

def num_len(number : int) -> int:
    i = 0
    while number > 1:
        i += 1
        number = number // 10
    return i

def last_two(number : int) -> int:
    while number > 100:
        number = number // 10
    return number

if is_valid(number):
    l = num_len(number)
    lt = last_two(number)

    if l == 15 and lt in [34, 37]:
        print("AMEX")
    elif l == 16 and lt in [51, 52, 53, 54, 55]:
        print("MASTERCARD")
    elif l == 13 or l == 16 and lt // 10 == 4:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
