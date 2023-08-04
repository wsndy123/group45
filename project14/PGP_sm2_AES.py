from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Protocol.KDF import PBKDF2
from Crypto.Util.Padding import pad, unpad
from gmssl import sm2
import time

sen_pubkey = 'B9C9A6E04E9C91F7BA880429273747D7EF5DDEB0BB2FF6317EB00BEF331A83081A6994B8993F3F5D6EADDDB81872266C87C018FB4162F5AF347B483E24620207'
sen_prikey = '00B9AB0B828FF68872F21A837FC303668428DEA11DCD1B24429D0C99E24EED83D5'
rec_pubkey = '893cb9392dabd2fac095f657a7e0bc308e32f4b79380d478547f57123dccb3bc4a3a2d009f5826b6624c99dd41baf470a8baf46722f2d36b1d26f19af112c5cd'
rec_prikey = '228a9707053e1b333fb8cb839567a9db4ca1cf5381e9a6a539774e6c3563cdfa'
AES_key = 'this is aes huihua key'

def generate_key(password, salt):
    kdf = PBKDF2(password, salt, dkLen=32, count=100000)
    return kdf

def AES_encrypt(plain_text, password):
    salt = get_random_bytes(16)
    key = generate_key(password, salt)
    cipher = AES.new(key, AES.MODE_CBC)
    padded_plain_text = pad(plain_text.encode('utf-8'), AES.block_size)
    cipher_text = salt + cipher.iv + cipher.encrypt(padded_plain_text)
    return cipher_text.hex()

def AES_decrypt(cipher_text_hex, password):
    cipher_text = bytes.fromhex(cipher_text_hex)
    salt = cipher_text[:16]
    iv = cipher_text[16:32]
    cipher_text = cipher_text[32:]
    key = generate_key(password, salt)
    cipher = AES.new(key, AES.MODE_CBC, iv=iv)
    padded_plain_text = cipher.decrypt(cipher_text)
    plain_text = unpad(padded_plain_text, AES.block_size)
    return plain_text.decode('utf-8')

def sm2_encrypt(data, public_key):
    sm2_crypt = sm2.CryptSM2(
        public_key=public_key,
        private_key=None,
    )
    data = bytes(data, encoding='utf-8')
    enc_data = sm2_crypt.encrypt(data)
    return enc_data.hex()


def sm2_decrypt(encrypted_data, private_key):
    sm2_crypt = sm2.CryptSM2(
        public_key='asdasd',
        private_key=private_key,
    )
    encrypted_data = bytes.fromhex(encrypted_data)
    dec_data = sm2_crypt.decrypt(encrypted_data)
    return dec_data.decode('utf-8')
start = time.time()
# 发送方将AES密钥加密AES_key
# sm2 rec_pubkey
print("AES会话密钥为：{}".format(AES_key))
sm2_to_AESkey = sm2_encrypt(AES_key, rec_pubkey)
message = "how are you?"
# 发送方使用AES会话密钥加密消息
print("加密前的消息为：{}".format(message))
AES_to_message = AES_encrypt(message, AES_key)

# 接收方解密AES会话密钥AES_key_get
AES_key_get = sm2_decrypt(sm2_to_AESkey, rec_prikey)
print("解密得到的AES会话密钥为：{}".format(AES_key_get))
# 使用AES会话密钥解密明文
message_get = AES_decrypt(AES_to_message, AES_key_get)
print("解密得到的消息为：{}".format(message_get))

end = time.time()
print("所用时间为：{} s".format(end - start))