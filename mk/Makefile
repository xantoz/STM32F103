NAME       = firmware
SOURCES    = $(shell find -L . -type f -name '*.c')
ASMSOURCES = $(shell find -L . -type f -name '*.s')
INCLUDES   = $(shell find -L . -type f -name '*.h')
DEPENDS    = $(SOURCES:%.c=%.dep)
OBJS       = $(SOURCES:%.c=%.o) $(ASMSOURCES:%.s=%.o)

CC       = arm-none-eabi-gcc
LD       = arm-none-eabi-ld
OC       = arm-none-eabi-objcopy
CCFLAGS  = -g -Os -std=c11 '-mcpu=cortex-m3' '-mthumb' -Wall -Winline -Wno-main
LDFLAGS  = '-Tlib/stm32f103c8t6.ld' -nostartfiles -nostdlib -nodefaultlibs
OCFLAGS  = -Obinary --strip-unneeded

# all generates our target bin
all: $(NAME).bin

# generate bin from elf by objcopying from linked obj file
$(NAME).bin: $(NAME).elf
	$(OC) $(OCFLAGS) $^ $@

# generate obj (ELF) by linking all .o files
$(NAME).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.s
	$(CC) $(CCFLAGS) -MD -o $@ -c $<

# compile .c to .o files. Generate .dep files on the fly.
%.o: %.c
	$(CC) $(CCFLAGS) -MD -o $@ -c $<
	@cp $*.d $*.dep; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.dep; \
	rm -f $*.d

# include depencies are defined in .dep files. Include them as rules here.
-include $(DEPENDS)

# clean all generated files
.PHONY: clean
clean:
	-rm -f $(NAME).bin $(NAME).hex $(NAME).elf $(OBJS) $(DEPENDS)
