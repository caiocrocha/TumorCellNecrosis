# TumorCellNecrosis

Trabalho sobre necrose de células tumorais da disciplina de Solução Numérica de Equações Diferenciais do Departamento de Ciência da Computação da UFJF. 
O artigo referência é "A Parallel 2D Numerical Simulation of Tumor Cells Necrosis by Local Hyperthermia", R F Reis et al 2014 J. Phys.: Conf. Ser. 490 012138. 

## Instruções (Linux)
Compilar o arquivo c, passando a flag -lm.
```
gcc main.c -o main -lm
```

Para executar e salvar a solução a cada segundo, rodar a linha de cmd a seguir. 
```
./main > sol_tempo_h_x_0.001_h_t_0.001_300000_steps.txt
```

Será executada uma simulação de 5 min da temperatura na região tumoral, que leva em torno de 1m30s (Intel i7-1165G7 8GB). 
A solução será salva num arquivo de texto, no qual cada linha representa a matriz de dimensões 101 x 101 "aplanada" (flattened) num dado instante de tempo. 
Cada instante de tempo vale 1 s. 
