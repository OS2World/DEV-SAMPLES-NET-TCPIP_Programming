
.SUFFIXES: .cpp .obj .rc .res

.all: ping.exe

.cpp.obj:
	@echo " Compile::C++ Compiler "
	icc.exe /I. /Iinclude /I..\..\pmclass\include /I..\..\nvclass\include /I..\..\netclass\include /Fi /Si /O /Gm /Fo"%|dpfF.obj" /C %s

.rc.res:
	@echo " Compile::Resource Compiler "
	rc.exe -r %s %|dpfF.RES

ping.exe: \
	code\tbartop.obj \
	code\address.obj \
	code\about.obj \
	code\ping.obj \
	ping.res \
	..\..\pmclass\pmclass.lib \
	..\..\nvclass\nvclass.lib \
	..\..\netclass\netclass.lib \
	code\ping.def
	@echo " Link::Linker "
	@echo " Bind::Resource Bind "
	icc.exe @<<
	 /B" /pmtype:pm"
	 /Feping.exe
	 ..\..\pmclass\pmclass.lib
	 ..\..\nvclass\nvclass.lib
	 ..\..\netclass\netclass.lib
	so32dll.lib
	tcp32dll.lib
	 code\ping.def
	 code\tbartop.obj
	 code\address.obj
	 code\about.obj
	 code\ping.obj
<<
	rc.exe ping.res ping.exe

code\ping.obj: code\ping.cpp

code\tbartop.obj: code\tbartop.cpp

code\address.obj: code\address.cpp

code\about.obj: code\about.cpp

ping.res: ping.rc rc.hpp

