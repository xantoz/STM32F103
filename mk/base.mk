SOURCE_DIRS += $(BASE)/lib
SOURCE_DIRS += .

NAME       = firmware
SOURCES    = $(shell find -L $(SOURCE_DIRS) -type f -name '*.c')
ASMSOURCES = $(shell find -L $(SOURCE_DIRS) -type f -name '*.s')
INCLUDES   = $(shell find -L $(SOURCE_DIRS) -type f -name '*.h')
DEPENDS    = $(SOURCES:%.c=%.dep) $(ASMSOURCES:%.s=%.dep)
OBJS       = $(SOURCES:%.c=%.o) $(ASMSOURCES:%.s=%.o)

LINKER_SCRIPT = $(BASE)/lib/stm32f103c8t6.ld

AS       = arm-none-eabi-as
CC       = arm-none-eabi-gcc
LD       = arm-none-eabi-ld
OC       = arm-none-eabi-objcopy
CPUFLAGS = -mcpu=cortex-m3 -march=armv7-m -mthumb
INCLUDES = $(addprefix -I,$(SOURCE_DIRS))
CPPFLAGS = $(INCLUDES) -DSTM32F10X_MD
DEBUG    ?= -g
ASFLAGS  = $(DEBUG) $(CPUFLAGS) $(INCLUDES)
CCFLAGS  = $(DEBUG) -Os -std=c11 $(CPUFLAGS) $(CPPFLAGS) -nostdinc -fshort-enums -fno-hosted -fdiagnostics-show-option -Wall -Wextra -Winline -Wvariadic-macros -Wno-main -Wno-type-limits
LDFLAGS  = '-T$(LINKER_SCRIPT)' -nostartfiles -nostdlib -nodefaultlibs --gc-sections -Map=$(NAME).map
OCFLAGS  = -Obinary --strip-unneeded

# all generates our target bin
all: $(NAME).bin

# generate bin from elf by objcopying from linked obj file
$(NAME).bin: $(NAME).elf
	$(OC) $(OCFLAGS) $^ $@

# generate obj (ELF) by linking all .o files
$(NAME).elf: $(OBJS) $(LINKER_SCRIPT)
	$(LD) $(LDFLAGS) -o $@ $(filter-out $(LINKER_SCRIPT),$^)

%.o: %.s
	$(AS) $(ASFLAGS) --MD $*.dep -o $@ $<

# compile .c to .o files. Generate .dep files on the fly.
%.o: %.c
	$(CC) $(CCFLAGS) -MD -MF $*.dep -o $@ -c $<

# include depencies are defined in .dep files. Include them as rules here.
-include $(DEPENDS)

# clean all generated files
.PHONY: clean
clean:
	-rm -f $(NAME).bin $(NAME).hex $(NAME).elf $(NAME).map $(OBJS) $(DEPENDS)
