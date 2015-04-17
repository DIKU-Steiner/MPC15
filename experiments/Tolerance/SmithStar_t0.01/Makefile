
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -method WarrenSmith -brancher Furthest -nodeComparator DepthFirst -input $^ -tolerance 0.01 > $@

all: $(TARGETS)


clean:
	rm *.txt
