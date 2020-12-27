# water-analysis-system

>Elaboração de um projeto de hardware que é capaz de detectar a presença de sal ou açúcar na água.

## Índice

<ol align="left">
  <li><a href="#10-objetivo">Objetivo</a></li>
  <li><a href="#20-proposta-de-solução">Proposta de solução</a></li>
  <li><a href="#30-desenvolvimento">Desenvolvimento</a>
    <ol>
      <li><a href="#31-arduino-uno">Arduino uno</a></li>
      <li><a href="#32-placa-de-circuito-impresso">Placa de circuito impresso</a></li>
      <li><a href="#interface">Interface</a></li>
    </ol>
  </li>
  <li><a href="#40-lista-de-materiais">Lista de materiais</a>
    <ol>
      <li><a href="#41-softwares-de-simulação">Softwares de simulação</a></li>
    </ol>
  </li>
  <li><a href="#50-referências">Referências</a></li>
</ol>

## 1.0 Objetivo

Elaboração de um projeto de hardware que é capaz de detectar a presença de sal na água.


## 2.0 Proposta de solução

Verificação da variação da corrente aplicando voltametria cíclica.
Meu circuito é capaz de detectar a presença de sal na solução uma vez que a resposta do circuito será caracterizado pela variação da corrente, evento este que ocorre devido ao uso da técnica de voltametria cíclica.


## 3.0 Desenvolvimento

A detecção da presença do sal na água é determinada uma vez que houver alteração dos níveis de tensão na entrada do ESP. Uma vez que a água irá se tornar mais condutora com o sal dissolvido nesta, analogamente haverá a detecção da presença do material na água.

Para simular a voltametria cíclica foram levadas em consideração as seguintes condições :

* Pela Lei da corrente Faradáica sabe-se que a corrente gerada pelo eletrodo é proporcional à concentração de analito presente na solução. Tendo isto em vista, foi estabelecido que a corrente máxima e mínima Faradáica seria 200*u*A e -200*u*A respectivamente para fins de simulação.

A proposta do presente projeto foi efetuada com base nas etapadas presentes na seguinte figura :

<p align="center">
  <img width="500px" src=".github/system_schema.png">
</p>


### 3.1 Arduino uno

Foi utilizado o [arduino uno](https://www.farnell.com/datasheets/1682209.pdf) para gerar o sinal PWM como entrada do circuito modelado, sendo também responsável pelo controle da **frequência, amplitude e duty cycle** da onda retangular. O pulso é caracterizado por uma amplitude de 2Vpp (de 0 à 2V) e frequência de aproximadamente 31kHz. Todos esses fatores influenciam na aquisição do sistema e posteriormente serão configuráveis.

Para um primeiro momento, tem-se [código](./embedded_system/pwm_generator.ino) utilizado para a programação do arduino uno e, os pontos em destaque são basicamente a mudança da frequência de saída ao alterar os registradores e o controle do duty cycle pelos dois laços de repetição.

A funcionalidade de oscilação de onda quadrada poderia ser resolvida utilizando um [LM555](https://www.ti.com/lit/ds/symlink/lm555.pdf), porém em atualizações futuras será feita a comunicação entre o arduino uno e o ESP01. Além disso permite adicionar uma maior complexidade ao projeto.


### 3.2 Placa de circuito impresso

Este módulo do projeto é responsável pelo tratamento do sinal de onda quadrada, ajuste de offset e conversão de corrente para tensão.

Na figura abaixo tem-se o esquema do circuito projetado, e destacam-se quatro partes (cada foi parte simulada separadamente). 
<p align="center">
  <img width="700px" src=".github/circuit_schema.png">
</p>

Na figura abaixo observa-se o resultado final da placa do circuito: 
<p align="center">
  <img width="300px" src=".github/pcb_back.png">
  <img width="300px" src=".github/pcb_front.png">
  <img width="300px" src=".github/pcb_front_3d.png">
</p>


### Circuito integrador

A segunda parte do circuito consiste em um integrador, sendo este responsável pela conversão da onda retangular para triangular. Tendo como referência o circuito básico de um integrador (figura 5), é importante atentar-se a amplitude do sinal de saída e ajuste de offset.

<p align="center">
  <img width="500px" src=".github/base_integrator.png"><br>
  Figura 5: Circuito base de um integrador (<a href="#50-referências">4</a>)
</p>

Uma vez que sabe-se as características da tensão de entrada, é necessário estabelecer os valores dos componentes que serão utilizados.
Partindo da fórmula presente na figura acima tem-se que:

V*out* = Vp/2*f*RC

E que V*out* é a tensão desejada de saída e Vp é a tensão de pico. É possível partir para os cálculos já que os seguintes valores são conhecidos:

Vout = 2V,
Vp = 2V,
*f* = 31kHz,
R1 = 4.7kOhms

O valor de R1 é abirtrário, basta substituir pelo valor desejado.
Substituindo os valores na equação, tem-se então:

2 = 2/(2 * 31k * R1 *C)

C = 1/ (62k * 4.7k)

C = 3.43*n*F

Neste caso será utilizado um capacitor de **3.3*n*F**.
E na figura abaixo pode-se observar o circuito final do integrador. É importante destacar a presença do **R2**, resistor de 47kOhms, pois este é responsável pelo ajuste de offset do sinal.

<p align="center">
  <img width="300px" src=".github/integrator2.png"><br>
  Figura: Circuito integrador
</p>

**Resultado da simulação**

<p align="center">
  <img width="500px" src=".github/integrator_simulation.png"><br>
  Figura: Simulação do integrador
</p>


### 3.4 Ajuste de Offset

O sinal PWM gerado pelo arduino uno tem amplitude de 2Vpp, e agora convertido para onda triangular, varia entre 0 e 2V. Entretando na voltametria cíclica se faz necessário utilizar pulsos negativos, portanto
foi adicionado o módulo *Offset Ajustment* para obter uma saída triangular variando entre -1 e 1V. O ajuste é realizado pelo potenciômetro de 100kOms.

<p align="center">
  <img width="300px" src=".github/offset_ajustment_circuit.png"><br>
  Figura : Circuito para ajuste de offset
</p>

**Resultado da simulação**

Percebe-se na figura abaixo a correção do offset, uma vez que a onda triangular verde é o sinal de entradam, e a onda triangular vermelha é a o sinal de saída. Então, nota-se que foi realizada a movimentação da onda triangular no eixo y (tensão), ou seja, foi adicionado o um offset de -1V sob o sinal.

<p align="center">
  <img width="600px" height="300px" src=".github/offset_ajustment.jpg"><br>
  Figura : Resultado da simulação<br>
</p>


### 3.5 Circuito potenciostato

O circuito base consiste em 3 pontos principais, um operacional que realiza a comparação da tensão entre o **CE** (*counter electrode* ou eletrodo contador) e **RE** (*reference electrode* ou eletrodo de referência), de modo que a tensão seja igual à tensão aplicada na entrada do operacional. A segunda parte consiste na configuração **seguidor emissor** para o sinal do eletrodo de referência, esse circuito é necessário pois deseja-se que a corrente presente nesta conexão seja nula ou despresível, fato este possível devido ao amplificador ter alta impedância de entrada.<br>
Por fim a terceira parte é referente a conversão da corrente gerada pelo *working electrode* (WE) ou eletrodo de trabalho para tensão. A conversão é feita pelo resistor R*f*, que terá valor variado dependendo da magnitude da corrente, dependendo da reação eletroquímica os valores podem ter variações consideráveis ou não ([2](https://iopscience.iop.org/article/10.1088/1742-6596/1550/4/042049/pdf)).<br>
A figura abaixo representa o circuito base para controle e medição da resposta dos eletrodos. 

<p align="center">
  <img width="300px" src=".github/potentiostat_base_circuit.png" alt="circuito base potenciostato">
</p>

### 3.6 Conversor Corrente Tensão

Levando em consideração as condições de análise, foi simulado o módulo de conversão da corrente proveniente do eletrodo de trabalho ou *working electrode* (WE). Essa conversão é necessária para que o microcontrolador possa receber o sinal. A saída do conversor é limitada entre 0 e 3.3V já que a tensão será lida pelo [ESP01](#esp01). Pode-se observar na figura abaixo uma entrada de -200*u*A e uma saída de aproximadamente 3.2V, que seria a tensão máxima.   

<p align="center">
  <img width="400px" src=".github/current_to_voltage.jpg">
</p>

### 3.7 ESP01

O ESP01 é um microchip de baixo custo que vem sendo amplamente utilizado em protótipos de projetos de IoT e pode ser programável pela plataforma Arduino.
O módulo ESP-01 suporta o padrão IEEE 802.11 b/g/n, protocolo TCP/IP, modo estação e ponto de acesso, isto permite que seja utilizado para conexão WiFi, controle e criação de redes e comunicação entre aplicações e dispositivos.

Este módulo é utilizado para receber e tratar os sinais de saída do circuito. Uma vez que o ESP é configurado para realizar a leitura do sinal, o microcontrolador irá enviar os dados para o Firebase, onde haverá conexão com um banco de dados em tempo real. E por fim esses dados serão consultados pela [interface](#interface).

O uso da conectividade permite que o usuário possa visualizar os dados remotamente. Para mais detalhes verifique o [código](./embedded_system/esp_firebase.ino) utilizado no ESP. 


### 3.8 Eletrodo

Nos parâmetros utilizados para simular a técnica de voltametria cíclica, levou-se em consideração uma célcula eletroquímica de Platina e eletrodos de carbono ([1](https://run.unl.pt/bitstream/10362/53149/1/Crespo_2017.pdf), pg.35).

## Interface

A interface gráfica é via terminal, e ao executar o programa permite comandos para extrair os dados em arquivo CSV, visualização do gráfico da voltametria cíclica. Além disso o usuário pode controlar o início e o fim da medição. Por fim pode sair do sistema.

<p align="center">
  <img width="400px" src=".github/interface.jpg">
</p>

É necessário que tenha o python instalado na máquina e instalar os pacotes **Pandas** e **Pyrebase**. Para mais detalhes verifique o [código](./interface/main.py).

## 4.0 Lista de Materiais

Lista de componentes e hardware utilizado: 

* Operacional [LM324](https://www.alldatasheet.com/datasheet-pdf/pdf/17871/PHILIPS/LM324.html) - uma unidade contém 4 operacionais;
* Resistores:
	* 1k - 2 unidades;
	* 5.6k - 1 unidade;
	* 9.1k - 1 unidade;
	* 33k - 1 unidade . 
* Potenciômetro de 100k - 1 unidade;
* Capacitor de 10nF - 1 unidade;
* Arduino uno - 1 unidade;
* ESP01 - 1 unidade;
* Fonte de alimentação simétrica +-5V;

### 4.1 Softwares de simulação

* [PSpice](https://www.electronics-lab.com/downloads/circutedesignsimulation/?page=5): software gratuito, para simular os sinais e circuitos apresentados no trabalho.

* [Proteus](#): Na modelagem e construção da placa de circuito impresso foi utilizado o software. 

* [IDE arduino](#): programação e simulação do arduino uno e ESP01 

## 5.0 Referências

[1](#) Ning, Z & Huang, J & Guo, S & Wang, L. (2020). A Portable Potentiostat for Three-Electrode Electrochemical Sensor. Journal of Physics: Conference Series. 1550. 042049. 10.1088/1742-6596/1550/4/042049.<br>

[2](#) Crespo, J. R. (2017). Electrochemical-SERS analysis of R6G using a microcontroller based Portable Potentiostat (Doctoral dissertation).<br>

[3](#) WEBSTER, H. E. J. G.Measurement, Instrumentation, and Sensors Handbook:Electromagnetic, Optical, Radiation, Chemical, and Biomedical Measurement. 2. ed.[S.l.]: CRC Press, 2014. 1921 p. ISBN 1439848912.<br>

[4](#) Malvino, Albert Paul; e Bates, David J.; Eletrônica, vol. 1 e vol. 2, 8a ed., Ed. McGraw Hill-Bookman, São Paulo, SP, 2016. Também em recurso eletrônico.


## Atulizações futuras

[ ] Aquisição de dados de modo offline.<br>
[ ] Remodelagem da placa.<br>
[ ] Ajustes de segurança no cicuito: regulador de tensão, corrente, etc.<br>
[ ] Comunicação entre ESP e Arduino.<br>
[ ] Interface gráfica (web de preferência).