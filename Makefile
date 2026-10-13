# Resort Records - native Wii homebrew build

TARGET := resort-records
BUILD := build
SOURCES := source
INCLUDES := source

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
OFILES := $(CFILES:.c=.o)

# LIBOGC_INC is exported by wii_rules; defer expansion until after inclusion.
INCLUDE = -I$(LIBOGC_INC) $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

# Attach source objects explicitly: generic wii_rules %.elf rule has no deps.
CFLAGS = -g -O2 -Wall -Wextra $(MACHDEP) $(INCLUDE)
LDFLAGS = -g -Wl,-Map,$(notdir $@).map
LIBS := -lwiiuse -lbte -logc
LIBPATHS = -L$(LIBOGC_LIB) -L$(PORTLIBS_PATH)/wii/lib -L$(PORTLIBS_PATH)/ppc/lib

.PHONY: all clean package-sd
all: $(OUTPUT).dol

include $(DEVKITPPC)/wii_rules

# Wii rules use $(LD) directly; GNU Make defaults LD to host ld. Force the
# PowerPC GCC driver so Wii startup objects/specs and runtime libraries work.
LD := $(DEVKITPPC)/bin/powerpc-eabi-gcc

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
