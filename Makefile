.PHONY: compile-cl_val_mul2
compile-cl_val_mul2:
	g++ cl_val_mul2.c -o cl_val_mul2.out --std=c++11 -Ofast -Wall -I./include -framework OpenCL -DMAC

.PHONY: compile-cl_copy_img
compile-cl_copy_img:
	g++ cl_copy_img.c -o cl_copy_img.out --std=c++11 -Ofast -Wall -I./include -framework OpenCL -DMAC

.PHONY: compile-cl_copy_img_x8
compile-cl_copy_img_x8:
	g++ cl_copy_img_x8.c -o cl_copy_img_x8.out --std=c++11 -Ofast -Wall -I./include -framework OpenCL -DMAC

.PHONY: clean
clean:
	rm -f *.out *.bmp