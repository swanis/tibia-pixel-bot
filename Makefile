FILES=fartyg.cpp essentials.cpp healer.cpp targetter.cpp looter.cpp tracker.cpp cavebotter.cpp waypoint.cpp
CFLAGS=
LFLAGS=User32.lib Gdi32.lib Kernel32.lib gdiplus.lib /LTCG /OPT:REF /OPT:ICF /INCREMENTAL:NO

fartyg:
	cl /W4 /EHsc /O2 /Ob2 /Oi /Ot /Oy /GL /Gy /GF /MD /GS- /fp:precise $(FILES) $(CFLAGS) /link $(LFLAGS)
#DEBUG	cl /W4 /EHsc /Zi /MDd /RTC1 $(FILES) $(CFLAGS) /link $(LFLAGS)

# /MD is apparently the correct optimization setting that matches release-mode in VS
# There is also /O2 etc
