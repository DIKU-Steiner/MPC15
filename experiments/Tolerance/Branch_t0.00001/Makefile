
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -input $^ -tolerance 0.00001 > $@

all: $(TARGETS)


clean:
	rm *.txt
