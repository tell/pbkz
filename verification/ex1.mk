include common.mk

ex_1-1: FORCE
	echo "#1.1# Generate an LLL-reduced basis of Ideal Lattice Challenge basis with index=421 and seed=0."
	$(PBKZ) -genbasis -type ideal -index 421 -seed 0 -of isvpc421.txt -lll -si 10 -of lll_i421.txt

ex_1-2: FORCE ex_1-1
	echo "#1.2# Generate the blocksize progressing strategy for approximate Ideal Lattice Challenge."
	$(PBKZ) -if lll_i421.txt -genstrategy -type shortvec -det 420 -of str421approx

ex_1-3: FORCE ex_1-2
	echo "#1.3# Run progressive BKZ to solve approximate Ideal Lattice Challenge."
	$(PBKZ) -if lll_i421.txt -bkz -sf str421approx -of bkz_i421.txt

run: FORCE ex_1-3
	cat $<
	cat str421approx
