#define uint64_t ulong
#define uint32_t uint
#define uint16_t ushort
#define uint8_t  uchar

__kernel void transpose_img(__global uint8_t* src, __global uint8_t* dst)
{
    int x = get_global_id(0) % 608;
    int y = get_global_id(0) / 608;

    if (0 <= x && x < 608 && 0 <= y && y < 684) {
        dst[x * 684 * 3 + y * 3 + 0] = src[y * 608 * 3 + x * 3 + 0];
        dst[x * 684 * 3 + y * 3 + 1] = src[y * 608 * 3 + x * 3 + 1];
        dst[x * 684 * 3 + y * 3 + 2] = src[y * 608 * 3 + x * 3 + 2];
    }
}