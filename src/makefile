all:
	gcc main.c menu.c processo/processoControle.c processo/processoSimulado.c estruturas/cpu.c estruturas/estadoBloqueado.c estruturas/estadoPronto.c estruturas/leitor.c estruturas/pipe.c estruturas/tempo.c -o main
run:
	./main
clean:
	rm main
debug:
	gcc main.c menu.c processo/processoControle.c processo/processoSimulado.c estruturas/cpu.c estruturas/estadoBloqueado.c estruturas/estadoPronto.c estruturas/leitor.c estruturas/pipe.c estruturas/tempo.c -o main && ./main < input 2> debug