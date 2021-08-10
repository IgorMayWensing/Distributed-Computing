Aluno: Igor May Wensing
Matricula: 17203362

Para compilar e rodar o server execute os comandos:

	$ gcc server.c -lm -o server
	$ ./server

Para compilar e rodar o client execute os comandos:
	
	$ gcc client.c -o client
	$ ./client



Para formatação do input escolhi o formato 2 sugerido no enunciando do trabalho, sendo ele:

Formato 2:  “op#op1#op2”: indica no primeiro byte o símbolo do operador e posteriormente os operandos. É utilizado um marcador de separação (#) entre operador e operandos.

Ex: “23+41”, “+#23#41”