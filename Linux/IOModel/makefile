all: noBlock selectd
noBlock: noBlock.cc
	g++ $^ -o $@
selectd: selectSvr.cc
	g++ $^ -o $@
epolld: epollSvr.cc
	g++ $^ -o $@

.PHONY:clean
clean: 
	rm -rf noBlock selectd epolld
