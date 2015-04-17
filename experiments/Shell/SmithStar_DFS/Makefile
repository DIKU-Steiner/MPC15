
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -method WarrenSmith -brancher Furthest -nodeComparator DepthFirst -input $^ > $@

all: $(TARGETS)


clean:
	rm *.txt
