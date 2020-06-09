
build : r.c q.c c.c p.c projectLib.c bin/
	gcc -std=gnu90 r.c projectLib.c -o bin/r
	gcc -std=gnu90 q.c projectLib.c -o bin/q
	gcc -std=gnu90 p.c projectLib.c -o bin/p
	gcc -std=gnu90 c.c projectLib.c -o bin/c
	gcc -std=gnu90 m.c projectLib.c -o bin/m

bin :
	mkdir bin

help :
	echo "utilizzare ./m per avviare il main"

r : r.c projectLib.c
	gcc -std=gnu90 r.c projectLib.c -o r

q : q.c projectLib.c
	gcc -std=gnu90 q.c projectLib.c -o q

p : p.c q.c projectLib.c q
	gcc -std=gnu90 p.c projectLib.c -o p

c : c.c p.c q.c projectLib.c p q
	gcc -std=gnu90 c.c projectLib.c -o c
