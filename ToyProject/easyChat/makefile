all: epolld client
epolld: epollSvr.cc
	g++ $^ -o $@ -L/usr/lib64/mysql -lpthread -ljsoncpp -lmysqlclient
client: chatClient.cc
	g++ $^ -o $@ -ljsoncpp

.PHONY:clean
clean: 
	rm -rf epolld client
