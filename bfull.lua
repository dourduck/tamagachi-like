#!/usr/bin/env lua

os.execute("echo '[***BULIDING***]'")
-- os.execute(
-- 	"gcc -o game ./lib/raygui.so ./src/main.c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
os.execute(
	"gcc -o command_example ./lib/raygui.so ./src/command_example.c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")

-- os.execute("mv src/raygui.h src/raygui.c")
-- os.execute(
-- 	"gcc -o raygui.so src/raygui.c -shared -fpic -DRAYGUI_IMPLEMENTATION -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11 -lGL")
-- os.execute("mv src/raygui.c src/raygui.h")
--


-- os.execute("echo '[***BULIDING RAYGUI LIB***]'")
-- os.execute("gcc ./src/raygui.c -c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
--
-- os.execute("echo '[***BULIDING CR LIB***]'")
-- os.execute("gcc cr.c -c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
--
-- os.execute("echo '[***BULIDING GAME LIB***]'")
-- os.execute("gcc game.c -c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
--
-- os.execute("echo '[***BUILDING RENDER LIB***]'")
-- os.execute("gcc render.c -c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
--
-- os.execute("echo '[***BUILDING MAIN***]'")
-- os.execute("gcc -c main.c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
--
-- os.execute("echo '[***LINKING***]'")
-- os.execute(
-- 	"gcc -o game main.o str.o cr.o game.o render.o -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
