#!/usr/bin/python3
from utils import *

class Client:
    """docstring for Client."""
    def __init__(self, host, port):
        print_warning('Client instanced')
        self.server = host
        self.port = int(port) if not isinstance(port, int) else port
        # Isso tá errado, vai abrir só quando vai consultar
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

    def __del__(self):
        print_blue('Client died')
        self.sock.close()

    # Envia uma QUERY ao servent
    def send_query(self, key):
        if len(key) > 0:
            struct_aux = struct.Struct('! H ' + str(len(key) + 1) + 's')
            data = (CLIREQ, bytes(key + '\0', 'ascii'))
            b = ctypes.create_string_buffer(struct_aux.size)
            struct_aux.pack_into(b, 0, *data)
            # Insanity check
            print_bold(b.raw)
            self.sock.sendto(b, (self.server, self.port))
        else:
            print_error('send_query: len == 0')


    def handle_RESPONSE(self, data):
        if data[0] != 0 or data[1] != RESPONSE:
            print_error('Error dude')
        data = data[2:]
        split_pos = 0
        for i in range(len(data)):
            if data[i] == 0:
                split_pos = i + 1
                break
        struct_aux = struct.Struct('! ' + str(split_pos) + 's ' + str(len(data) - split_pos) + 's')
        data = struct_aux.unpack(data)
        print(data[0].decode('ascii'))
        print(data[1].decode('ascii'))

    def receive_data(self):
        # NOTE: Tornar dinâmico o buffer recebido ou procurar na documentação
        #       se existe um limite máximo
        data, addr = self.sock.recvfrom(BUFFER_SIZE) # buffer size is 1024 bytes
        return data

    def get_command(self):
        command = sys.stdin.readline()
        if command[:-1] == 'help':
            # TODO: completar com português correto
            print_blue('<key>')
            print_blue('/quit')
            print()
        elif command[:-1] == '/quit':
            sys.exit()
        else: # QUERY
            self.send_query(command[:-1])

    def start(self):
        # Clear terminal
        print('\033c', end="")
        print_blue('Type "help" for more info!')

        while True:
            socket_list = [sys.stdin, self.sock]

            # stuck in here until a fd is ready
            try:
                read_sockets, write_sockets, error_sockets = select.select(socket_list,[],[])
            except Exception as e:
                print_error('Something wrong in select')
                print_error(socket_list)
                sys.exit()

            for sock in read_sockets:
                if sock == self.sock:
                    print_warning('Receive data')
                    data = self.receive_data()
                    if data[0] != 0 or data[1] != RESPONSE:
                        print_error('Error dude')
                    self.handle_RESPONSE(data)
                elif sock == sys.stdin:
                    self.get_command()
                else:
                    print_error('Unexpected state')
                    print_error(sock)

def main(args):
    if len(args) < 2:
        print_blue('Client')
        print_blue('  USAGE:', end=" ")
        usage = args[0] + ' <IP:port>'
        print_blue(usage)
        sys.exit(0)

    print_warning(args)

    host, port = args[1].split(":")
    client = Client(host, port)

    client.start()

if __name__ == '__main__':
    main(sys.argv)
