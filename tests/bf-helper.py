# Turns a line from stdin into bf code that writes that same line to
# stdout (including the newline).

s = bytearray(input('>> ') + '\n', 'utf-8')
while len(s) > 0:
	c = s.pop(0)
	for _ in range(c):
		print("+", end="")
	print(".>")



