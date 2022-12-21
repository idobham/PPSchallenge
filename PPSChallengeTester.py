import string
import random
import itertools
import socket
import configparser

config = configparser.ConfigParser()
config.read('fconfig.ini')
string_pfx1 = config.get('prefix', 'p1')
string_pfx2 = config.get('prefix', 'p2')
string_port = config.get('address', 'port')

print(string_pfx1, string_pfx2, string_port)
s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

print("Socket Created Successfully")
print("Socket Created Successfully")

host = '127.0.0.1:9000'
t = int(string_port)

s.bind(host)
s.listen(5)

print("Socket Successfully binded to %s"% port)

def get_random_string(length):
    # choose from all lowercase letters
    letters = string.ascii_lowercase
    result_str = ''.join(random.choice(letters) for i in range(length))
    a = string_pfx1 + "." + result_str
    c.send(a.encode)


result_num = random.randrange(1,999999999)
b = string_pfx2 + "." + str(result_num)

while True:
    c, addr = s.accept()
    length = random.randint(1,25)
    get_random_string(length)
    c.send(b.encode())
 

    





