#define uint64_t ulong
#define uint32_t uint
#define uint16_t ushort
#define uint8_t  uchar

__kernel void copy_img(__global uint8_t* src, __global uint8_t* dst)
{
    int x = get_global_id(0) % 608;
    int y = get_global_id(0) / 608;

    dst[y * 608 + x] = src[y * 608 + x];
}