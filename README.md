# redsom_pack
Software project written in C (using Pthreads library and OPENCV library) to create, train and visualize a SOM neural network.

It's composed by 5 tools:
- som: application that allow to create and train a SOM neural network, using multithreading tecnology;
- normalizer: tool useful to normalize input dataset before the training process;
- datamap: tool useful to locate, on a trained map, each pattern of a dataset;
-umatrixvisual: application that allow to obtain a U-MATRIX visualization of a trained map (very useful to check the presence of some clusters);
-redvisual: application that allow to obtain a RedSOM visualization of two trained maps (relative density visualization method, that compares cluster structures in temporally ordered datasets, introduced by Denny, Graham J. Williams and Peter Christen in https://ieeexplore.ieee.org/document/4781112/).

All the applications in the pack require the presence of the file common.h to compile.

The applications umatrixvisual and redvisual require the presence of OPENCV library on the system.

Copyright(C) 2011 by Giovanni Russotto (russottogio@gmail.com)
This is free software; you can redistribute it and/or modify it for accademic and scientific purposes, including a reference to its author.
This software CANNOT be used, copied, included or modified for commercial purposes. It's distributed in the hope that it will be useful but WITHOUT ANY WARRANTY!!
