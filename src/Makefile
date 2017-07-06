CLIENT_PATH=client
SERVENT_PATH=servent
COMMON_PATH=common

all:
	@printf '\033[94m'
	@printf 'Make in Client'
	(cd "$(CLIENT_PATH)" && make)
	@printf '\033[92m'
	@printf 'Make in Servent'
	(cd "$(SERVENT_PATH)" && make)
	@printf '\033[0m'

clean:
	@printf 'Make clean in Client'
	@printf '\033[94m'
	(cd "$(CLIENT_PATH)" && make clean)
	@printf '\033[92m'
	@printf 'Make in Servent'
	(cd "$(SERVENT_PATH)" && make clean)
	@printf '\033[0m'

common_copy:
	@if [ -d "$(CLIENT_PATH)/$(COMMON_PATH)" ]; then rm -rf $(CLIENT_PATH)/$(COMMON_PATH); fi;
	@if [ -d "$(SERVENT_PATH)/$(COMMON_PATH)" ]; then rm -rf $(SERVENT_PATH)/$(COMMON_PATH); fi;
	@cp -r "$(COMMON_PATH)" "$(CLIENT_PATH)"
	@cp -r "$(COMMON_PATH)" "$(SERVENT_PATH)"

common_delete:
	@if [ -d "$(CLIENT_PATH)/$(COMMON_PATH)" ]; then rm -rf $(CLIENT_PATH)/$(COMMON_PATH); fi;
	@if [ -d "$(SERVENT_PATH)/$(COMMON_PATH)" ]; then rm -rf $(SERVENT_PATH)/$(COMMON_PATH); fi;