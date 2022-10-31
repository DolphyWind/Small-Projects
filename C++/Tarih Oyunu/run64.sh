APPNAME="60 Saniyede Tarih x64"
OUTPUTPATH="./output/x64/"
mkdir -p $OUTPUTPATH
rm -f $OUTPUTPATH/$APPNAME.exe
x86_64-w64-mingw32-g++ -std=c++17 -DSFML_STATIC -static-libgcc -static-libstdc++ src/*.cpp -Wno-narrowing -Wall -Wextra -O2 -s -DNDEBUG -mwindows -o "$OUTPUTPATH/$APPNAME.exe" -Iinclude -I/home/yunus/ExtraLibs/SFML64/include -L/home/yunus/ExtraLibs/SFML64/lib -Llib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lopengl32 -lwinmm -lgdi32 -lfreetype
wine "$OUTPUTPATH/$APPNAME.exe"
