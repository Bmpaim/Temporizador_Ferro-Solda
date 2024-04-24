#define RELE A0    //chavear tomada para o ferro de solda
#define BUZZER A1  //alerta
#define BT1 A2     //liga timer
#define BT2 A3     //para timer
#define LED1 A4    //led verde
#define LED2 A5    //led vermelho
#define POT A6     //configurar potencia
#include <LiquidCrystal.h>

long duracao = 0;      //tempo ligado
bool ativo = true;     //estado temporizador
long intervalo;        //diminuir 2 segundo
int bip1, bip2, bip3;  //alertas buzzer
int pot_div=100;           //divisao da potencia
long temp_F_inicial;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(POT, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELE, OUTPUT);

  digitalWrite(RELE, LOW);   //rele iniciar desligado
  digitalWrite(LED2, HIGH);  //led vermelho iniciar desligado
}

void ferro(pot_div){
  long tempo_ferro = (10000 * pot_div)/100; // 10 seg vezes porcentagem/100 = tempo em milisegundos
  long temp_F_atual = millis();
  
  if (temp_F_atual - temp_F_inicial <= tempo_ferro) {
    digitalWrite(RELE, HIGH);  
  } else {
    digitalWrite(RELE, LOW);    
  }
 if (temp_F_atual - temp_F_inicial >=  tempo_ferro) {
    temp_F_inicial  = temp_F_atual;  
  }
}

void loop() {

  //potenciometro para controle da potencia
  float pot = analogRead(POT);
  float pot_true = (5.0 * pot) / 1023.0;
  /*4 divisoes para potencia do rele
  1-25%    2-50%   3-75%     4-100%
  */
  if (pot_true > 0 && pot_true <= 1.25) {  //div1 potencia 25%
    pot_div = 25;
  }
  if (pot_true > 1.25 && pot_true <= 2.5) { //div2 potencia 50%
    pot_div = 50;
  }
  if (pot_true > 2.5 && pot_true <= 3.75) { //div3 potencia 75%
    pot_div = 75;
  }
  if (pot_true > 3.75 && pot_true <= 5) { //div4 potencia 100%
    pot_div = 100;
  }

  if (ativo) {  //função temporizador
    long tempo_restante = duracao;
    int press;

    if (millis() - intervalo > 1000) {  //diminuir 1 segundo
      Serial.println("teste");
      duracao = duracao - 1000;
      intervalo = millis();
    }
    if (digitalRead(BT1) == HIGH) {  //trava botao
      press = 0;
    }
    if (digitalRead(BT1) == LOW && press == 0) {
      press = 1;
      delay(100);
      if (duracao + 60000 > 300000) {  //limite de 5 min
        duracao = 300000;
      } else {
        duracao = duracao + 60000;  //incrementar duração (1 min=60000ms)
      }
    }
    if (tempo_restante <= 0) {  //tempo acabou
      analogWrite(RELE, 0);     //desligar rele
      lcd.clear();
      lcd.print("Tempo esgotado");
      delay(50);
    } else if (tempo_restante > 0) {
      TELA(tempo_restante, 100);  //enviar tempo restante e potencia do ferro
    }
  }
}

void TELA(long tempo_restante, int potencia) {  //função para atualizar lcd
  lcd.clear();
  lcd.print("Tempo restante: ");
  lcd.setCursor(0, 1);               //linha de baixo
  lcd.print(tempo_restante / 1000);  //millisegundos dividos por mil = segundos
  lcd.print(" seg  ");
  lcd.print("P:");
  lcd.print(potencia);  //potencia ferro de solda
  lcd.print("%");
  delay(50);
}
