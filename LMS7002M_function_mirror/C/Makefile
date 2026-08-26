CC = gcc

TARGET = menu.exe

MINGW_BIN = C:/Program Files/CodeBlocks/MinGW/bin

SRCS = main.c functions_AIP.c lms7002m_functions.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -O2 -I"C:/ipdi/wrappers/C/include"

LDFLAGS = -L"C:/ipdi/lib" \
          -L"C:/ipdi/lib/ip" \
          -L"C:/ipdi/wrappers/C/lib"

LDLIBS = -lcaip

all: $(TARGET)

#$(TARGET): $(OBJS)
# $(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $(TARGET)
#		$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -static-libgcc -static-libstdc++ -o $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -static-libgcc -static-libstdc++ -o $(TARGET)
#	-cmd /C copy /Y "$(MINGW_BIN)\libgcc_s_seh-1.dll" "."
#	-cmd /C copy /Y "$(MINGW_BIN)\libstdc++-6.dll" "."
#	-cmd /C copy /Y "$(MINGW_BIN)\libwinpthread-1.dll" "."

main.o: main.c functions_AIP.h lms7002m_functions.h
	$(CC) $(CFLAGS) -c main.c -o main.o

functions_AIP.o: functions_AIP.c functions_AIP.h
	$(CC) $(CFLAGS) -c functions_AIP.c -o functions_AIP.o

lms7002m_functions.o: lms7002m_functions.c lms7002m_functions.h functions_AIP.h
	$(CC) $(CFLAGS) -c lms7002m_functions.c -o lms7002m_functions.o

clean:
	-del /Q *.o $(TARGET) 2>NUL