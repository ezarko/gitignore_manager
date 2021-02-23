all: gim

gim: obj/Fragment.o obj/Manager.o obj/main.o
	c++ -o $@ $^

clean:
	rm -rf obj gim

obj:
	mkdir obj

obj/Fragment.o: ../../lib/Fragment.cpp obj
	c++ -c -o $@ $<

obj/Manager.o: ../../lib/Manager.cpp obj
	c++ -c -o $@ $<

obj/main.o: main.cpp
	c++ -I../../lib -c -o $@ $<
