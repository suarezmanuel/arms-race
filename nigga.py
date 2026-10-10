# import math
# def binary(number):
#     s = ""
#     n = number
#     expo = math.floor(math.log2(number))
#     for i in range(expo+1):
#         s = str(math.floor(n%2)) + s
#         n/=2
#     return s

def decimal_to_base(number, base):
    s = ""
    n = number
    while n > 0:
        s = str(n % base) + s
        n = n // base
    return s

print(decimal_to_base(7, 4))