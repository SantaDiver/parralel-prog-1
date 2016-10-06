build:
	g++ -Wall -std=c++11 -O0 -o "lol" "lol.cpp"

run: build
	echo "" > ./reportdata
	for i in 0 1 2 3 4 5; do \
	./lol --report $$i >> ./reportdata; \
	done;

report: run
	gnuplot plotrules

test: build
	./lol --test