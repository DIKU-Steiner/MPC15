
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -input $^ > $@

all: $(TARGETS)


clean:
	rm *.txt
