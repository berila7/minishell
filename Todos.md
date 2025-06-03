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






==========================================





minishell ➤ export a="  "
minishell ➤ $a
	: command not found