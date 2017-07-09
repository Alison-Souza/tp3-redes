# Anotações

- A comunicação entre os programas cliente e o seu ponto de contato e entre os pares se dá através de mensagens UDP.
- O cliente envia uma mensagem UDP para o seu ponto de contato contendo apenas um campo de tipo de mensagem (uint16_t) com valor 1 (CLIREQ) e o texto da chave.
- Se ele esperar por 4 segundos e não receber nenhuma resposta, ele deve retransmitir a consulta uma vez apenas e voltar a esperar.
- Se ele receber uma resposta, ele não sabe quantas outras respostas ele pode receber, pois vários nós podem conter dados para uma mesma chave.
- Sendo assim, seu cliente deve entrar em um loop lendo as mensagens de resposta que porventura receba, até que ele fique esperando por 4 segundos sem receber novas respostas.
- À medida que as respostas cheguem ele pode exibi-las para o usuário, indicando que par respondeu (definido pelo par IP:porto) e ao final do tempo de espera indicar que não há mais respostas.
