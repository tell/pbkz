include common.mk

run: FORCE check

check: FORCE ex1.log ex2.log

ex1.log:
	echo "Ex.1 Verify the contents in section 6.2 by Ideal Lattice Challenge."
	$(MAKE) -f ex1.mk >$@ 2>&1

ex2.log:
	echo "Ex.2 Verify the contents in section 8 by SVP Challenge."
	$(MAKE) -f ex2.mk >$@ 2>&1
