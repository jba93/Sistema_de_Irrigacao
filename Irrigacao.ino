//Sistema de irrigação usando Arduino
//Disciplina de Microprocessadores e Microcontroladores
//Prof. Dr. Aledir S. Pereira
//Juliana B. Arvani e Matheus A. dos Santos Guissi 

int seg = 50, min = 0; //Variavéis para controle do tempo
int trocou = 1;  //Variável para controle da impressão do tempo
int inicio = 0;

int Sensor = A0; //Pino analógico 0

int Botao = 2; //Pino digital 2
int estadoBotao;

int val; //Armazena a umidade momentânea

int num; //Variável que armazena a quantidade de irrigações que serão feitas

int hora[3]; //Armazena o horário das irrigações
 
int Valvula = 6; //Pino da Valvula

int i;
 
void setup()
{
  pinMode(Valvula, OUTPUT); //Pino 6 da válvula é saída

  pinMode(Botao, INPUT); //Pino 2 do botão é entrada
  
  Serial.begin(9600);
}
 
void loop()
{
  seg = 0;
  min = 0;
  num = 0;
  
  Serial.println("**************** Sistema de Irrigacao ****************");
  Serial.println("O sistema e capaz de realizar ate 3 irrigacoes por dia.");
  Serial.println("Quantas irrigacoes serao realizadas?");
  while(true)
  {
    if(Serial.available() > 0)
    {
      num = Serial.parseInt();
      break;
    }
  }
  
  //detecção de erro de digitação
  while(num < 1 || num > 3)
  {
      Serial.println("\n***********************ATENCAO!***********************");
      Serial.println("Quantidade de irrigacoes invalida. O numero de irrigacoes deve estar entre 1 e 3.\n");
      Serial.println("Quantas irrigacoes serao realizadas?");
      while(true)
      {
          if(Serial.available() > 0)
          {
              num = Serial.parseInt();
              break;
          }
      }
   }
      
  for(i = 0; i < num; i++)
  {
    Serial.println("Qual o horario desejado?");
    while(true)
    {
      if(Serial.available() > 0)
      {
        hora[i] = Serial.parseInt();
        break;
      }
    }
      //detecção de erro de didgitação
      while(hora[i] < 0 || hora[i] > 23)
      {
          Serial.println("\n***********************ATENCAO!***********************");
          Serial.println("Horario invalido. O horario deve ser um valor inteiro entre 0 e 23.\n");
          Serial.println("Qual o horario desejado?");
          while(true)
          {
              if(Serial.available() > 0)
              {
                  hora[i] = Serial.parseInt();
                  break;
              }
          }
      }
  }

//configurar hora atual
Serial.println("\nHora atual: ");   
    while(min==0)   
        if (Serial.available() > 0) //verifica se foi digitado um valor          
            min = Serial.parseInt(); //o valor inteiro é colocado na variável hora

  //detecção de erro de didgitação
  while(min < 0 || min > 23)
  {
      Serial.println("\n***********************ATENCAO!***********************");
      Serial.println("Hora invalida. A hora deve ser um valor inteiro entre 0 e 23.\n");
      Serial.println("Hora atual: ");
      while(true)
      {
        if(Serial.available() > 0)
       {
          min = Serial.parseInt();
          break;
       }
      }
  }
  
//configurar minuto atual
  Serial.println("Minuto atual: ");  
     while(seg==0)   
        if (Serial.available() > 0) //verifica se foi digitado um valor          
            seg = Serial.parseInt(); //o valor inteiro é colocado na variável minuto

  //detecção de erro de didgitação
  while(seg < 0 || seg > 59)
  {
      Serial.println("\n***********************ATENCAO!***********************");
      Serial.println("Minuto invalido. O minuto deve ser um valor inteiro entre 0 e 59.\n");
      Serial.println("Minuto atual: ");
      while(true)
      {
        if(Serial.available() > 0)
       {
          seg = Serial.parseInt();
          break;
       }
      }
  }

  //exibe as informações digitadas
  Serial.print("\nTotal de irrigacao(oes) por dia: ");
  Serial.println(num);

  Serial.println("Horario(s) de irrigacao:");
  for (i=0; i<num; i++)
  {
    Serial.print(hora[i]);
    Serial.println("H");
  }
  
  Serial.print("\n\nInicializando sistema as ");
  Serial.print(min);
  Serial.print("h ");
  Serial.print(seg);
  Serial.println("m");
  Serial.println("Pressione o botao para abortar o sistema.\n");
  Irrigacao(num, hora);
  Serial.println("Sistema abortado!\n\n\n");
}

//Sistema de Irrigação
void Irrigacao(int numero, int horas[])
{ 
  estadoBotao = digitalRead(Botao);
  while(estadoBotao == LOW)
  {
    estadoBotao = digitalRead(Botao);

    Relogio();
    
    for(i = 0; i < num; i++)
    { 
      //Irriga a "plantação" 3 vezes ao dia
      if(min == hora[i] && seg == 0 && trocou == 1)
      {
        analisarSolo();
      }
    }
  }
}

//Função p/ analisar o solo e abrir a valvula se necessário
void analisarSolo()
{  
  val = analogRead(Sensor);
  
  if(val > 900)
  {
    Serial.println("\nO solo esta seco!");
    Serial.println("A bomba sera ligada por 6 minutos.\n");
    ligarValvula(6);
  }
  if((val >= 300) && (val <= 900))
  {
    Serial.println("\nO solo esta um pouco umido!");
    Serial.println("A bomba sera ligada por 3 minutos.\n");
    ligarValvula(3);
  }
  if(val < 300)
  {
      Serial.println("\nO solo esta umido!");
      Serial.println("A bomba nao sera ligada.\n");
  }
}

//Função p/ abrir a valvula por determinado tempo
void ligarValvula(int tempo)
{
  digitalWrite(Valvula, HIGH);
  inicio = seg;
  while(inicio + tempo != seg)
    Relogio();
  digitalWrite(Valvula, LOW);
  Serial.println("\nA bomba foi desligada!\n");
}

//Função Relógio
void Relogio()
{
          
    static unsigned long ult_tempo = 0;
    int tempo = millis();
    if(tempo - ult_tempo >= 1000)
    {
      ult_tempo = tempo;
      seg++;
      trocou = 1;
    }
    else
      trocou = 0;
    if(seg >= 60)
    {
      seg = 0;
      min++;
    }
    if(min >= 24)
    {
      min = 0;
    }
    if(trocou)
    {
      Serial.print("Hora atual: ");
      if(min < 10)
        Serial.print("0");
      Serial.print(min);
      Serial.print(":");
      if(seg < 10)
        Serial.print("0");
      Serial.println(seg);
    }
}

