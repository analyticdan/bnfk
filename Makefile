default:
	cc interpreter/bf.c -o bfi
	cc compiler/bf.c -o bfc
i:
	cc interpreter/bf.c -o bfi
c:
	cc compiler/bf.c -o bfc
clean:
	rm bf