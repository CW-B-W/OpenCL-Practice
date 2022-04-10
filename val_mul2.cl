#define uint64_t ulong
#define uint32_t uint
#define uint16_t ushort
#define uint8_t  uchar

__kernel void val_mul2(__global uint8_t* src, __global uint8_t* dst)
{
    int x = get_global_id(0) % 608;
    int y = get_global_id(0) / 608;

    uint32_t val = (uint32_t)2 * src[y * 608 + x];
    dst[y * 608 + x] = val > 255 ? 255 : val;
}