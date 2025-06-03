<!-- Execution -->
[-] check heredoc undefined behavour related to file name when multy minishells are open
[X] unset env and run bash
[x] oldpwd and pwd when unset (cd behavior)
[-] mkdir -p 1/2 && cd 1/2 | chmod 000 1/2 
[-] mkdir -p 1/2 && cd 1/2 | rm -rf 1 | pwd
[-] unset a b
[-] cat < Makefile < dfkjsdf
[-] mkdir -p 1/2 && cd 1/2 | rm -rf 1 | pwd



[-] unset PATH - ls : change error




ljljllhj
ctr_d exit with last exit status
. -> permission dinied 

anachat@e2r8p11:/home/anachat$ ./ls
bash: ./ls: No such file or directory
anachat@e2r8p11:/home/anachat$ bash
anachat@e2r8p11:/home/anachat$ lasdjf
lasdjf: command not found
anachat@e2r8p11:/home/anachat$ lasjfld/lsdjflds
bash: lasjfld/lsdjflds: No such file or directory
ambiguois
..
export $VAR="somthing"
pip leaks






minishell ➤ pwd >> a
minishell ➤ cat a
Cannot find PATH env
cat: command not found



=================================================================
==10896==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 30 byte(s) in 1 object(s) allocated from:
    #0 0x79781cafc778 in realloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:85
    #1 0x79781c71aa97 in __GI___getcwd ../sysdeps/unix/sysv/linux/getcwd.c:86
    #2 0x79781ca7c5dc in getcwd ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:3513
    #3 0x6164beb74325 in ft_cd src/builtins/cd.c:58
    #4 0x6164beb73bc7 in exec_builtin src/builtins/builtins_utils.c:40
    #5 0x6164beb752c7 in exec src/execution/exec.c:69
    #6 0x6164beb689d5 in execute_commands src/minishell.c:35
    #7 0x6164beb693ed in run_shell_loop src/minishell_utils.c:74
    #8 0x6164beb68c59 in main src/minishell.c:65
    #9 0x79781c62a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #10 0x79781c62a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #11 0x6164beb68724 in _start (/home/ayoub/Desktop/minishell/minishell+0x5724) (BuildId: 647d194553e6a0da355b83362d128665b06992d5)

SUMMARY: AddressSanitizer: 30 byte(s) leaked in 1 allocation(s).
minishell ➤ ^C
minishell ➤ 

==========================================





minishell ➤ export a="  "
minishell ➤ $a
	: command not found

segfault in export $a.$a






