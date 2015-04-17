all:
	for n in {10..15}; do \
		for d in 2 3 4 5; do \
			for s in {1..50}; do \
				~/bin/STPGenerator edge -n $${n} -d $${d} -s $${s} -name "edge_n$${n}_d$${d}_s$${s}" > edge_n$${n}_d$${d}_s$${s}.stp; \
			done; \
		done; \
	done ;

clean:
	rm *stp
