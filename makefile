CFLAGS= -g -Wall -Wextra  -O0

ray_tracing_test: ray_tracing_main.o ray_tracing.o
	g++ $(CFLAGS) -o ray_tracing_test ray_tracing_main.o ray_tracing.o

ray_tracing_main.o: ray_tracing_main.cpp ray_tracing.hpp
	g++ $(CFLAGS) -c ray_tracing_main.cpp

ray_tracing.o: ray_tracing.cpp ray_tracing.hpp
	g++ $(CFLAGS) -c ray_tracing.cpp

clean:
	rm test.ppm ray_tracing_main.o ray_tracing_test.exe ray_tracing.o
 