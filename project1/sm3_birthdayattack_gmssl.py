from gmssl import sm3, func
import random
import time


def SM3_birthdayattack(n):
    while True:
        x = random.random()
        y = random.random()
        if x == y: continue
        h1 = sm3.sm3_hash(func.bytes_to_list(bytes(str(x), encoding='utf-8')))
        h2 = sm3.sm3_hash(func.bytes_to_list(bytes(str(y), encoding='utf-8')))
        ha = int(h1, 16)
        hb = int(h2, 16)
        b1 = str(bin(ha))
        b2 = str(bin(hb))
        if b1[:n] == b2[:n]:
            break


count = 0

for nbit in range(15, 21):
    start = time.time()
    SM3_birthdayattack(nbit)
    end = time.time()
    runtime = (end - start) / 10
    print("{} bit碰撞找到，用时{}ms".format(nbit, runtime * 1000))

