import secrets
import hashlib
from math import gcd

A = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
B = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
P = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
G_X = 0x32c4ae2c1f1981195f9904466a39c9948fe30bbff2660be1715a4589334c74c7
G_Y = 0xbc3736a2f4f6779c59bdcee36b692153d0a9877cc62a474002df32e52139f0a0
N = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
G = (G_X, G_Y)

def calc_inverse(M, m):
    if gcd(M, m) != 1:
        return None
    u1, u2, u3 = 1, 0, M
    v1, v2, v3 = 0, 1, m
    while v3 != 0:
        q = u3 // v3
        v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3
    return u1 % m

def frac_to_int(up, down, p):
    num = gcd(up, down)
    up //= num
    down //= num
    return up * calc_inverse(down, p) % p

def add_point(P_, Q_, p):
    if P_ == 0:
        return Q_
    if Q_ == 0:
        return P
    x1, y1, x2, y2 = P_[0], P_[1], Q_[0], Q_[1]
    e = frac_to_int(y2 - y1, x2 - x1, p)
    x3 = (e*e - x1 - x2) % p
    y3 = (e * (x1 - x3) - y1) % p
    ans = (x3, y3)
    return ans

def double_point(P_, p, a):
    if P_ == 0:
        return P_
    x1, y1 = P_[0], P_[1]
    e = frac_to_int(3 * x1 * x1 + a, 2 * y1, p)
    x3 = (e * e - 2 * x1) % p
    y3 = (e * (x1 - x3) - y1) % p
    Q_ = (x3, y3)
    return Q_

def mul_point(s, p):
    n = p
    ret = 0
    for i in reversed(list(bin(s)[2:])):
        if i == '1':
            ret = add_point(ret, n, P)
        n = double_point(n, P, A)
    return ret

def inv_point(p):
    x = p[0]
    y = P - p[1]
    return (x, y)

def ENTL_(id_str):
    id_bytes = id_str.encode('utf-8')
    id_length = len(id_bytes)
    entl_bytes = id_length.to_bytes(2, byteorder='big')
    return entl_bytes

def calculate_sha256_hash(input_str):
    input_bytes = bytes(input_str.encode('utf-8'))
    sha256_hash = hashlib.sha256()
    sha256_hash.update(input_bytes)
    hash_hex = sha256_hash.hexdigest()
    return int(hash_hex,16)

def Key_Generation():
    private_key = int(secrets.token_hex(32), 16)
    public_key = mul_point(private_key, G)
    return private_key, public_key

def ECDSA_sign(m, sk):
    k = secrets.randbelow(N)
    R = mul_point(k, G)
    r = R[0] % N
    e = calculate_sha256_hash(m)
    s = calc_inverse(k, N) * (e + sk * r) % N
    return (r, s)