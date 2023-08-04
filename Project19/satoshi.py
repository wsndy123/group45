from base import *
import random
import time

if __name__ == "__main__":
    start = time.time()
    prikey, pubkey = Key_Generation()
    print("生成的 Satoshi 的公钥为:", pubkey)
    u = random.randint(1, N - 1)
    v = random.randint(1, N - 1)
    R = add_point(mul_point(u, G), mul_point(v, pubkey), P)
    r = R[0] % N
    s = (r * calc_inverse(v, N)) % N
    sign = (r, s)
    print("生成的签名为:", sign[0])
    e = (r * u * calc_inverse(v, N)) % N
    r_ = (mul_point(calc_inverse(s, N), add_point(mul_point(e, G), mul_point(r, pubkey), P)))[0] % N
    print("伪造的签名问:", r_)
    if r_ == r:
        print("签名伪造成功！")
    else:
        print("伪造失败")
    end = time.time()
    print("所用时间为: {} s".format(end - start))