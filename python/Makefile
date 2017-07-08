all:
	@echo 'None'
	@python3 serventTP3.py 5000 key.txt
	@python3 clientTP3.py 127.0.0.1:5000

client:
	@python3 clientTP3.py 127.0.0.1:5000

servent:
	@python3 serventTP3.py 5000 key.txt

test:
	@python3 serventTP3.py 5000 key.txt 127.0.0.1:5001 127.0.0.1:5002 127.0.0.1:5003 127.0.0.1:5004
