build :
	gcc main.c -o phoenix

install :
	gcc main.c -o phoenix
	mkdir -p ${HOME}/.local/bin
	mv ./phoenix ${HOME}/.local/bin
