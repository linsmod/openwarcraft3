# build.sh
cd ~/openwarcraft3
./tools/bin/premake5 gmake2
cd build/gmake2 && make

cd ~/openwarcraft3