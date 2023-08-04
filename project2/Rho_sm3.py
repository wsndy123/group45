from gmssl import sm3, func
import random
import time


def Rho(n):
    x = random.random()
    h1 = sm3.sm3_hash(func.bytes_to_list(bytes(str(x), encoding='utf-8')))
    h2 = sm3.sm3_hash(func.bytes_to_list(bytes(str(h1), encoding='utf-8')))
    while 1:
        h1 = sm3.sm3_hash(func.bytes_to_list(bytes(str(h1), encoding='utf-8')))
        tmp = sm3.sm3_hash(func.bytes_to_list(bytes(str(h2), encoding='utf-8')))
        h2 = sm3.sm3_hash(func.bytes_to_list(bytes(str(tmp), encoding='utf-8')))
        if h1[:n] == h2[:n]:
            break


if __name__ == "__main__":
    for n in [1, 2, 3, 4]:
        start = time.time()
        Rho(n)
        end = time.time()
        print("{} bit的Rho方法攻击，用时: {} s".format(n * 8, end - start))

