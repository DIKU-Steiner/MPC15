all:
	for n in {10..15}; do \
		for d in 2 3 4 5; do \
			for s in {1..50}; do \
				~/bin/STPGenerator spheresurface -n $${n} -d $${d} -s $${s} -name "shell_n$${n}_d$${d}_s$${s}" > shell_n$${n}_d$${d}_s$${s}.stp; \
			done; \
		done; \
	done ;

clean:
	rm *stp
