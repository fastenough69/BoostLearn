OS := $(shell_uname)

all: get_req

get_req:
	ifeq($(OS), Linux)
		sudo apt-get install libboost-system-dev libboost-asio-dev
	endif

