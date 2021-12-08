include ../Makefile.conf

TARGET=$(BIN_DIR)/RAVGJCZ
OBJS=main.o

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ -pthread 

%.o:%.c
	$(CC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
