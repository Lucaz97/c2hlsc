
CONFIG?=inputs/quicksort/config_qs.yaml

default: run

run:
	python3.11 run.py $(CONFIG)

runlog:
	python3.11 run.py $(CONFIG) > out.log
	
clean:
	rm -rf Catapult*