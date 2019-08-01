#include <CL/cl.h>
#include <bits/stdc++.h>

#define THREAD_SIZE 256
#define ARR_SIZE 32768*2
#define MAX_SOURCE_SIZE 1000000

using namespace std;

int main(int argc, char const *argv[])
{
	cl_int err, t_err;
	cl_uint num_devices;
	cl_uint num_platforms;

	cl_context context;
	cl_program program;
	cl_command_queue queue;
	cl_device_id device_id;
	cl_platform_id platform_id;

	size_t source_size;
	int arr_size = ARR_SIZE;
	int *arr = new int[arr_size];
	char *source = new char[MAX_SOURCE_SIZE];

	for(int i=0; i<arr_size; i++)
		arr[arr_size-1-i] = i+1;

	err = clGetPlatformIDs(1, &platform_id, &num_platforms);
	err |= clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &num_devices);
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &t_err);
	err |= t_err;
	queue = clCreateCommandQueue(context, device_id, 0, &t_err);
	err |= t_err;

	FILE *fp = fopen("./bitonicSort.cl", "rb");
	source_size = fread(source, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	program = clCreateProgramWithSource(context, 1, (const char **)&source, &source_size, &t_err);
	err |= t_err;
	err |= clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	cl_mem d_arr = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, sizeof(int)*arr_size, arr, &t_err);
	err |= t_err;

	cl_kernel kernel = clCreateKernel(program, "bitonicSort", &t_err);
	err |= t_err;
	err |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_arr);
	err |= clSetKernelArg(kernel, 3, sizeof(int), &arr_size);
	err |= clFinish(queue);

	size_t global_size[3] = {ARR_SIZE, 1, 1};
	size_t local_size[3] = {THREAD_SIZE, 1, 1};

	//start timer here
	//bitonic sort
	for(int i=2; i<=arr_size; i*=2)
		for(int j=i/2; j>=1; j/=2)
		{
			err |= clSetKernelArg(kernel, 1, sizeof(int), &i);
			err |= clSetKernelArg(kernel, 2, sizeof(int), &j);
			err |= clEnqueueNDRangeKernel(queue, kernel, 1, 0, (const size_t*)global_size, (const size_t*)local_size, 0, NULL, NULL);
		}
	err |= clFinish(queue);
	//end timer here

	err |= clEnqueueReadBuffer(queue, d_arr, CL_TRUE, 0, sizeof(int)*arr_size, arr, 0, NULL, NULL);
	err |= clFinish(queue);

	err |= clReleaseKernel(kernel);
	err |= clReleaseProgram(program);

	err |= clReleaseMemObject(d_arr);

	err |= clReleaseCommandQueue(queue);
	err |= clReleaseContext(context);
	err |= clReleaseDevice(device_id);

	if(err != 0)
		cout << "Failure" << endl;
	else
	{
		int i = 1;
		for(; i<arr_size; i++)
		{
			if(arr[i] < arr[i-1])
			{
				cout << "Failure" << endl;
				break;
			}			
		}
		if(i==arr_size)
			cout << "Success" << endl;
	}

	free(source);
	delete[] arr;

	return 0;
}