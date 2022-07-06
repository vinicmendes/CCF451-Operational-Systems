all:
	gcc main.c menu.c processo/processoControle.c processo/processoSimulado.c estruturas/cpu.c estruturas/estadoBloqueado.c estruturas/estadoPronto.c estruturas/leitor.c estruturas/pipe.c estruturas/tempo.c -o main 
input:
	./main < entrada 2> log
run:
	./main 2> log
clean:
	rm main
