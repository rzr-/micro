#!/bin/bash
if [ "$1" == "clean" ]; then
	if [ -d "build" ]; then
		cd build
		make clean
		cd -
		echo "Porject cleaned!"
		rm -rf build
	fi
else 
	mkdir -p build
	cd build
	cmake .. && make && ./bin/expr
fi