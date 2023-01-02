APPNAME="gezgin"
OUTPUTPATH="./output/x86"
mkdir -p $OUTPUTPATH
rm -f $OUTPUTPATH/$APPNAME.exe
DLL1="/usr/lib/gcc/i686-w64-mingw32/9.3-win32/libstdc++-6.dll"
DLL2="/usr/lib/gcc/i686-w64-mingw32/9.3-win32/libgcc_s_sjlj-1.dll"
export WINEPATH="/usr/i686-w64-mingw32/lib"
if test -f "$DLL1"; then
	cp "$DLL1" "$OUTPUTPATH"
fi
if test -f "$DLL2"; then
	cp "$DLL2" "$OUTPUTPATH"
fi
i686-w64-mingw32-g++ -std=c++17 src/*.cpp -Wall -Wextra -O2 -s -DNDEBUG -o "$OUTPUTPATH/$APPNAME.exe"
wine "$OUTPUTPATH/$APPNAME.exe"
