
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	(../../stpToSmith.sh $^ | ../../../code/WarrenSmith/wds_smt_timing -tol 0.0001) > $@

all: $(TARGETS)


clean:
	rm *.txt
