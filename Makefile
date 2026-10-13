# Resort Records - native Wii homebrew build

TARGET := resort-records
BUILD := build
SOURCES := source
INCLUDES := source

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
OFILES := $(CFILES:.c=.o)

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

# wii_rules defines LIBOGC_INC, LIBOGC_LIB, MACHDEP and PORTLIBS.
# Defer INCLUDE/LIBPATHS expansion until after that include.
LIBDIRS := $(PORTLIBS)
INCLUDE = $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
          $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
          -I$(CURDIR)/$(BUILD) -I$(LIBOGC_INC)

CFLAGS = -g -O2 -Wall -Wextra $(MACHDEP) $(INCLUDE)
LDFLAGS = -g $(MACHDEP) -Wl,-Map,$(notdir $@).map
LIBS := -lwiiuse -lbte -logc -lm
LIBPATHS = $(foreach dir,$(LIBDIRS),-L$(dir)/lib) -L$(LIBOGC_LIB)

.PHONY: all clean package-sd
all: $(OUTPUT).dol

include $(DEVKITPPC)/wii_rules

# The standard Wii rules call $(LD), but GNU Make otherwise defaults to the
# host linker. Use the devkitPPC GCC driver for the correct Wii runtime setup.
export LD := $(CC)

# Explicit dependencies are required: wii_rules provides a generic %.elf
# recipe but no prerequisite list of source objects.
$(OUTPUT).elf: $(OFILES)

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
