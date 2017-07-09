#!/usr/bin/python3
from utils import *

# NOTE: Só guardar isso aqui pra usar posteriormente
# um_string_de_endereco = "160.164.10.1"
#
# mreq = struct.pack("=4sl", socket.inet_aton(um_string_de_endereco), socket.INADDR_ANY)

# Para fins de dimensionamento, considere que uma chave tem no máximo 40 caracteres e o valor associado tem no máximo 160 caracteres.
# Vamos considerar que esses valores não incluem o caractere nulo ao final, que tem que estar lá sempre. Então serão 41 e 161 bytes no limite em cada caso.


class Connection:
    """docstring for Connection."""
    def __init__(self, *args):
        if isinstance(args, tuple):
            if len(args) == 2:
                host, port = args
            else:
                args = args[0]
                host, port = args.split(":")
        else:
            print_error('Unknow format of args in Connection')
            sys.exit(1)
        self.host = host
        self.port = int(port) if not isinstance(port, int) else port

    def __del__(self):
        pass

    def __str__(self):
        return str(self.host) + ', ' + str(self.port)

class Servent:
    """docstring for Servent."""
    def __init__(self, port, namefile_key, *args):
        self.port = int(port) if not isinstance(port, int) else port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

        # Tratado como conjunto para ignorar repetições
        self.neighborhoods = list()
        set_of_ip_and_port = set()

        # Adiciona IP e porta da vizinhança passado nos argv
        for arg in list(args):
            if len(arg.split(":")) != 2:
                print_error('Error in arg')
                print_bold(arg)
                sys.exit(1)

            # Evita duplicação de conexões
            if arg not in set_of_ip_and_port:
                self.neighborhoods.append(Connection(arg))
                set_of_ip_and_port.add(arg)

        # Lê arquivo key e armazena como dicionário
        self.keys = dict()

        with open(namefile_key) as f:
            for line_read in f:
                # Insanity check
                line = line_read
                # Comentário ou linha vazia
                if not line or line[0] == '#':
                    continue
                # Remove quebra de linha no final
                if line[-1] == '\n':
                    line = line[:-1]

                # Remove duplo espaçamento e divide
                line = " ".join(line.strip().split()).split(" ")
                value = " ".join(line[1:])

                self.keys[line[0]] = value
        # end of with open(...

    def __del__(self):
        print_green('Servent died')
        self.sock.close()

    # Lista os vizinhos
    def list_neighborhoods(self):
        print(BLUE, end="")
        print('IP - port')
        for conn in self.neighborhoods:
            print(conn)
        print(ENDC, end="")

    # Lista as chaves
    def list_keys(self):
        print(BLUE, end="")
        pprint.pprint(self.keys)
        print(ENDC, end="")

    # Adiciona chave
    def add_key(self, key, value):
        if isinstance(key, str) and isinstance(value, str):
            self.keys[key] = value
        else:
            print_error('Unexpected key and value to add')
            print_error(str(key) + ' -- ' str(value))

    def receive_data(self):
        # NOTE: Tornar dinâmico o buffer recebido ou procurar na documentação
        #       se existe um limite máximo
        data, addr = self.sock.recvfrom(1024) # buffer size is 1024 bytes
        print_warning(addr)
        print_bold(data)
        struct_aux = struct.Struct('! H H H H')
        b = ctypes.create_string_buffer(struct_aux.size)
        struct_aux.pack_into(b, 0, *(1, 2, 3, 4))
        print_bold(b.raw)
        self.sock.sendto(b, addr)
        return data

    def send_data(self):
        struct_aux = struct.Struct('! H H H H')
        b = ctypes.create_string_buffer(struct_aux.size)
        struct_aux.pack_into(b, 0, *(1, 2, 3, 4))
        print_bold(b.raw)
        self.sock.sendto(b, (self.host, self.port))
        pass

    def start(self):
        self.sock.bind(("0.0.0.0", self.port))

        # DEBUG
        if DEBUG:
            self.list_neighborhoods()
            self.list_keys()

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
                    print_warning('Something in socket')
                    self.receive_data()
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
