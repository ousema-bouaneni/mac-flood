LINK_TARGET = l2flood
OBJS = \
	l2flood.o

REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)

clean: 
	rm -f $(REBUILDABLES)

$(LINK_TARGET) : $(OBJS)

%.o : %.c
	gcc -g -Wall -o $@ -c $< -fcommon
