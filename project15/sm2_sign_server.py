import secrets

from sm2_signature import *
import socket
import time

HOST = ''
POST = 50007
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, POST))
s.listen(1)
print('Listening on port:', POST)

str, addre = s.accept()
str_P_1_data = str.recv(1024).decode()
place = str_P_1_data.find('||')
P_1 = (int(str_P_1_data[:place]), int(str_P_1_data[place + 2:]))
print("接收到客户端传来的 P_1 = {}".format(P_1))

d_2, P_2 = Key_Generation_server(P_1)
print("服务端生成公私钥成功！")

# time.sleep(2)

str_Q_1_e_data = str.recv(1024).decode()
p1 = str_Q_1_e_data.find('||')
p2 = str_Q_1_e_data.find('**')
Q_1 = (int(str_Q_1_e_data[:p1]), int(str_Q_1_e_data[p1 + 2: p2]))
e = int(str_Q_1_e_data[p2 + 2:])
print("接收到客户传来的\nQ_1 = {}\ne = {}".format(Q_1, e))

k_2 = int(secrets.randbelow(N))
Q_2 = mul_point(k_2, G)
k_3 = int(secrets.randbelow(N))
x_1, y_1 = add_point(mul_point(k_3, Q_1), Q_2, P)
r = (x_1 + e) % N
s_2 = (d_2 * k_3) % N
s_3 = (d_2 * (r + k_2)) % N
r_s2_s3 = '{}||{}**{}'.format(r, s_2, s_3)
print("服务端计算的\nr = {}\ns_2 = {}\ns_3 = {}".format(r, s_2, s_3))
str.sendall(r_s2_s3.encode())
