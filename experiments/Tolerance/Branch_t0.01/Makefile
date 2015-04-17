
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -input $^ -tolerance 0.01 > $@

all: $(TARGETS)


clean:
	rm *.txt
