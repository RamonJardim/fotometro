#include <LiquidCrystal.h>

/* Portas do arduino para leitura da tensão, do resistor e do bbotao */
#define leitura A0
#define resistorProva 10000
#define botao 8

/* Portas do display LCD */
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/* Variaveis para mudanca de estado do LCD */
int estadoBotao = LOW;
int muda = 1; // 0 para resistencia e 1 para tensao

/* Variaveis para medicao da tensao e da resistencia */
float tensao;
float resistorX;

/* Variaveis para detectar aperto longo do botão */
int vezesPressionado = 0;
unsigned long tempo = 0;

/* Tensao do branco para calibracao */
float tensaoBranco = 0;
float tensaoCorrigida;

/* Funcao para ler a tensao */
float getTensao() {
  return analogRead(leitura)*5/1023.0;
}

/* Funcao para mudar o estado do LCD (Tensao / Resistencia) */
void inverte(int* n) {
  if(*n == 0) {
    *n = 1;
  } else {
    *n = 0;
  }
}

/* Funcao executada na calibracao */
void calibracao() {
  tensaoBranco = getTensao();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibracao");
  lcd.setCursor(0,2);
  lcd.print("       realizada");
  delay(1500);
  lcd.clear();
}

void setup() {
  Serial.begin(9600);
  pinMode(leitura, INPUT);
  pinMode(botao, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  estadoBotao = digitalRead(botao);
  if(estadoBotao == HIGH) {
    
    tempo = millis();
    vezesPressionado++;
    
    while(digitalRead(botao) == HIGH && millis()-tempo < 2000);
    tempo = millis() - tempo;

    if(tempo < 2000) {
      inverte(&muda);
      lcd.clear();
    } else {
      calibracao();
    }
    vezesPressionado = 0;
    
  } else {
      tensao = getTensao();
      tensaoCorrigida = getTensao() - tensaoBranco;
      resistorX = (5.0*resistorProva)/tensao - resistorProva;
      
      Serial.print("Tensao medida: ");
      Serial.println(tensao);
      Serial.print("Tensao branco: ");
      Serial.println(tensaoBranco);
      Serial.print("Tensao corrigida: ");
      Serial.println(tensaoCorrigida);
      Serial.print("Resistencia do LDR =  ");
      Serial.print(resistorX);
      Serial.println(" Ohms");
      Serial.println();
      
    if(muda == 0) {  
      lcd.setCursor(0, 0);
      lcd.print("Resistencia LDR: ");
      lcd.setCursor(0, 1);
      lcd.print("          ");
      lcd.write(B11110100);
      lcd.setCursor(0, 1);
      lcd.print(resistorX);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Tensao: ");
      lcd.setCursor(0, 1);
      lcd.print("     V");
      lcd.setCursor(0,1);
      lcd.print(tensaoCorrigida);
    }
  }
  delay(200);
}
