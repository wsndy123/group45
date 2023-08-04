from sm2_signature import *
import socket
import sys
import time

HOST = '127.0.0.1'
PORT = 50007
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
start = time.time()
try:
    s.connect((HOST, PORT))
except Exception as e:
    print("连接服务端失败！")
    sys.exit()
print("连接服务端成功！")

d_1, P_1 = Key_Generation()
print("客户端生成的\n私钥为：{}\n公钥为：{}".format(d_1, P_1))
str_P_1 = '{}||{}'.format(P_1[0], P_1[1])
s.sendall(str_P_1.encode())

Z = "ID_server||ID_client"
M = 'How are you?'
M_ = Z + M
e = calculate_sha256_hash(M_)
k_1 = int(secrets.randbelow(N))
Q_1 = mul_point(k_1, G)
Q_1_e = '{}||{}**{}'.format(Q_1[0], Q_1[1], e)
s.sendall(Q_1_e.encode())
print("客户端生成的\nk_1 = {}\nQ_1 = {}\ne = {}".format(k_1, Q_1, e))

r_s2_s3_data = s.recv(1024).decode()
p1 = r_s2_s3_data.find('||')
p2 = r_s2_s3_data.find('**')
r = int(r_s2_s3_data[:p1])
s_2 = int(r_s2_s3_data[p1 + 2: p2])
s_3 = int(r_s2_s3_data[p2 + 2:])
print("客户端接收的\nr = {}\ns_2 = {}\ns_3 = {}".format(r, s_2, s_3))

s = ((((d_1 * k_1) * s_2 + d_1 * s_3 - r) % N) + r) % N
print("生成的数字签名为：{}".format(s))

end = time.time()
print("{:.6f}".format(end - start))