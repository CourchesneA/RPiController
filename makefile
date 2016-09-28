pastaDipper: pastaDipper.c
	gcc -g -o pastaDipper pastaDipper.c  -I/usr/local/include -L/usr/local/lib -lwiringPi
clean:
	rm -f pastaDipper
