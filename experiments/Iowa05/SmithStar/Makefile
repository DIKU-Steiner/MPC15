
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -method WarrenSmith -brancher Furthest -nodeComparator BestFirst -input $^ > $@

all: $(TARGETS)


clean:
	rm *.txt
