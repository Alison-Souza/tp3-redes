#!/usr/bin/python3
from utils import *

class Connection:
    """docstring for Connection."""
    def __init__(self, *args):
        if len(args) == 2:
            host, port = args
        elif len(args.split(":")) == 2:
            host, port = args.split(":")
        else:
            print(args)
            print_error('Unknow format of args in Connection')
            sys.exit(1)
        self.host = host
        self.port = int(port) if not isinstance(port, int) else port

    def __del__(self):
        pass

class Servent:
    """docstring for Servent."""
    def __init__(self, port, namefile_key, *args):
        self.port = int(port) if not isinstance(port, int) else port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
        self.connections = list()

        for arg in list(args):
            if len(arg.split(":")) != 2:
                print_error('Error in arg')
                print_bold(arg)
                sys.exit(1)

            self.connections.append(arg)
        print_warning(self.connections)

    def __del__(self):
        print_green('Servent died')
        self.sock.close()

    def receive_data(self):
        # NOTE: Tornar dinâmico o buffer recebido ou procurar na documentação
        #       se existe um limite máximo
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
        print_warning(addr)
        print_bold(data)
        return data

    def send_data(self):
        pass

    def start(self):
        self.sock.bind(("0.0.0.0", self.port))

        while True:
            socket_list = [self.sock]

            try:
                read_sockets, write_sockets, error_sockets = select.select(socket_list,[],[])
            except Exception as e:
                print_error('Something wrong in select')
                print_error(socket_list)
                sys.exit(1)

            for sock in read_sockets:
                if sock == self.sock:
                    # TODO: receive_data
                    pass
                elif sock == sys.stdin:
                    # NOTE: Criar comandos?
                    pass
                else:
                    print_error('Unexpected state')
                    print_error(sock)

def main(args):
    if len(args) < 3:
        print_green('Server/Client')
        print_green('  USAGE:', end=" ")
        usage = args[0] + ' <localport> <key-values> <ip1:port1> ... <ipN:portN>'
        print_green(usage)
        sys.exit(0)

    print_warning('args = ', end="")
    print_warning(args)
    port = args[1]
    namefile_key = args[2]

    servent = Servent(port, namefile_key, *args[3:])

    servent.start()

if __name__ == '__main__':
    main(sys.argv)
