# Resort Records - native Wii homebrew build

TARGET := resort-records
BUILD := build
SOURCES := source
INCLUDES := source

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
OFILES := $(CFILES:.c=.o)
INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

# The standard Wii rules compile C sources with powerpc-eabi-gcc but GNU Make's
# built-in LD defaults to host ld. Use the cross-compiler driver for linking so
# Wii startup objects/specs and runtime libraries are set up correctly.
CFLAGS = -g -O2 -Wall -Wextra $(MACHDEP) $(INCLUDE)
LDFLAGS = -g $(MACHDEP) -Wl,-Map,$(notdir $@).map
LIBPATHS = -L$(LIBOGC_LIB) -L$(PORTLIBS_PATH)/wii/lib -L$(PORTLIBS_PATH)/ppc/lib
LIBS := -lwiiuse -lbte -logc

.PHONY: all clean package-sd
all: $(OUTPUT).dol

include $(DEVKITPPC)/wii_rules

LD := $(DEVKITPPC)/bin/powerpc-eabi-gcc

$(BUILD):
	mkdir -p $@

$(OFILES): | $(BUILD)

package-sd: $(OUTPUT).dol
	mkdir -p dist/apps/resort-records
	cp $(OUTPUT).dol dist/apps/resort-records/boot.dol
	cp meta.xml dist/apps/resort-records/meta.xml
	cp $(OUTPUT).dol dist/resort-records.dol
	cd dist && zip -qr resort-records-sd.zip apps

clean:
	rm -rf $(BUILD) $(OFILES) $(OUTPUT).elf $(OUTPUT).dol $(OUTPUT).elf.map dist

-include $(wildcard $(BUILD)/*.d)
