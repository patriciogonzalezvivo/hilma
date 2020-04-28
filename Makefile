scripts=$(wildcard test/*.py)

install:
	swig -c++ -python hilma.i
	python3 setup.py build_ext --inplace
	$(LIB)
	@rm -rf build
	sudo python3 setup.py install

clean:
	@rm -rvf $(LIB) *.o */*.o *.so hilma.py* hilma_wrap.c* build

test: $(scripts)

$(scripts): hilma.py
	@python $@
