#define RELE A0    //chavear tomada para o ferro de solda
#define BUZZER A1  //alerta
#define BT1 A2     //liga timer
#define BT2 A3     //para timer
#define LED1 A4    //led verde
#define LED2 A5    //led vermelho
#define POT A6     //configurar potencia
#include <LiquidCrystal.h>

long duracao = 0;      //tempo ligado
bool ativo = false;    //estado temporizador
long intervalo;        //diminuir 2 segundo
int bip1, bip2, bip3;  //alertas buzzer

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

void loop() {

  //potenciometro para controle da potencia
  float pot = analogRead(POT);
  float pot_true = (5.0 * pot) / 1023.0;

  if (digitalRead(BT1) == LOW && !ativo) {  //ligar timer
    bip1 = 0;                               //alerta 15s
    bip2 = 0;                               //alerta 5s
    bip3 = 0;                               //alerta 0s
    comecar_tempo();
  }

  if (digitalRead(BT2) == LOW && ativo) {  //encerrar timer
    parar_tempo();
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
    if (tempo_restante <= 15000 && bip1 == 0) {  //alerta 15 segundos
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      bip1 = 1;
    }
    if (tempo_restante <= 5000 && bip2 == 0) {  //alerta 5 segundos
      digitalWrite(BUZZER, HIGH);
      delay(500);
      digitalWrite(BUZZER, LOW);
      bip2 = 1;
    }
    if (tempo_restante <= 1000 && bip3 == 0) {  //alerta 0 segundos
      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);
      bip3 == 1;
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
void comecar_tempo() {       //setup timer
  digitalWrite(RELE, HIGH);  //ligar rele(220VAC)
  digitalWrite(LED1, HIGH);  //led verde ligado
  digitalWrite(LED2, LOW);   //led vermelho desligado
  duracao = 60000;           //duração inicial
  ativo = true;              //ativar temporizador
}
void parar_tempo() {          //desligar
  digitalWrite(BUZZER, LOW);  //desligar buzzer
  digitalWrite(RELE, LOW);    //desligar rele(30VAC)
  digitalWrite(LED1, LOW);    //led verde desligado

  digitalWrite(LED2, HIGH);   //led vermelho ligado
  lcd.clear();
  lcd.print("Desligado ");
  ativo = false;
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
