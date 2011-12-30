CC=gcc
CFLAGS=-ggdb -O0  `pkg-config ExtremeCUnit --cflags` -fdevirtualize
LDFLAGS=`pkg-config ExtremeCUnit --libs`
SRC=mock.c
MAKEDEPEND = gcc -M $(CFLAGS) -o $*.d $<

OBJ_FILE_NAMES=$(SRC:.c=.o)
OBJS=$(patsubst %,$(OBJDIR)/%,$(OBJ_FILE_NAMES))

DEPDIR = .dep
OBJDIR = obj
INSTALL_PREFIX = /usr
UNIT_TEST_SRC= test.c

SHARE_LIBRARY_TARGET = libExtremeCMock.so

INCLUDES=$(patsubst %,$(DEPDIR)/%,$(INC_FILE_NAMES))

all: run
-include $(INCLUDES)

$(OBJDIR)/%.o : %.c
	mkdir -p $(OBJDIR); \
	mkdir -p $(DEPDIR); \
	$(MAKEDEPEND); \
	sed -e 's/\(^[^:\.o \t]\)/$(OBJDIR)\/\1/' < $*.d > $(DEPDIR)/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' -e  's/\(^[^:.o \t]*\.o: .*\)/$(OBJDIR)\/\1/' < $*.d >> $(DEPDIR)/$*.P; \
	$(COMPILE.c) -o $@ $<; \
	rm -f $*.d;

$(SHARE_LIBRARY_TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ -shared $(OBJS) 

test: $(UNIT_TEST_SRC) $(SHARE_LIBRARY_TARGET)
	$(CC) $(CFLAGS) $(LDFLAGS) -DUNIT_TEST -o $@ $< $(SHARE_LIBRARY_TARGET)
	
.PHONY: run clean
run: test
	export LD_LIBRARY_PATH=.;./test
clean:
	rm -rf $(SHARE_LIBRARY_TARGET) $(OBJDIR) $(DEPDIR)
