include common.mk

run: FORCE check

check: FORCE check-ex1 check-ex2

check-ex1: FORCE
	echo "Ex.1 Verify the contents in section 6.2 by Ideal Lattice Challenge."
	$(MAKE) -f ex1.mk 2>&1 | tee ex1.log

check-ex2: FORCE
	echo "Ex.2 Verify the contents in section 8 by SVP Challenge."
	$(MAKE) -f ex2.mk 2>&1 | tee ex2.log
