.SUFFIXES: .cpp .obj .rc .res

.all: ftp.exe

.cpp.obj:
	@echo " Compile::C++ Compiler "
	icc.exe /I. /Iinclude /I..\..\pmclass\include /I..\..\nvclass\include /I..\..\netclass\include /Fi /Si /O /Gm /Fo"%|dpfF.obj" /C %s

.rc.res:
	@echo " Compile::Resource Compiler "
	rc.exe -r %s %|dpfF.RES

ftp.exe: \
	code\tbartop.obj \
	code\ftp.obj \
	ftp.res \
	..\..\pmclass\pmclass.lib \
	..\..\nvclass\nvclass.lib \
	..\..\netclass\netclass.lib \
	code\ftp.def
	@echo " Link::Linker "
	@echo " Bind::Resource Bind "
	icc.exe @<<
	 /B" /pmtype:pm"
	 /Feftp.exe
	 ..\..\pmclass\pmclass.lib
	 ..\..\nvclass\nvclass.lib
	 ..\..\netclass\netclass.lib
	so32dll.lib
	tcp32dll.lib
	 code\ftp.def
	 code\tbartop.obj
	 code\ftp.obj
<<
	rc.exe ftp.res ftp.exe

code\ftp.obj: code\ftp.cpp

code\tbartop.obj: code\tbartop.cpp

ftp.res: ftp.rc rc.hpp

