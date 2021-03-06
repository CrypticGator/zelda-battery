## Process this file with automake to produce Makefile.in

bin_PROGRAMS = no_common.h zbatt zbatc

zbatc_SOURCES = color.c main.c acpi.c power.c concat.c
zbatt_SOURCES = text.c main.c acpi.c power.c concat.c
gzbatt_SOURCES = main_gui.pyx main.c acpi.c power.c concat.c
noinst_HEADERS = main.h acpi.h power.h compat.h

iconsdir = $(pkgdatadir)/icons
scaledir = $(iconsdir)/scalable/apps
pngdir = $(iconsdir)/16x16/apps

AM_CFLAGS =
AM_LDFLAGS =
AM_CPPFLAGS =

AM_CFLAGS += -O2 -Wall -Wextra
AM_CPPFLAGS += -DNO_COMMON_H

if TEST
AM_CPPFLAGS += -DZB_ACPI_ROOT=\"$(top_srcdir)/test\"
endif

if DEBUG
AM_CFLAGS += -g -ggdb
AM_CPPFLAGS += -DZB_DEBUG=1
endif

if !DEBUG
if NO_DEBUG
AM_CPPFLAGS += -DZB_DEBUG=0
endif
endif

if NATIVE
if !GENERIC
AM_CFLAGS += -march=native -mtune=native
endif
endif

if GENERIC
if CPU_KNOWN
AM_CFLAGS += -mtune=generic-$(CPUTYPE)
else
AM_CFLAGS += -mtune=generic
endif
endif

zbatt_CFLAGS = $(AM_CFLAGS)
zbatc_CFLAGS = $(AM_CFLAGS)

if STATIC
zbatt_CFLAGS += --static
zbatc_CFLAGS += --static
endif

zbatc_CPPFLAGS = -DZB_PROGNAME=\"zbatc\" $(AM_CPPFLAGS)
zbatt_CPPFLAGS = -DZB_PROGNAME=\"zbatt\" $(AM_CPPFLAGS)
gzbatt_CFLAGS = $(AM_CFLAGS)
gzbatt_CPPFLAGS = -DZB_PROGNAME=\"gzbatt\" $(AM_CPPFLAGS)
gzbatt_LDFLAGS = $(AM_LDFLAGS)

if GUI
gzbatt_CPPFLAGS += -DPNGDIR=\"$(pngdir)\" \
-DDATADIR=\"$(pkgdatadir)\"
endif

if GTK2
gzbatt_CPPFLAGS += -DWITH_GTK2=1
gzbatt_CFLAGS += @gtk2_CFLAGS@ @python2_CFLAGS@
gzbatt_LDFLAGS += @gtk2_LIBS@ @python2_LIBS@
bin_PROGRAMS += gzbatt
endif

.PHONY: valgrind lint
all-local:

no_common.h:
	ln -sf "main.h" "no_common.h"

move-local: $(bin_PROGRAMS)
	for file in $?; do mv "$$file" ../../; done

clean-local: tidy-local

lint:
	for f in $(zbatt_SOURCES) $(zbatc_SOURCES); do cppcheck --enable=all $$f; echo; done

valgrind: clean $(bin_PROGRAMS)
	for f in $(bin_PROGRAMS); do \
		test "$$f" = "gzbatt" || \
		$@ --leak-check=full --show-leak-kinds=all --track-origins=yes ./"$$f"; \
		echo; \
		echo; \
	done

vvalgrind: clean $(bin_PROGRAMS)
	for f in $(bin_PROGRAMS); do \
		test "$$f" = "gzbatt" || \
		valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./"$$f"; \
		echo; \
		echo; \
	done

mtrace: $(bin_PROGRAMS)
	for f in $^; do MALLOC_TRACE=./mtrace.txt ./$$f; mtrace ./$$f ./mtrace.txt; echo; cat mtrace.txt; echo; echo; done

expand:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -E $(zbatt_SOURCES)
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -E $(zbatc_SOURCES)

check-local: $(bin_PROGRAMS) refresh-check
	cd test; \
	./configure

refresh-check:
	cd test; \
	autoconf

asm: $(zbatt_SOURCES:.c=.s) $(zbatc_SOURCES:.c=.s) $(bin_PROGRAMS)
	for f in $(zbatt_SOURCES:.c=.o); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done
	for f in $(zbatc_SOURCES:.c=.o); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done

.c.s:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -S $<

.pyx.c:
	@GPP@ $(gzbatt_CPPFLAGS) -o $(@:.c=.co) $<
	$(CYTHON) --embed -p -2 -Wextra -o $@ $(@:.c=.co)

tidy-local:
	$(RM) *.o || true
	$(RM) *.od || true
	$(RM) *.py || true
	$(RM) *.pyc || true
	$(RM) main_gui.c || true
	$(RM) main_gui.co || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null

drone:
	cd /sys/class/power_supply; ls -R; ls -R /sys/class/power_supply

akpi:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) $(AM_LDFLAGS) -o acpi acpi.c main.c

test-setup:
	cd "$(top_srcdir)"; $(MAKE) $@

test-clean:
	cd "$(top_srcdir)"; $(MAKE) $@

test-onac:
	cd "$(top_srcdir)"; $(MAKE) $@

test-noac:
	cd "$(top_srcdir)"; $(MAKE) $@

test-lowbat:
	cd "$(top_srcdir)"; $(MAKE) $@

test-empty:
	cd "$(top_srcdir)"; $(MAKE) $@

test-nosup:
	cd "$(top_srcdir)"; $(MAKE) $@

prod:
	cd "$(top_srcdir)"; \
	autoconf && \
	./configure --disable-debug --disable-testdir --enable-static --disable-native --enable-generic && \
	$(MAKE)

no-debug:
	cd "$(top_srcdir)"; \
	autoconf && \
	./configure --disable-debug --enable-generic --disable-native && \
	$(MAKE)

check-configure: refresh-check
	cd test && \
	cat configure | less

check-all: $(bin_PROGRAMS) gzbatt refresh-check
	cd test && \
	./configure --with-gtk2

debug:
	cd "$(top_srcdir)"; \
	autoconf && \
	./configure --enable-debug --enable-testdir --enable-generic --disable-native && \
	$(MAKE)
