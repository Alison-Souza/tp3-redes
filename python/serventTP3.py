#!/usr/bin/python3
from utils import *

# UDP_IP = "127.0.0.1"
# UDP_PORT = 5005
#
# sock = socket.socket(socket.AF_INET, # Internet
#                   socket.SOCK_DGRAM) # UDP
# sock.bind((UDP_IP, UDP_PORT))
#
# while True:
#     data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
#     print("received message:", data)

# class Servent:
#     """docstring for Servent."""
#     def __init__(self, arg):
#         super(Servent, self).__init__()
#         self.arg = arg

def main(args):
    if len(args) < 3:
        print_green('Server/Client')
        print_green('  USAGE:', end=" ")
        usage = args[0] + ' <localport> <key-values> <ip1:port1> ... <ipN:portN>'
        print_green(usage)
        sys.exit(0)

    print_warning(args)

if __name__ == '__main__':
    main(sys.argv)
