.PHONY:all
all: thread thread_b

atomic: atomic.cc
	g++ $^ -o $@
thread:: thread.cc
	g++ $^ -o $@
thread_b:: thread_b.cc
	g++ $^ -o $@

.PHONY:clean
clean:
	rm thread thread_b

