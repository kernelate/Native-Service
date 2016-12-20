SOURCE_FILE=    \
	platformservice.c	\
	socket_server_api.c	\
	connectivity.c \
	wpa.c	\
	wifi_ap.c	\
	ldr_ir.c  \
	watchdog_Api.c\
	android_ui_Api.c\
	file_operation.c\
	custom_app_api.c

SOURCE_FILEX=    \
	keypress.c


COMPILER='/home/ntekcom27/FriendlyARM/toolschain/4.5.1/bin/arm-linux-gcc' 
FILENAME='platformservice'
NATIVE_FILENAME='platformservice'
PARM=-lpthread -static -lm -o  $(FILENAME)
NATIVE_PARM=-o $(NATIVE_FILENAME);
FILENAMEX='a.out'


default:
	clear
	$(COMPILER) $(SOURCE_FILE) $(PARM)
	$(COMPILER) -static -o test_platformservice socket_client_test.c
	chmod 777 test_platformservice
	chmod 777 platformservice
clean:
	rm $(FILENAME) $(NATIVE_FILENAME)
native:
	g++ $(SOURCE_FILE) $(NATIVE_PARM)
push:
	clear
	$(COMPILER) $(SOURCE_FILE) $(PARM)
	$(COMPILER) -static -o test_platformservice socket_client_test.c
	chmod 777 test_platformservice
	chmod 777 platformservice
	adb remount
	adb push test_platformservice /data/
	adb push platformservice /data/
