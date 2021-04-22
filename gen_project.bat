md build
cd build
cmake .. ^
	-G "Visual Studio 14 2015 Win64" ^
	-DBUILD_TESTING=ON ^
	-DMUGGLE_BUILD_SHARED_LIB=ON
