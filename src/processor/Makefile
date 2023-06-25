IN = processor.c ../camera/*.c ../camera/*.h ../core/shared.h ../math/d_math.*
OUT = nacv-processor
LIB = -lapriltag -lv4l2 -lm -lrt

all:
	gcc $(IN) $(LIB) -o $(OUT) && cp $(OUT) /usr/bin/
