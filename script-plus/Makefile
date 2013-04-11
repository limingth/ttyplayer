
all: 
	gcc client.c -o client 
	gcc server.c -o server -lpthread
	@echo
	@echo if you are teacher, make teach 
	@echo if you are student, make learn

# see log
deamon:
	cat sfifo | ./server 

deamon1:
	cat sfifo | ./server 2>/dev/null

deamon2:
	cat sfifo | ./server 2>/dev/null  1>&2 &

# teacher
teach:
	cat sfifo | ./server 2>/dev/null  1>&2 &
	script -f sfifo

# student
learn:	
	./client

elf_install_dir = /usr/local/bin
sh_install_dir = /home/v

install:
	sudo cp teach $(elf_install_dir)
	sudo cp view $(elf_install_dir)
	sudo cp teach $(sh_install_dir)
	sudo cp view $(sh_install_dir)
	sudo cp client $(elf_install_dir)
	sudo cp server $(elf_install_dir)
	sudo cp README $(sh_install_dir)/README

clean:
	-rm server
	-rm client

