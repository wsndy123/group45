from sm2_signature import *

while True:
    key = Key_Generation()
    print('私钥为: {}\n公钥为: {}'.format(key[0], key[1]))
    message = input("输入消息为：")
    ID = input("ID为： ")
    Z_A = Precompute(ID, A, B, G_X, G_Y, key[1][0], key[1][1])
    signature = Sign(key[0], message, str(Z_A))
    print("消息的数字签名为", signature)
    if Verify(key[1], ID, message, signature) == 1:
        print("验证通过!!")
    else:
        print("验证失败!!")
    print("-------------------------------------------------------------------------------------------------------------")
