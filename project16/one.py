import socket
from sm2_decrypt import *
import time


if __name__ == "__main__":
    start = time.time()
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print("连接成功！")
    d1, P1 = Gen_Key_one()
    data = str(P1[0]) + '||' + str(P1[1])
    s.sendto(data.encode(), ("127.0.0.1", 12300))
    data, addr = s.recvfrom(1024)
    data = data.decode()
    flag = data.index('||')
    pk = (int(data[:flag]), int(data[flag + 2:]))
    M = 'asdasdasdas'
    C1, C2, C3 = SM2_enc(M, pk)
    T1 = C_to_T1(d1, C1)
    data = str(T1[0]) + '||' + str(T1[1])
    s.sendto(data.encode(), addr)
    data, addr = s.recvfrom(1024)
    data = data.decode()
    flag = data.index('||')
    T2 = (int(data[:flag]), int(data[flag + 2:]))
    M_ = dec(T2, C1, C2, C3)
    print("解密结果: ", M_)
    s.close()
    print("成功！连接断开")
    end = time.time()
    print(end - start)
