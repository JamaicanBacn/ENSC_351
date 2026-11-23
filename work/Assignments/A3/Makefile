all:
	cmake -S . -B build
	cmake --build build


clean:
	rm -rf build

remake: clean all

.PHONY: all clean remake submission
submission:
	mkdir -p build/submission
	tar --exclude=node_modules --transform='s,^,as3/,' -czf build/submission/as3-beatbox.tar.gz CMakeLists.txt Makefile app hal wave-files as3-server
	cd build/submission && zip -q -j ../as3-submission.zip as3-beatbox.tar.gz
