@ECHO OFF
IF NOT EXIST Build mkdir Build

pushd Build

SET COMMOM_COMPILER_FLAGS=-MDd -nologo -GR- -Gm- -EHa- -Od -Oi -WX -W4 -wd4701 -wd4324 -wd4477 -wd4459 -wd4700 -wd4100 -wd4996 -wd4201 -wd4530 -wd4189 -wd4244 -wd4101 -wd4312 -wd4456 -wd4457 -wd4505 -FC -Z7 -std:c++20
SET PLATFORM_LINKER_FLAGS=-incremental:no -opt:ref -NODEFAULTLIB:MSVCRT opengl32.lib user32.lib gdi32.lib shell32.lib
SET LINKER_FLAGS=-incremental:no -opt:ref -NODEFAULTLIB:MSVCRT
SET SRC=..\Code\Vectors.cpp ..\Code\Matrices.cpp

del *.pdb>NUL 2>NUL
echo WAITING FOR PDB > lock.tmp
cl %COMMOM_COMPILER_FLAGS% ..\Code\Platform.cpp ..\Code\Vectors.cpp ..\Code\Matrices.cpp /LD /link%PLATFORM_LINKER_FLAGS% -PDB:Platform%random%.pdb /EXPORT:UpdateGame
del lock.tmp
cl %COMMOM_COMPILER_FLAGS% ..\Code\Main.cpp ..\Code\Vectors.cpp ..\Code\Matrices.cpp -FmMain.mp /link%LINKER_FLAGS% -PDB:Main%random%.pdb
popd