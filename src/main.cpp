/*
wersja testowa programiku do sterowania HD44780
kod częściowo realizuje przykład z datasheetu (czyli wypisuje kilka liter w pętli)
istnieje też funkcja inicjalizacji, która jest realizowana zgodnie ze schematem z datasheetu
timingi są na razie od czapy

brak glitchy an ekranie

serial monitor nie działą jak jak powinien - np. wyświetla dwie pierwsze litery printa, po chwili restzę i początek kolejnego printa....

*/

#define F_CPU 16000000UL    // dla 16MHz

#include <util/delay.h>     // dodajemy bibliotekę do opóźnień

// te dwie linie powyżej są w zasadzie bez sensu, ale chciałem aby kod był częściowo kompatybilny poza frameworkiem arduino... w każdym razie, niczego nie psują

#include <Arduino.h>

#define RS0 2
#define E 4
#define DB7 8
#define DB6 7
#define DB5 6
#define DB4 5

void enable(void) { // prosta funkcja wyzwalająca Enable na 1us
    _delay_us(1);
    digitalWrite(E, HIGH);
    _delay_us(1);
    digitalWrite(E, LOW);
}

void send(int RS, int RW, int B7, int B6, int B5, int B4) { // prosta funkcja do wysyłania pojedyńczego sygnału. RW puste, podciągnięte do GND - na razie działą tylkow  trybie write
    _delay_us(45); // ten delay ma na celu upewnienie się, że poprzednia ionstrukcja została już wykonana (przeciętnie trwa to do 40 us)
    // ustawia stan pinów przed wysłaniem instrukcji
    digitalWrite(RS0, RS);
    digitalWrite(DB7, B7);
    digitalWrite(DB6, B6);
    digitalWrite(DB5, B5);
    digitalWrite(DB4, B4);
    // enable triggeruje ekran do odczytania instrukcji
    enable();
}



void initiate(void) { // funkcja inicjująca zgodnie z przykładem ze strony 46 datasheeta
    int x = 100; // śmieszna wartość opóźnienia, której używałem do debugowania. na razie zostawiam, mimo że program powinien radzić sobie bez tego delay

    _delay_ms(15);
    send(0, 0, 0, 0, 1, 1); // krok 1/3
    
    _delay_ms(5);
    send(0, 0, 0, 0, 1, 1); // krok 2/3
    
    _delay_ms(1);
    send(0, 0, 0, 0, 1, 1); // krok 3/3
    
    _delay_ms(x);
    send(0, 0, 0, 0, 1, 0); // krok 4 - fucntion set - na razie ustawia tylko tryb 4 bit, z uwagi na to zę piny DB0-3 są niepodpięte, czyli w stanie 0
    
    _delay_ms(x);
    send(0, 0, 0, 0, 1, 0); // krok 5 1/2 - function set 1 linia normalny font - czyli nadpisanie poprzedniej instrukcji, tym razem już w trybie 4bitowym, czyli wysyłając bit "na dwa razy"
    _delay_ms(x);
    send(0, 0, 0, 0, 0, 0); // krok 5 2/2
    
    _delay_ms(x);
    send(0, 0, 0, 0, 0, 0); // krok 6 1/2 - display off
    _delay_ms(x);
    send(0, 0, 1, 0, 0, 0); // krok 6 2/2
    
    _delay_ms(x);
    send(0, 0, 0, 0, 0, 0); // krok 7 1/2 - display clear
    _delay_ms(x);
    send(0, 0, 0, 0, 0, 1); // krok 7 2/2
    
    _delay_ms(x);
    send(0, 0, 0, 0, 0, 0); // krok 8 1/2 - entry mode set (increment i s = 0)
    _delay_ms(x);
    send(0, 0, 0, 1, 1, 1); // krok 8 2/2

}

int main(void) {

    pinMode(RS0, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(DB7, OUTPUT);
    pinMode(DB6, OUTPUT);
    pinMode(DB5, OUTPUT);
    pinMode(DB4, OUTPUT);

    Serial.begin(9600);
    Serial.println("test start"); // serial monitor z jakiegoś powodu nie działa nma tym etapie tak jak powinien

    // _delay_ms(500); // oczekiwanie na "stabilizacje" LCD - totalnie zbędne
    
    initiate(); // ręczna inicjacja. ekran powinien zainicjować się samoczynnie przez Internal Reset circuit, gdy ma zapewnione "stabilne" zasilanie
    /* i tak też się dzieję, ale chciałęm mieć mimo wszystko swoją funckję inicjacji*/

    // i dalej realizuje częściwoo przykład ze strony 42

    // send(0,0,0,0,1,0); // 2 fucntion set - ten krok należy pominąć, jeśli wykonuje się ręczną inicjacje
    /* wynika to z faktu, że sekwencja ręcznej inicjacji ustawia już tryb 4bitowy*/

    send(0,0,0,0,1,0); // 3 function set  1/2
    send(0,0,0,0,0,0); // 3 function set 3 2/2

    send(0,0,0,0,0,0); // 4 display on 1/2
    send(0,0,1,1,1,0); // 4 display on 2/2

    send(0,0,0,0,0,0); // 5 enter set mode 1/2
    send(0,0,0,1,1,0); // 5 enter set mode 1/2

    
    while(1) { // pętla wypisująca HITACHI. na koniec clear i od nowa
        

        _delay_ms(10);
        send(1,0,0,1,0,0);
        send(1,0,1,0,0,0);
        _delay_ms(100);
        send(1,0,0,1,0,0);
        send(1,0,1,0,0,1);
        _delay_ms(100);
        send(1,0,0,1,0,1);
        send(1,0,0,1,0,0);
        _delay_ms(100);
        send(1,0,0,1,0,0);
        send(1,0,0,0,0,1);
        _delay_ms(100);
        send(1,0,0,1,0,0);
        send(1,0,0,0,1,1);
        _delay_ms(100);
        send(1,0,0,1,0,0);
        send(1,0,1,0,0,0);
        _delay_ms(100);
        send(1,0,0,1,0,0);
        send(1,0,1,0,0,1);
        _delay_ms(100);

        // clear display
        send(0,0,0,0,0,0);
        send(0,0,0,0,0,1);
        

    }
    return 0;

    

}



