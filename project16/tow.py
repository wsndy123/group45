import socket
from sm2_decrypt import *



if __name__ == "__main__":
    # 建立连接
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('', 12300))
    print("Listening at post 12300")
    # 【Gen_Key】
    data, addr=s.recvfrom(1024)
    data = data.decode()
    flag = data.index('||')
    P1 = (int(data[:flag]), int(data[flag + 2:]))
    d2, P = Gen_Key_tow(P1)
    data = str(P[0]) + '||' + str(P[1])
    s.sendto(data.encode(), addr)

    #【P1_to_P2】
    data, addr=s.recvfrom(1024)
    data = data.decode()
    flag = data.index('||')
    T1 = (int(data[:flag]), int(data[flag + 2:]))
    T2 = T1_to_T2(T1, d2)
    data = str(T2[0]) + '||' + str(T2[1])
    s.sendto(data.encode(), addr)

    s.close()
    print("连接断开")

