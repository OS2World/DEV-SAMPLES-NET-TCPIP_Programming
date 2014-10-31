.SUFFIXES: .LIB .cpp .dll .obj 

.all: PMClass.LIB

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Iinclude /DSTATIC_LINK /Fi /Si /O /Gm /Ge- /Fo"%|dpfF.obj" /C %s

.dll.LIB:
    @echo " Lib::Import Lib "
    implib.exe %|dpfF.LIB %s

PMClass.dll: \
    code\window.obj \
    code\winstd.obj \
    code\app.obj \
    code\BUTTON.obj \
    code\CONTAIN.obj \
    code\dialog.obj \
    code\EDIT.obj \
    code\LISTBOX.obj \
    code\log.obj \
    code\menu.obj \
    code\mle.obj \
    code\pushbtn.obj \
    code\slider.obj \
    code\status.obj \
    code\tbar.obj \
    code\WINCHILD.obj \
    code\pmclass.def
    @echo " Link::Linker "
    icc.exe @<<
     /B" /noe"
     /FePMClass.dll 
     code\pmclass.def
     code\window.obj
     code\winstd.obj
     code\app.obj
     code\BUTTON.obj
     code\CONTAIN.obj
     code\dialog.obj
     code\EDIT.obj
     code\LISTBOX.obj
     code\log.obj
     code\menu.obj
     code\mle.obj
     code\pushbtn.obj
     code\slider.obj
     code\status.obj
     code\tbar.obj
     code\WINCHILD.obj
<<

code\window.obj: code\window.cpp

code\WINCHILD.obj: code\WINCHILD.CPP

code\tbar.obj: code\tbar.cpp

code\status.obj: code\status.cpp

code\slider.obj: code\slider.cpp

code\pushbtn.obj: code\pushbtn.cpp

code\mle.obj: code\mle.cpp

code\menu.obj: code\menu.cpp

code\log.obj: code\log.cpp

code\LISTBOX.obj: code\LISTBOX.CPP

code\EDIT.obj: code\EDIT.CPP

code\dialog.obj: code\dialog.cpp

code\CONTAIN.obj: code\CONTAIN.CPP

code\BUTTON.obj: code\BUTTON.CPP

code\app.obj: code\app.cpp

code\winstd.obj: code\winstd.cpp

PMClass.LIB: PMClass.dll
