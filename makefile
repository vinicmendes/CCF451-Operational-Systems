all:
	gcc src/main.c src/menu.c src/processo/processoControle.c src/processo/processoSimulado.c src/estruturas/cpu.c src/estruturas/estadoBloqueado.c src/estruturas/estadoPronto.c src/estruturas/leitor.c src/estruturas/pipe.c src/estruturas/tempo.c -o main
run:
	./main
clean:
	rm main
