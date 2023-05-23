VPATH := src
SHARED_DIR := /home/freddy/Documents/sk00l/2023/DM510/uml/shared
CFLAGS += -I/home/freddy/Documents/sk00l/2023/DM510/uml/linux-6.1.9/usr/include
BINS := msgbox_put msgbox_get test

override SHARED_BINS := $(addprefix $(SHARED_DIR)/, $(BINS))

all: $(BINS) $(SHARED_BINS)

bins: $(BINS)

$(SHARED_BINS): $(BINS)
	cp $(BINS) $(SHARED_DIR)

.PHONY: clean
clean:
	$(RM) $(BINS)

