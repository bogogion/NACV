CC = gcc
SRCS = core/*.h camera/* math/* server/* config/* ../lib/cjson/*
TARGET_SRC = core/main.c
TARGET_CTRL_SRC = controller/nacv-controller.c
TARGET_PROCESSOR_SRC = processor/processor.c
LIB = -lapriltag -lm -lv4l2 -lpthread -lrt
TARGET = nacv
TARGET_CTRL = nacv-controller
TARGET_PROCESSOR = nacv-processor
all: $(SRCS)
	$(CC) $(SRCS) $(TARGET_SRC) $(LIB) -O3 -o $(TARGET) && \
	$(CC) $(SRCS) $(TARGET_CTRL_SRC) $(LIB) -O3 -o $(TARGET_CTRL) && \
	$(CC) $(TARGET_PROCESSOR_SRC) $(SRCS) $(LIB) -o $(TARGET_PROCESSOR)
install: $(SRCS)
	@$(CC) $(SRCS) $(TARGET_SRC) $(LIB) -O3 -o $(TARGET) && \
	$(CC) $(SRCS) $(TARGET_CTRL_SRC) $(LIB) -O3 -o $(TARGET_CTRL) && \
	$(CC) $(TARGET_PROCESSOR_SRC) $(SRCS) $(LIB) -o $(TARGET_PROCESSOR)
	mv $(TARGET) /usr/bin/ && \
	mv $(TARGET_CTRL) /usr/bin && \
	mv $(TARGET_PROCESSOR) /usr/bin 
