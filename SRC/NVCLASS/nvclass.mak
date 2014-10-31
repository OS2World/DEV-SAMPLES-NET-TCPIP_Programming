.SUFFIXES: .LIB .cpp .dll .obj 

.all: NVClass.LIB

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Iinclude /I..\pmclass\include /DSTATIC_LINK /Fi /Si /O /Gm /Ge- /Fo"%|dpfF.obj" /C %s

.dll.LIB:
    @echo " Lib::Import Lib "
    implib.exe %|dpfF.LIB %s

NVClass.dll: \
    code\thread.obj \
    code\THREADPM.obj \
    code\INI.obj \
    code\INISYS.obj \
    code\INIUSER.obj \
    code\SEMEV.obj \
    code\nvclass.def
    @echo " Link::Linker "
    icc.exe @<<
     /B" /noe"
     /FeNVClass.dll 
     code\nvclass.def
     code\thread.obj
     code\THREADPM.obj
     code\INI.obj
     code\INISYS.obj
     code\INIUSER.obj
     code\SEMEV.obj
<<

code\thread.obj: code\thread.cpp

code\SEMEV.obj: code\SEMEV.CPP

code\INIUSER.obj: code\INIUSER.CPP

code\INISYS.obj: code\INISYS.CPP

code\INI.obj: code\INI.CPP

code\THREADPM.obj: code\THREADPM.CPP

code\NVClass.LIB: NVClass.dll
