#
# Feet of Fury
# Copyright (C)2002,2003 Cryptic Allusion, LLC
#   

# What are we partial target are we building?
BUILD = FULL

# This is our output binary
TARGET = ../fof.elf

# Add our include dir and RUN_FROM_PC for debugging
KOS_LOCAL_CFLAGS = -Iinclude -DTEST_FROM_PC -DRUN_FROM_PC # -DKIOSK_MODE
DBG_LIBS = -lkosh -lconio	# Needed for RUN_FROM_PC

# Uncomment to really disable debugging
# KOS_LOCAL_CFLAGS += -DNDEBUG

# No romdisk for us these days...
ROMDISK = romdisk_empty

# Dependencies go in their own subdir
DEPDIR = .deps
df = $(DEPDIR)/$(*F)

# Declare all these as phony targets so Make knows not to screw
# with checking 'em for existance in the FS...
.PHONY:		all clean rm-elf dist reset run fonts

# Not supported in GCC 3.0.x :(
# PRECOMPHDR	= include/global.h.gch

# All the sources
##SRCS_LUA	= lua/lua.cc lua/lua_demo.cc
##SRCS_DEMO	= demo/demo.cc

SRCS_MAIN	= main/main.cc main/init_shutdown.cc main/gfx.cc main/loading.cc main/pak.cc \
			main/cache.cc main/ogg.cc main/bsod.cc main/bsod_snd.cc \
			main/character.cc main/vmu.cc main/hsv.cc main/cdtray.cc \
			main/options.cc main/bkgloader.cc main/genmenu.cc main/plrgenmenu.cc \
			main/region.cc main/widget.cc main/res.cc main/unlock.cc \
			main/songs.cc
			# main/stlstring.cc
SRCS_TITLE	= title/title.cc

SRCS_ABOUT	= about/about.cc about/bubbles.cc about/cube.cc

SRCS_DRAWABLES	= drawables/bubble.cc drawables/cube.cc drawables/bubblefield.cc \
			drawables/lifemeter.cc drawables/combometer.cc \
			drawables/combobomb.cc \
			drawables/cdtray.cc drawables/songinfo.cc \
			drawables/winner.cc \
			drawables/wavetxr.cc drawables/gamemode.cc \
			drawables/topbar.cc drawables/slideshow.cc \
			drawables/bubble_selector.cc drawables/textdisplay.cc \
			drawables/gridselector.cc drawables/scroller.cc \
			drawables/plasma.cc drawables/starfield.cc \
			drawables/loadhoops.cc drawables/cubetunnel.cc \
			drawables/bouncetxr.cc drawables/winticks.cc \
			drawables/tube.cc drawables/log3dmover.cc \
			drawables/exp3dmover.cc drawables/menubox.cc drawables/popup.cc \
			drawables/soundfader.cc drawables/screenblinds.cc \
			drawables/particlestorm.cc drawables/cubecube.cc drawables/vs.cc \
			drawables/messagebox.cc drawables/saving.cc drawables/manualviewer.cc
# drawables/mouse.cc

SRCS_CA_INTRO	= ca_intro/ca_intro.cc

SRCS_MENU_MAIN	= menu_main/menu_main.cc

SRCS_MENU_STAGE	= menu_stage/menu_stage.cc

SRCS_MENU_PLAYSTYLE	= menu_playstyle/menu_playstyle.cc menu_playstyle/attack_mode_popup.cc

SRCS_MENU_SCORE	= menu_score/menu_score.cc

SRCS_MENU_CHAR	= menu_char/menu_char.cc

SRCS_MENU_WIN	= menu_win/menu_win.cc

SRCS_MENU_VMU	= menu_vmu/menu_vmu.cc

SRCS_MENU_SWAPCD = menu_swapcd/menu_swapcd.cc

SRCS_MENU_EXTRAS = menu_extras/menu_extras.cc menu_extras/multiOptionMenu.cc \
	menu_extras/textOption.cc menu_extras/unlockMenu.cc menu_extras/jukeboxMenu.cc \
	menu_extras/slotsMenu.cc menu_extras/optionBtnMap.cc menu_extras/picviewMenu.cc \
	menu_extras/manualviewMenu.cc

SRCS_MENU_5060HZ = menu_5060hz/menu_5060hz.cc

SRCS_GAMEPLAY	= gameplay/gpskin.cc gameplay/gpstate.cc \
			gameplay/stepdata.cc \
			gameplay/plrstate.cc \
			gameplay/gameplay.cc \
			gameplay/player.cc \
			gameplay/wordlist.cc

SRCS_BGEFFECTS	= gameplay/bgeffects/bgeffects.cc \
			gameplay/bgeffects/top_scope.cc

# SRCS_EDITOR	= editor/editor.cc editor/ed_popup.cc

SRCS = $(SRCS_MAIN) $(SRCS_DRAWABLES) $(SRCS_CA_INTRO) $(SRCS_MENU_MAIN) \
	$(SRCS_MENU_STAGE) $(SRCS_MENU_PLAYSTYLE) $(SRCS_GAMEPLAY) $(SRCS_TITLE) $(SRCS_ABOUT) \
	$(SRCS_EDITOR) $(SRCS_DEMO) $(SRCS_LUA) $(SRCS_MENU_CHAR) \
	$(SRCS_MENU_WIN) $(SRCS_MENU_SWAPCD) $(SRCS_BGEFFECTS) \
	$(SRCS_MENU_EXTRAS) $(SRCS_MENU_5060HZ) $(SRCS_MENU_VMU) $(SRCS_MENU_SCORE)

# Objects are just the sources switched to .o extensions.
OBJS = $(SRCS:%.cc=%.o)

# Extra libs and CFLAGS for link/compilation
LIBS = -ltsunami -lk++ -lparallax -lpng -lz -ljpeg -lkmg -loggvorbisplay $(DBG_LIBS) -lkosutils -lm
KOS_CFLAGS := $(KOS_CFLAGS) -DBUILD_$(BUILD) # -I$(KOS_BASE)/libk++/STLport-4.5.1/stlport

# Our grand 'all' target
all: rm-elf $(PRECOMPHDR) $(TARGET)

# Again, not supported in GCC 3.0.x :(
#include/global.h.gch: include/global.h
#	$(KOS_CCPLUS) $(KOS_CFLAGS) $(KOS_LOCAL_CFLAGS) -D_arch_$(KOS_ARCH) $(KOS_CPPFLAGS) \
#		$(KOS_ALL_INCS) -c $< -o $@

# Include the default KOS Makefile rules
include $(KOS_BASE)/Makefile.rules

# This really ought to be pulled from the KOS Makefile.rules, but
# I'll be lazy for now...
CMP = $(KOS_CCPLUS) $(KOS_CFLAGS) $(KOS_LOCAL_CFLAGS) -D_arch_$(KOS_ARCH) $(KOS_CPPFLAGS) $(KOS_ALL_INCS)

# Replace the C++ compilation line with one that generates dependencies for us.
# This compiles with -MD to produce a .d file, and then we hack up the .d to
# work a little better with Make, and dump it into $(DEPDIR).
%.o : %.cc
	$(CMP) -MD -o $@ -c $<
	@cp $*.d $(df).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $(df).P; \
	  rm -f $*.d

# Hack around the list of sources until we have a list of dependency includes.
DEPFILES1 = $(SRCS:%.cc=%.P)
DEPFILES2 = $(notdir $(DEPFILES1))
DEPFILES3 = $(addprefix $(DEPDIR)/,$(DEPFILES2))

# Include 'em. Ignore failure, since it generally means an initial build.
-include $(DEPFILES3)

# BSOD has its own dep
main/bsod_snd.o: main/bsod_snd.wav
	$(KOS_BASE)/utils/bin2o/bin2o main/bsod_snd.wav bsod_snd main/bsod_snd.o

# main.cc and init_shutdown.cc themselves depend on Makefile since our 'BUILD' at
# the top influences them a whole lot.
main/main.o: Makefile
main/init_shutdown.o: Makefile

# Clean up everything
clean:
	-rm -f $(TARGET) $(OBJS) romdisk.* $(DEPDIR)/*.P

# Pretty standard KOS boilerplate below this

rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS) romdisk.o
	$(KOS_CCPLUS) $(KOS_CFLAGS) $(KOS_CPPFLAGS) $(KOS_LDFLAGS) \
		-o $(TARGET) $(KOS_START) $(OBJS) romdisk.o $(OBJEXTRA) \
		-L$(KOS_BASE)/lib $(LIBS) $(KOS_LIBS)
#	$(KOS_OBJCOPY) -O binary $(TARGET) $(TARGET).bin
#	$(KOS_CCPLUS) $(KOS_CFLAGS) $(KOS_CPPFLAGS) $(KOS_LDFLAGS) \
#		-Wl,-Ttext=0x00000000 -e _start
#		-o dbg-$(TARGET) $(KOS_BASE)/lib/crt0.o $(OBJS) romdisk.o $(OBJEXTRA) \
#		-L$(KOS_BASE)/lib $(LIBS) -lkos -lkshims -lc -lgcc
#	$(KOS_CCPLUS) $(KOS_CFLAGS) $(KOS_CPPFLAGS) $(KOS_LDFLAGS) \
#		-Wl,-d -Wl,-r -Wl,-S -Wl,-x \
#		-Wl,-T $(KOS_BASE)/loadable/programs/shlelf_dc.xr \
#		-o dbg-$(TARGET) $(KOS_BASE)/lib/crt0.o $(OBJS) romdisk.o $(OBJEXTRA) \
#		-L$(KOS_BASE)/lib $(LIBS) -lkos -lkshims -lc -lgcc

romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d ../$(ROMDISK) -v

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

run: $(TARGET)
	# $(KOS_LOADER) $(TARGET).bin
#	dc-tool -x $(TARGET).bin -i ../swapcds/tmp.iso
	dc-tool -x $(TARGET)

runswap: $(TARGET)
	dc-tool -x $(TARGET).bin -i ../swapcds/max.iso

dist:
	rm -f $(OBJS)
	$(KOS_CC_BASE)/bin/sh-elf-strip $(TARGET)

reset:
	$(KOS_LOADER) reset.elf


# Font generation lines
GTF = $(KOS_BASE)/utils/gentexfont/gentexfont
fonts:
	$(GTF) -fn '-adobe-courier-medium-r-normal--24-*-100-100-p-*-iso8859-1' -file ../gfx/fonts/default.txf
	$(GTF) -fn '-adobe-helvetica-bold-r-normal--32-*-100-100-p-*-iso8859-1' -file ../gfx/fonts/sanserif.txf
	$(GTF) -fn '-larabiefonts-axaxax-medium-r-normal--46-*-100-100-p-*-iso8859-1' -file ../gfx/fonts/axaxax.txf
	$(GTF) -fn '-larabiefonts-axaxax-medium-r-normal--32-*-100-100-p-*-iso8859-1' -file ../gfx/fonts/axaxax_small.txf
	$(GTF) -fn '-2rebels-neuropol-medium-r-normal--46-*-100-100-p-*-iso8859-1' -file ../gfx/fonts/neuropol.txf -w 512
	$(GTF) -fn '-larabiefonts-wintermute-medium-r-normal--32-*-100-100-p-*-iso8859-1' -file ../gfx/fonts/wintermute.txf
#	$(GTF) -fn '-larabiefonts-axaxax-medium-r-normal--64-*-100-100-p-*-iso8859-1' -file ../gfx/fonts/axaxaxbig.txf -w 512
