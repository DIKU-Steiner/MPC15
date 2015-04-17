
CC = gcc
CFLAGS = -O3 -Wall


wds_smt_timing: wds_smt_timing.c
	$(CC) $(CFLAGS) -o $@ $^

wds_smt_org: wds_smt_org.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f wds_smt_timing wds_smt_org
