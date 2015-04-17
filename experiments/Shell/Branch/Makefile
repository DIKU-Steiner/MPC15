
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))

%.txt: ../Instances/%.stp
	GenGeoSteiner -method GeoSteiner -input $^ > $@

all: $(TARGETS)


clean:
	rm *.txt
