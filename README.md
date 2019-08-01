# openclBitonicSort
Simple implementation of bitonic sort in OpenCL.
* To build, run on command line:<br />
```g++ -o bitonic_sort -I<location to OpenCL headers> -L<location to OpenCL library> bitonicSort.cpp -lOpenCL```
* Profiling best done using a profiler. To enable profiling, make changes in the createCommandQueue function as follows:<br />
```34: queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &t_err);```
