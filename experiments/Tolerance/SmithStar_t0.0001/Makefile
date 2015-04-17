
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -method WarrenSmith -brancher Furthest -nodeComparator DepthFirst -input $^ -tolerance 0.0001 > $@

all: $(TARGETS)


clean:
	rm *.txt
