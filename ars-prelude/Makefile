#---- Proj settings ---
PROJ    := arstactica
EXT     := gba

SOBJS  = \
	tileset_lava.o \
	normal_font.o \
	outline_font.o \
	tileset_dungeon.o \
	tileset_undersea.o \
	ui_tileset.o \
	obj_tileset.o \
	sprites_sheet.o \
	intro_tileset.o \
	intro_tileset_fire.o \
	intro_tileset_crystal.o \
	alcapa_logo.o \
	ars_intro.o \
	licensed.o \
	press_to_play.o \
	character_menu_obj_sheet.o \
	character_menu_bg_sheet.o \
	shop_menu_bg_sheet.o \
	shop_menu_obj_sheet.o \

COBJS = \
	main.o \
	gba.o \
	dungeonlevel.o \
	system.o \
	dungeonmap.o \
	dungeongenerator.o \
	gamerenderer.o \
	gamefonts.o \
	graphicsUtil.o \
	movementcommand.o \
	attackcommand.o \
	useitemcommand.o \
	objectGraphics.o \
	gameobject.o \
	unit.o \
	item.o \
	gamecontroller.o \
	gameobjectfactory.o \
	scenemanager.o \
	gamescene.o \
	charactermenuscene.o \
	introscene.o \
	shopscene.o \
	cutscenescene.o \


OBJS    := $(SOBJS) $(COBJS) $(COBJSARM)

#--- Tool settings ---
CROSS   := arm-none-eabi-
AS      := $(CROSS)as
CC      := $(CROSS)g++
LD      := $(CROSS)g++
OBJCOPY := $(CROSS)objcopy

MODEL   := -mthumb-interwork -mthumb
SPECS   := -specs=gba.specs

ASFLAGS := -mthumb-interwork
CFLAGS  := $(MODEL) -O3 -Wall
CFLAGSARM  := -O3 -Wall
LDFLAGS := $(SPECS) $(MODEL)

#--- Build steps ---
build : $(PROJ).$(EXT)

$(PROJ).$(EXT) : $(PROJ).elf
	@$(OBJCOPY) -v -O binary $< $@
	-@gbafix $@

$(PROJ).elf : $(OBJS)
	@$(LD) $^ $(LDFLAGS) -o $@

#COBJS compiled automatically via implicit rules
$(COBJS) : %.o : %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

$(COBJSARM) : %.o : %.cpp
	$(CC) -c $< $(CFLAGSARM) -o $@

$(SOBJS) : %.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

# --- Clean ---
.PHONY : clean
clean : 
	@rm -fv $(COBJSARM) $(COBJSARM)
	@rm -fv $(COBJS) $(SOBJS)
	@rm -fv $(PROJ).$(EXT) 
	@rm -fv $(PROJ).elf	

all: arstactica.gba

# Run the ROM image using VBA.
run: arstactica.gba
	VisualBoyAdvance arstactica.gba

# Run a program with a different value of TARGET.
# (This lets you have multiple GBA programs in the same directory;
# if you have foo.cpp, you can compile and run it with "make run-foo".)
run-%:
	@$(MAKE) run TARGET=$*

-include depend.mk
depend.mk: *.cpp *.h *.s
	$(CC) -MM *.cpp >depend.mk
