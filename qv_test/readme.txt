<Guide to create a qpc project>
0. Create a QPC folder, and create QP, QP_port, QS and include subfolders.
1. In QP folder add C:\qp\qpc\src\qf sources files into QP folder. Also add C:\qp\qpc\src\<your kernel> files into QP folder too, for example qv kernel, add qv.c into QP folder.

2. In QP_port folder. Select your target core, qp kernel and compiler. For example, files in C:\qp\qpc\ports\arm-cm\qv\armclang for arm cortex-m, qv kernel and arm-clang compiler. 

3. In QS folder. Copy all files execpt qutest.c in C:\qp\qpc\src\qs into QS folder.

4. Copy files in C:\qp\qpc\include into include subfolder.

5. Confing include path in you IDE, specifically:
	a. ../QPC/include
	b. ../QPC/QP_port

6. At lease implement following callback functions: Q_onAssert, QF_onCleanup, QF_onStartup, QV_onIdle.

	