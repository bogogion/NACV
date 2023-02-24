CC = gcc
SRCS = core/* camera/* math/* server/*
LIB = -lapriltag -lm -lv4l2 -lpthread
TARGET = nacv
CFG_TARGET = ~/.config/NACV/cam.camcfg
CFG_DIR = ~/.config/NACV
CFG_ORG = ../cfg/test.camcfg
all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -O3 -o $(TARGET)
install: $(SRCS)
	@$(CC) $(SRCS) $(LIB) -O3 -o $(TARGET) && \
	sudo cp $(TARGET) /usr/bin/ && \
	rm $(TARGET) && \
	if ! [ -e $(CFG_DIR) ]; \
		then mkdir $(CFG_DIR); \
	fi; \
	if ! [ -f $(CFG_TARGET) ]; \
		then cp $(CFG_ORG) $(CFG_TARGET); \
	fi;
