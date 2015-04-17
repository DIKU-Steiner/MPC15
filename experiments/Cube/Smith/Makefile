
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	(../../../WarrenSmith/stpToSmith.sh $^ | ../../../WarrenSmith/smith) > $@

all: $(TARGETS)


clean:
	rm *.txt
