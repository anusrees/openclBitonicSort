__kernel void bitonicSort(__global *arr, int i, int j, int size)
{
	int gid = get_global_id(0);

	if(gid < size && gid%(2*j) < j)
	{
		int index = i-(gid+1)%i-gid%i;

		if(i == 2*j && gid + index < size && arr[gid] > arr[gid + index])
		{
			int temp = arr[gid];
			arr[gid] = arr[gid + index];
			arr[gid + index] = temp;
		}
		else if(gid + j < size && arr[gid] > arr[gid+j])
		{
			int temp = arr[gid];
			arr[gid] = arr[gid + j];
			arr[gid + j] = temp;
		}
	}
}