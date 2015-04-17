
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	(../../../WarrenSmith/stpToSmith.sh $^ | ../../../WarrenSmith_MartinFix/wds/wds_smt_org) > $@

all: $(TARGETS)


clean:
	rm *.txt
