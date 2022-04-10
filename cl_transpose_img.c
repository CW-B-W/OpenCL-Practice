#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "OpenCLUtils.h"

int main()
{
    int width, height, channels;
    uint8_t *data = stbi_load("input/image.bmp", &width, &height, &channels, 3);
    uint8_t *data_out = (uint8_t*)malloc(3*608*684*sizeof(uint8_t));
    printf("Width    = %d\n", width);
    printf("Height   = %d\n", height);
    printf("Channels = %d\n", channels);

    {
        #define PROGRAM_FILE "transpose_img.cl"
        #define KERNEL_FUNC "transpose_img"

        cl_device_id device;
        cl_context context;
        cl_program program;
        cl_kernel kernel;
        cl_command_queue queue;
        cl_int i, err;

        size_t local_size, global_size;

        cl_mem src_cl_mem, dst_cl_mem;

        device = create_device();
        context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

        program = build_program(context, device, PROGRAM_FILE);

        queue = clCreateCommandQueue(context, device, 0, &err);

        src_cl_mem = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, 3*608*684*sizeof(uint8_t), (void*)data, NULL);
        dst_cl_mem = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, 3*608*684*sizeof(uint8_t), (void*)data_out, NULL);
        void *src_mapped_ptr = clEnqueueMapBuffer(queue, src_cl_mem, CL_TRUE, CL_MAP_READ,
                                    0, 3*608*684*sizeof(uint8_t), 0, NULL, NULL, NULL);
        void *dst_mapped_ptr = clEnqueueMapBuffer(queue, dst_cl_mem, CL_TRUE, CL_MAP_WRITE,
                                    0, 3*608*684*sizeof(uint8_t), 0, NULL, NULL, NULL);

        kernel = clCreateKernel(program, KERNEL_FUNC, &err);

        err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &src_cl_mem);    // <=====INPUT
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &dst_cl_mem); // <=====OUTPUT

        
        err = clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_size), &local_size, NULL);
        // !! It's (3*608*684) / 3
        global_size = (3*608*684/3*sizeof(uint8_t)+local_size-1)/local_size*local_size;
        printf("local_size = %zu\n", local_size);
        printf("global_size = %zu\n", global_size);
        
        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL); 

        clFinish(queue);

        clEnqueueUnmapMemObject(queue, src_cl_mem, src_mapped_ptr, 0, NULL, NULL);
        clEnqueueUnmapMemObject(queue, dst_cl_mem, dst_mapped_ptr, 0, NULL, NULL);

        clReleaseKernel(kernel);
        clReleaseMemObject(src_cl_mem);
        clReleaseMemObject(dst_cl_mem);
        clReleaseCommandQueue(queue);
        clReleaseProgram(program);
        clReleaseContext(context);
        
        #undef PROGRAM_FILE
        #undef KERNEL_FUNC
    }

    stbi_write_bmp("image.bmp", height, width, 3, data_out);
    stbi_image_free(data);
    free(data_out);

    return 0;
}