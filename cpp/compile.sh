cmake . -DOpenCL_LIBRARY=/usr/lib/x86_64-linux-gnu/libOpenCL.so -DBoost_USE_STATIC_LIBS=OFF -DBUILD_MCTS=1 -DUSE_BACKEND=OPENCL
make
