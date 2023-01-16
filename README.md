# TumorCellNecrosis

Trabalho sobre necrose de células tumorais da disciplina de Solução Numérica de Equações Diferenciais do Departamento de Ciência da Computação da UFJF. 
O artigo referência é "A Parallel 2D Numerical Simulation of Tumor Cells Necrosis by Local Hyperthermia", R F Reis et al 2014 J. Phys.: Conf. Ser. 490 012138. 

## Instruções (Linux)
Compilar o arquivo c, passando a flag -lm.
```
gcc main.c -o main -lm
```

Para executar e imprimir na tela a solução a cada segundo de simulação, rodar a linha de cmd a seguir. 
```
./main false
```

O argumento *false* representa a simulação sem advecção. Para rodá-la com advecção, passar o argumento *true*.
Será executada uma simulação de 10 min da temperatura na região tumoral, que leva em torno de 3m20s (Intel i7-1165G7 8GB). 
A solução será impressa numa única linha a cada passo de tempo, que representa a matriz de dimensões 101 x 101 "aplanada" (flattened). 

Para salvá-la num arquivo, é possível redirecionar a saída com o comando a seguir.
```
./main false > sol_tempo.txt
```

É possível também gerar gráficos 3D da solução ao longo do tempo de simulação e do mapa de calor da solução no último instante de tempo. Para isso, executar o script *plot_solution.py*, da seguinte maneira.

```
python3 plot_solution.py sol_tempo.txt figs/
```

O primeiro argumento é o arquivo de texto gerado pelo programa principal e o segundo argumento é o diretório que será criado para salvar as imagens. 
