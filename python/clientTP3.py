#!/usr/bin/python3
from utils import *

# UDP_IP = "127.0.0.1"
# UDP_PORT = 5005
# MESSAGE = "Hello, World!"
#
# print("UDP target IP:", UDP_IP)
# print("UDP target port:", UDP_PORT)
# print("message:", MESSAGE)
#
# sock = socket.socket(socket.AF_INET, # Internet
#                   socket.SOCK_DGRAM) # UDP
# sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

def main(args):
    if len(args) < 2:
        print_blue('Client')
        print_blue('  USAGE:', end=" ")
        usage = args[0] + ' <IP:port>'
        print_blue(usage)
        sys.exit(0)

    print_warning(args)

if __name__ == '__main__':
    main(sys.argv)
