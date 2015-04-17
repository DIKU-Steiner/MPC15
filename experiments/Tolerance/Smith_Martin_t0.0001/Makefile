
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	(../../../WarrenSmith/stpToSmith.sh $^ | ../../../WarrenSmith_MartinFix/wds/wds_smt_timing -tol 0.0001) > $@

all: $(TARGETS)


clean:
	rm *.txt
