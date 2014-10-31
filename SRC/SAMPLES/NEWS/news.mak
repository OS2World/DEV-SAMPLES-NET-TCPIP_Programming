.SUFFIXES: .cpp .obj .rc .res

.all: news.exe

.cpp.obj:
	@echo " Compile::C++ Compiler "
	icc.exe /I. /Iinclude /I..\..\pmclass\include /I..\..\nvclass\include /I..\..\netclass\include /Fi /Si /O /Gm /Fo"%|dpfF.obj" /C %s

.rc.res:
	@echo " Compile::Resource Compiler "
	rc.exe -r %s %|dpfF.RES

news.exe: \
	code\news.obj \
	code\groups.obj \
	code\subs.obj \
	code\msg.obj \
	code\article.obj \
	code\about.obj \
	code\conmgr.obj \
	code\contgrp.obj \
	code\contsub.obj \
	code\contmsg.obj \
	code\tbartop.obj \
	code\tbargrp.obj \
	code\tbarsub.obj \
	code\tbarmsg.obj \
	code\tbarart.obj \
	news.res \
	..\..\pmclass\pmclass.lib \
	..\..\nvclass\nvclass.lib \
	..\..\netclass\netclass.lib \
	code\news.def
	@echo " Link::Linker "
	@echo " Bind::Resource Bind "
	icc.exe @<<
	 /B" /pmtype:pm"
	 /Fenews.exe
	 ..\..\pmclass\pmclass.lib
	 ..\..\nvclass\nvclass.lib
	 ..\..\netclass\netclass.lib
	so32dll.lib
	tcp32dll.lib
	 code\news.def
	 code\news.obj
	 code\groups.obj
	 code\subs.obj
	 code\msg.obj
	 code\article.obj
	 code\about.obj
	 code\conmgr.obj
	 code\contgrp.obj
	 code\contsub.obj
	 code\contmsg.obj
	 code\tbartop.obj
	 code\tbargrp.obj
	 code\tbarsub.obj
	 code\tbarmsg.obj
	 code\tbarart.obj
<<
	rc.exe news.res news.exe

code\news.obj: code\news.cpp

code\groups.obj: code\groups.cpp

code\subs.obj: code\subs.cpp

code\msg.obj: code\msg.cpp

code\article.obj: code\article.cpp

code\about.obj: code\about.cpp

code\conmgr.obj: code\conmgr.cpp

code\contgrp.obj: code\contgrp.cpp

code\contsub.obj: code\contsub.cpp

code\contmsg.obj: code\contmsg.cpp

code\tbartop.obj: code\tbartop.cpp

code\tbargrp.obj: code\tbargrp.cpp

code\tbarsub.obj: code\tbarsub.cpp

code\tbarmsg.obj: code\tbarmsg.cpp

code\tbarart.obj: code\tbarart.cpp

news.res: news.rc rc.hpp

