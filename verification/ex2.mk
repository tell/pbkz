include common.mk

ex_2-1: FORCE
	echo "#2.1# Generate an LLL-reduced basis of SVP Challenge basis with dim=100 and seed=0."
	$(PBKZ) -genbasis -type svp -dim 100 -seed 0 -of svpc100.txt -lll -of lll100.txt

ex_2-2: FORCE ex_2-1
	echo "#2.2# Generate BKZ blocksize strategy to get the first vector of output basis shorter than 1.05GH with ENUM."
	cp -f $(DATAFILE_0) .
	$(PBKZ) -if lll100.txt -genstrategy -type shortvec -a 1.05 -withenum -of str100_1.05gh -nt 6 -stopatfound -lf svp100.log -ll 1

ex_2-3: FORCE ex_2-2
	echo "#2.3# Change the file system modes of the output shell script and run it to find a vector shorter than 1.05GH."
	chmod u+x str100_1.05gh.sh
	./str100_1.05gh.sh

run: FORCE ex_2-3
	cat svp100.log
