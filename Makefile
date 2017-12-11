CXX=gcc
CXXFLAGS=-std=c11 -Wall -Wextra -pedantic -g
CXXLIBS=-lpthread -lm
CXXINCLUDES=-Ivendor/
TARGET=rctvrp
OUT_FOLDER=./out/

SRC_FOLDER=./src
VENDOR_FOLDER=./vendor
OBJ_FOLDER=./obj

SRCS=$(shell find $(SRC_FOLDER) -type f -name '*.c')
VENDORS=$(shell find $(VENDOR_FOLDER) -type f -name '*.c')

SRCOBJS=$(subst $(SRC_FOLDER), $(OBJ_FOLDER), $(SRCS))
VENDOROBJS=$(subst $(VENDOR_FOLDER), $(OBJ_FOLDER), $(VENDORS))

SRCOBJS:=$(SRCOBJS:.c=.o)
VENDOROBJS:=$(VENDOROBJS:.c=.o)

all: makedirs $(TARGET)

$(TARGET): $(VENDOROBJS) $(SRCOBJS)
	@echo "Compiling final executable to $(OUT_FOLDER)$(TARGET)"
	$(CXX) $(CXXLIBS) $(SRCOBJS) $(VENDOROBJS) -o $(OUT_FOLDER)$(TARGET)

$(VENDOROBJS):
	@echo "Compiling vendor library $< to $@"
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES) -c $< -o $@

$(SRCOBJS):
	@echo "Compiling $< into $@"

	$(CXX) $(CXXFLAGS) $(CXXINCLUDES) -c $< -o $@

$(foreach x,$(join $(addsuffix :,$(SRCOBJS)),$(SRCS)),$(eval $x))
$(foreach x,$(join $(addsuffix :,$(VENDOROBJS)),$(VENDORS)),$(eval $x))

makedirs:
	@mkdir -p obj out

.PHONY: clean
clean:
	@rm -rf ./obj/
	@rm -rf ./rctvrp

watch:
	while sleep 1 ; do find -L . -name '*.[ch]' | entr -d make; done
	

