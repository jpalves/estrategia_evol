all:
	swig -python -c++ fpga3.i
	g++ -fPIC -c fpga3.cpp  fpga3_wrap.cxx  -I/usr/include/python3.8
	#gcc -fPIC -c fpga3_wrap.c -I/usr/include/python3.8
	g++ -shared fpga3.o fpga3_wrap.o -o _fpga3.so

clean:
	rm *.cxx *.so *.o
