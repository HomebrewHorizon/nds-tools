#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# These set the information text in the nds file (metadata for the DS ROM)
GAME_TITLE     := Flavacorn Simulator
GAME_SUBTITLE1 := Chata500
GAME_SUBTITLE2 := v1.0
include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary files embedded using bin2o
# GRAPHICS is a list of directories containing image files (unused here)
# AUDIO is a list of directories containing audio to be converted by maxmod
# ICON is unused here (no icon injected)
# NITRO is a directory that will be accessible via NitroFS
#---------------------------------------------------------------------------------
TARGET   := $(shell basename $(CURDIR))
BUILD    := build
SOURCES  := source
INCLUDES := include
DATA     := data
GRAPHICS :=
AUDIO    :=
ICON     :=
# (No icon is specified, so no icon processing is performed.)

# Specify a directory containing the Nitro filesystem (if using NitroFS)
NITRO    :=

#---------------------------------------------------------------------------------
# Options for code generation
#---------------------------------------------------------------------------------
ARCH := -march=armv5te -mtune=arm946e-s

CFLAGS   := -g -Wall -O2 -ffunction-sections -fdata-sections\
            $(ARCH) $(INCLUDE) -DARM9
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS  := -g $(ARCH)
LDFLAGS  := -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# Additional libraries to link with (order is important)
#---------------------------------------------------------------------------------
LIBS := -lnds9

# Automatically add libraries for NitroFS (if applicable)
ifneq ($(strip $(NITRO)),)
LIBS := -lfilesystem -lfat $(LIBS)
endif

# Automatically add maxmod library (if using AUDIO)
ifneq ($(strip $(AUDIO)),)
LIBS := -lmm9 $(LIBS)
endif

#---------------------------------------------------------------------------------
# List of directories containing libraries (the top-level directories containing include and lib)
#---------------------------------------------------------------------------------
LIBDIRS := $(LIBNDS) $(PORTLIBS)

#---------------------------------------------------------------------------------
# No need to edit anything past this point unless additional rules are needed
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT := $(CURDIR)/$(TARGET)

export VPATH  := $(CURDIR)/$(subst /,,$(dir $(ICON)))\
                $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))\
                $(foreach dir,$(DATA),$(CURDIR)/$(dir))\
                $(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PNGFILES := $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

# Prepare NitroFS directory (if using NitroFS)
ifneq ($(strip $(NITRO)),)
  export NITRO_FILES := $(CURDIR)/$(NITRO)
endif

# Get audio list for maxmod (if using AUDIO)
ifneq ($(strip $(AUDIO)),)
  export MODFILES := $(foreach dir,$(notdir $(wildcard $(AUDIO)/*.*)),$(CURDIR)/$(AUDIO)/$(dir))
  ifneq ($(strip $(NITRO)),)
    export SOUNDBANK := $(NITRO_FILES)/soundbank.bin
  else
    export SOUNDBANK := soundbank.bin
    BINFILES += $(SOUNDBANK)
  endif
endif

#---------------------------------------------------------------------------------
# Define the compiler to use for linking
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
  export LD := $(CC)
else
  export LD := $(CXX)
endif

export OFILES_BIN    := $(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES       := $(PNGFILES:.png=.o) $(OFILES_BIN) $(OFILES_SOURCES)
export HFILES       := $(PNGFILES:.png=.h) $(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE  := $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir))\
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include)\
                   -I$(CURDIR)/$(BUILD)
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

#---------------------------------------------------------------------------------
# PHONY targets
#---------------------------------------------------------------------------------
.PHONY: $(BUILD) clean

$(BUILD):
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo Cleaning...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(SOUNDBANK)

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# Main targets
#---------------------------------------------------------------------------------
$(OUTPUT).nds: $(OUTPUT).elf $(NITRO_FILES)
$(OUTPUT).elf: $(OFILES)

# Make generated headers depend on their source file
$(OFILES_SOURCES): $(HFILES)

# Make sure soundbank is built (if using audio)
$(OFILES): $(SOUNDBANK)

#---------------------------------------------------------------------------------
# Rule to build soundbank from audio files (if any)
#---------------------------------------------------------------------------------
$(SOUNDBANK): $(MODFILES)
	mmutil $^ -d -o$@ -hsoundbank.h

#---------------------------------------------------------------------------------
%.bin.o %_bin.h: %.bin
	@echo Processing $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
# This rule creates assembly source files using grit.
# grit processes an image file and a corresponding .grit description.
# (Unused here, since no graphics are being converted.)
#---------------------------------------------------------------------------------
%.s %.h: %.png %.grit
	grit $< -fts -o$*

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
