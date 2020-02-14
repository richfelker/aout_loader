
OBJS = loader.o

all: aout_loader

CFLAGS = -fPIC -O2
LDFLAGS = -pie -static

aout_loader: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f aout_loader $(OBJS)
