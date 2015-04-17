
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -method WarrenSmith -brancher Furthest -nodeComparator DepthFirst -stopAfter 86400 -input $^ > $@

all: $(TARGETS)


clean:
	rm *.txt
