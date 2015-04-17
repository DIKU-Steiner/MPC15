
TARGETS = $(subst .stp,.txt,$(subst ../Instances/,,$(wildcard ../Instances/*.stp)))
GEOPATH = ../../../geosteiner-3.1

%.txt: ../Instances/%.stp
	($(GEOPATH)/stpToGeoSteiner.sh $^ | $(GEOPATH)/efst | $(GEOPATH)/bb ) > $@

all: $(TARGETS)


clean:
	rm *.txt
