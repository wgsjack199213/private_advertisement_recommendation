from Crypto.Cipher import AES
import os

# Convert an array of ASCII values to a string
def get_str(arr):
    arr = map(chr, arr)
    return ''.join(arr)

# Convert each char of a string to its ASCII value
def get_num(str):
    for k in str:
        print ord(k),
    print ''

key = [0, 6, 0, 8, 0, 3, 3, 0, 79, 0, 15, 0, 0, 0, 4, 2]
key_str = get_str(key)
counter_str = get_str([0 for k in range(16)])

print "Suppose we want to encrypt 3 integers: 0, 3, 8"
input_str = get_str([0, 3, 8])

secret = os.urandom(16)
crypto = AES.new(key_str, AES.MODE_CTR, counter=lambda: counter_str)
encrypted = crypto.encrypt(input_str)

print "The 3 numbers of the ciphter text are:"
get_num(encrypted)

crypto = AES.new(key_str, AES.MODE_CTR, counter=lambda: counter_str)
decrypted = crypto.decrypt(encrypted)

print "After derypting the ciphertext, we can get:"
get_num(decrypted)
