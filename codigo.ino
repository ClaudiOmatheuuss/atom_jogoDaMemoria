// code with ♥ by: ATOM
// instagram: @atom_robot

byte escolhaDesligada = 0;
byte escolhaNenhuma = 0;

#define escolhaVermelho (1 << 0)
#define escolhaVerde (1 << 1)
#define escolhaAzul (1 << 2)
#define escolhaAmarelo (1 << 3)
// dif
#define escolhaEasy (1 << 4)
#define escolhaMid (1 << 5)
#define escolhaHard (1 << 6)

int ledVermelho = 10;
int ledVerde = 3;
int ledAzul = 13;
int ledAmarelo = 5;

int botaoVermelho = 9;
int botaoVerde = 2;
int botaoAzul = 12;
int botaoAmarelo = 6;

// leitura: modo atracao
byte leitura;

// dif
int botaoEasy = 7;
int botaoMid = 8;
int botaoHard = 11;

int buzzer1 = A1;
int buzzer2 = A0;

byte tabuleiroJogo[10];  // armazena todos os movimentos(sequencias) de cada level

byte dificuldade;

// dependem da dificuldade
int rodadasParaVencer;
int tempoEspera;

byte rodadaJogo = 0;  

void setup()
{
  Serial.begin(9600);
  pinMode(botaoVermelho, INPUT);
  pinMode(botaoVerde, INPUT);
  pinMode(botaoAzul, INPUT);
  pinMode(botaoAmarelo, INPUT);
  
  pinMode(botaoEasy, INPUT);
  pinMode(botaoMid, INPUT);
  pinMode(botaoHard, INPUT);

  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);

  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
}

void loop()
{
  // comeca com modoAtracao() ate que seja escolhida a dificuldade desejada: escolhaEasy escolhaMid ou escolhaHard
  dificuldade = modoAtracao();

  setarLEDs(escolhaVermelho | escolhaVerde | escolhaAzul | escolhaAmarelo); 
  delay(1000);
  setarLEDs(escolhaDesligada); 
  delay(2000);

  if (jogarMemoria(dificuldade) == true) 
    tocarVencedor(); 
  else 
    tocarPerdedor(); 
}

boolean jogarMemoria(byte dificuldade)
{
  randomSeed(millis()); 

  if (dificuldade == escolhaEasy) {
    rodadasParaVencer = 5;
    tempoEspera = 5000;
  } else if (dificuldade == escolhaMid) {
    rodadasParaVencer = 10;
    tempoEspera = 4000;
  } else if (dificuldade == escolhaHard) {
    rodadasParaVencer = 15;
    tempoEspera = 2000;
  }

  rodadaJogo = 0; 

  while (rodadaJogo < rodadasParaVencer) 
  {
    adicionarAosMovimentos(); 

    reproduzirMovimentos(); 

    for (byte movimentoAtual = 0 ; movimentoAtual < rodadaJogo ; movimentoAtual++)
    {
      byte escolha = esperarPorBotao(tempoEspera); 

      if (escolha == escolhaNenhuma) return false; 

      // botoes de opcao dificuldade aqui sao descartados da situacao em que o jogador perde, isto eh, levamos em consideracao apenas os 4 botoes(vermelho, verde, azul e amarelo)
      if (escolha != tabuleiroJogo[movimentoAtual] && escolha != escolhaEasy && escolha != escolhaMid && escolha != escolhaHard) return false; 
    }

    delay(1000); 
  }

  return true; 
}

void reproduzirMovimentos(void)
{
  for (byte movimentoAtual = 0 ; movimentoAtual < rodadaJogo ; movimentoAtual++) 
  {
    tonar(tabuleiroJogo[movimentoAtual], 150);

    delay(150); 
  }
}

void adicionarAosMovimentos(void)
{
  // gera um byte que representa um numero de 0 a 4
  byte novoBotao = random(0, 4); 

  // identifica o botao escolhido de maneira randomica 
  if(novoBotao == 0) novoBotao = escolhaVermelho;
  else if(novoBotao == 1) novoBotao = escolhaVerde;
  else if(novoBotao == 2) novoBotao = escolhaAzul;
  else if(novoBotao == 3) novoBotao = escolhaAmarelo;

  // incrementa o botao que eh representado por um byte a tabuleiroJogo ao fim desse array
  tabuleiroJogo[rodadaJogo++] = novoBotao; 
}

void setarLEDs(byte leds)
{
  if (leds & escolhaVermelho)
    digitalWrite(ledVermelho, HIGH);
  else
    digitalWrite(ledVermelho, LOW);

  if (leds & escolhaVerde)
    digitalWrite(ledVerde, HIGH);
  else
    digitalWrite(ledVerde, LOW);

  if (leds & escolhaAzul)
    digitalWrite(ledAzul, HIGH);
  else
    digitalWrite(ledAzul, LOW);

  if (leds & escolhaAmarelo)
    digitalWrite(ledAmarelo, HIGH);
  else
    digitalWrite(ledAmarelo, LOW);
}

byte esperarPorBotao(int tempoEspera)
{
  long tempoInicio = millis(); 
  int limiteTempoEntrada = tempoEspera;

  while ((millis() - tempoInicio) < limiteTempoEntrada) 
  {
    byte botao = verificarBotao();

    if (botao != escolhaNenhuma && botao != escolhaEasy && botao != escolhaMid && botao != escolhaHard)
    { 
      tonar(botao, 150); 
	    delay(10);

      return botao;
    }
  }

  return escolhaNenhuma; 
}

byte verificarBotao(void)
{  
  // dificuldades
  if (digitalRead(botaoVermelho) == 1)  {Serial.println("Vermelho pressionado!"); return(escolhaVermelho);}
  else if (digitalRead(botaoVerde) == 1) { Serial.println("Verde pressionado!"); return(escolhaVerde);}
  else if (digitalRead(botaoAzul) == 1) { Serial.println("Azul pressionado!"); return(escolhaAzul);}
  else if (digitalRead(botaoAmarelo) == 1)  {Serial.println("Amarelo pressionado!"); return(escolhaAmarelo);}
  else if (digitalRead(botaoEasy) == 1) {Serial.println("Easy pressionado!");  return(escolhaEasy);}
  else if (digitalRead(botaoMid) == 1)  {Serial.println("Mid pressionado!"); return(escolhaMid);}
  else if (digitalRead(botaoHard) == 1)  {Serial.println("Hard pressionado!"); return(escolhaHard);}
  
  return(escolhaNenhuma); 
}

void tonar(byte qual, int duracaoBuzzMs)
{
  setarLEDs(qual);

  switch(qual) 
  {
  case escolhaVermelho:
    somBuzz(duracaoBuzzMs, 1136); 
    break;
  case escolhaVerde:
    somBuzz(duracaoBuzzMs, 568); 
    break;
  case escolhaAzul:
    somBuzz(duracaoBuzzMs, 851); 
    break;
  case escolhaAmarelo:
    somBuzz(duracaoBuzzMs, 638); 
    break;
  }

  setarLEDs(escolhaDesligada); 
}

void somBuzz(int duracaoBuzzMs, int delayBuzzUs)
{
  long duracaoBuzzUs = duracaoBuzzMs * (long)1000;

  while (duracaoBuzzUs > (delayBuzzUs * 2))
  {
    duracaoBuzzUs -= delayBuzzUs * 2; 

    digitalWrite(buzzer1, LOW);
    digitalWrite(buzzer2, HIGH);
    delayMicroseconds(delayBuzzUs);

    digitalWrite(buzzer1, HIGH);
    digitalWrite(buzzer2, LOW);
    delayMicroseconds(delayBuzzUs);
  }
}

void tocarVencedor()
{
  int melodia[] = {
    440, 440, 440, 349, 523, 440, 349, 523, 440,
    659, 659, 659, 698, 523, 415, 349, 523, 440
  };

  int duracoes[] = {
    500, 500, 500, 350, 150, 500, 350, 150, 650,
    500, 500, 500, 350, 150, 500, 350, 150, 650
  };

  for (int i = 0; i < 18; i++) {
    somBuzz(duracoes[i], 1000000 / melodia[i] / 2); 
    delay(50); 
  }

  setarLEDs(escolhaDesligada);
}


void somInicio(void)
{
    for (byte y = 0 ; y < 3 ; y++)
    {
      digitalWrite(buzzer2, HIGH);
      digitalWrite(buzzer1, LOW);
      delayMicroseconds(20);

      digitalWrite(buzzer2, LOW);
      digitalWrite(buzzer1, HIGH);
      delayMicroseconds(20);
    }
}

void tocarPerdedor(void)
{
  setarLEDs(escolhaVermelho | escolhaVerde);
  somBuzz(255, 1500);

  setarLEDs(escolhaAzul | escolhaAmarelo);
  somBuzz(255, 1500);

  setarLEDs(escolhaVermelho | escolhaVerde);
  somBuzz(255, 1500);

  setarLEDs(escolhaAzul | escolhaAmarelo);
  somBuzz(255, 1500);
}

byte modoAtracao() {
  const unsigned long intervalo = 100;  // tempo entre cada LED em ms
  unsigned long ultimoMillis = 0;
  byte estadoAtual = 0;

  while (true) {
    unsigned long agora = millis();

    // Se passou o tempo, muda para o próximo LED e som
    if (agora - ultimoMillis >= intervalo) {
      ultimoMillis = agora;

      switch (estadoAtual) {
        case 0:
          setarLEDs(escolhaVermelho);
          somInicio();
          break;
        case 1:
          setarLEDs(escolhaAzul);
          somInicio();
          break;
        case 2:
          setarLEDs(escolhaVerde);
          somInicio();
          break;
        case 3:
          setarLEDs(escolhaAmarelo);
          somInicio();
          break;
      }

      estadoAtual = (estadoAtual + 1) % 4; // ciclo entre 0–3
    }

    // Checa se o jogador escolheu uma dificuldade
    leitura = verificarBotao();
    if (leitura == escolhaEasy || leitura == escolhaMid || leitura == escolhaHard)
      return leitura;
  }
}
