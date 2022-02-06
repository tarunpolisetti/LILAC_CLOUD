CC = gcc

# compiler flags:
CFLAGS  = -g 

# the build target executable:
TARGET = md5-compute

all: $(TARGET)

# compilation and storing the target file
$(TARGET): $(TARGET).c 
			$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lssl -lcrypto

# for gmake clean			 
clean:
	$(RM) $(TARGET)
