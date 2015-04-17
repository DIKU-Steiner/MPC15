
#TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

TARGETS = estein20_1_org.txt

%_org.txt: ../Instances/%.stp 
	(../../../WarrenSmith/stpToSmith.sh $^ | ./wds_org) > $@

%_fix.txt: ../Instances/%.stp 
	(../../../WarrenSmith/stpToSmith.sh $^ | ./wds_fix) > $@

all: $(TARGETS)


wds_%: wds_%.c
	gcc -o $@ -O0 -lm $^

clean:
	rm *.txt
