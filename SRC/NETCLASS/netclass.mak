.SUFFIXES: .CPP .LIB .dll .obj

.all: NetClass.LIB

.CPP.obj:
	@echo " Compile::C++ Compiler "
	icc.exe /Iinclude /I..\pmclass\include /DSTATIC_LINK /Fi /Si /O /Gm /Ge- /Fo"%|dpfF.obj" /C %s

.dll.LIB:
	@echo " Lib::Import Lib "
	implib.exe %|dpfF.LIB %s

NetClass.dll: \
	code\NETNEWS.obj \
	code\netping.obj \
	code\NET.obj \
	code\NETFTP.obj \
	..\pmclass\pmclass.lib \
	code\netclass.def
	@echo " Link::Linker "
	icc.exe @<<
	 /B" /noe"
	 /FeNetClass.dll
	 so32dll.lib
	 tcp32dll.lib
     ..\pmclass\pmclass.lib 
     code\netclass.def
     code\NETNEWS.obj
     code\netping.obj
     code\NET.obj
     code\NETFTP.obj
<<

code\NETNEWS.obj: code\NETNEWS.CPP

code\NETFTP.obj: code\NETFTP.CPP

code\NET.obj: code\NET.CPP

code\netping.obj: code\netping.cpp

NetClass.LIB: NetClass.dll
