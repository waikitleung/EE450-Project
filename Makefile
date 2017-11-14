all:
	gcc -o client client.c
	gcc -o aws aws.c
	gcc -o serverA severA.c
	gcc -o serverB severB.c
	gcc -o serverC severC.c

serverA:
	./serverA

serverB:
	./serverB

serverC:
	./serverC

aws:
	./aws
.PHONY: serverA serverB serverC aws 
