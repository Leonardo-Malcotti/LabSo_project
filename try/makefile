
compile : r.c q.c c.c p.c projectLib.c
	gcc -std=gnu90 r.c projectLib.c -o r
	gcc -std=gnu90 q.c projectLib.c -o q
	gcc -std=gnu90 p.c projectLib.c -o p
	gcc -std=gnu90 c.c projectLib.c -o c

r : r.c projectLib.c
	gcc -std=gnu90 r.c projectLib.c -o r

q : q.c projectLib.c
	gcc -std=gnu90 q.c projectLib.c -o q

p : p.c q.c projectLib.c q
	gcc -std=gnu90 p.c projectLib.c -o p

c : c.c p.c q.c projectLib.c p q
	gcc -std=gnu90 c.c projectLib.c -o c
