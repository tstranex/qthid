LIBS=`pkg-config libusb-1.0 --libs`
CFLAGS=`pkg-config libusb-1.0 --cflags` -I.. -Wall

SOURCES = \
	main.cpp \
	../fcd.c

qthid-cli: $(SOURCES) hid-libusb.o
	g++ $(CFLAGS) $(LIBS) -o qthid-cli $(SOURCES) hid-libusb.o

hid-libusb.o: ../hid-libusb.c
	gcc $(CFLAGS) -c -o hid-libusb.o ../hid-libusb.c
