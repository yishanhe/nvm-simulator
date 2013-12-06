all : test example1 example2


example1:
	make -f Makefile-example1

example2:
	make -f Makefile-example2

test: 
	make -f Makefile-test


clean: clean-example1 clean-example2 clean-test
clean-example1:
	make clean -f Makefile-example1

clean-example2:
	make clean -f Makefile-example2

clean-test: 
	make clean -f Makefile-test



