style:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -n server/*.cc server/*.h client/*.cc client/*.h
	clang-format -i server/*.cc server/*.h client/*.cc client/*.h
	rm -rf .clang-format
.PHONY : style