all:
	@echo 'Olá Mario, você está atrasado. A princesa está em outro castelo!'
	@echo 'Veja mais opções no Makefile'

client:
	@python3 clientTP3.py 127.0.0.1:5000

servent:
	@python3 serventTP3.py 5000 key.txt

test2a:
	@python3 serventTP3.py 5000 key.txt 127.0.0.1:5001

test2b:
	@python3 serventTP3.py 5001 key2.txt 127.0.0.1:5000

test3a:
	@python3 serventTP3.py 5000 key.txt 127.0.0.1:5001

test3b:
	@python3 serventTP3.py 5001 key.txt 127.0.0.1:5002

test3c:
	@python3 serventTP3.py 5002 key.txt 127.0.0.1:5003

test3d:
	@python3 serventTP3.py 5003 key.txt 127.0.0.1:5004

test3e:
	@python3 serventTP3.py 5004 key4.txt

test3:
	@python3 serventTP3.py 5000 key.txt 127.0.0.1:5001 &
	@python3 serventTP3.py 5001 key2.txt 127.0.0.1:5002 &
	@python3 serventTP3.py 5002 key.txt 127.0.0.1:5003 &
	@python3 serventTP3.py 5003 key.txt 127.0.0.1:5004 &
	@python3 serventTP3.py 5004 key4.txt

test4:
	@python3 serventTP3.py 5000 key.txt 127.0.0.1:5001  &
	@python3 serventTP3.py 5001 key2.txt 127.0.0.1:5000 127.0.0.1:5002 &
	@python3 serventTP3.py 5002 key.txt 127.0.0.1:5000 127.0.0.1:5001

kill:
	./kill.sh
