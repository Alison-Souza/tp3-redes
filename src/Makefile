all:
	@echo 'None'
	@python3 serventTP3.py 5000 key.txt
	@python3 clientTP3.py 127.0.0.1:5000

client:
	@python3 clientTP3.py 127.0.0.1:5000

servent:
	@python3 serventTP3.py 5000 key.txt

test1a:
	@python3 serventTP3.py 5000 key.txt 127.0.0.1:5001

test1b:
	@python3 serventTP3.py 5001 key2.txt 127.0.0.1:5000

# @python3 serventTP3.py 5000 key.txt 127.0.0.1:5001 &
# @python3 serventTP3.py 5001 key.txt 127.0.0.1:5000 127.0.0.1:5002 &
# @python3 serventTP3.py 5002 key.txt 127.0.0.1:5001 127.0.0.1:5003 &
# @python3 serventTP3.py 5003 key.txt 127.0.0.1:5002 127.0.0.1:5004 &
# @python3 serventTP3.py 5004 key.txt 127.0.0.1:5003
