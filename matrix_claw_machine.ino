
#include <LedControl.h>
// Pin Definitions
#define LEDMATRIX_PIN_DIN 49
#define LEDMATRIX_PIN_CS  51
#define LEDMATRIX_PIN_CLK 53


//Matriz con controlador
LedControl lc = LedControl(LEDMATRIX_PIN_DIN, LEDMATRIX_PIN_CLK, LEDMATRIX_PIN_CS, 1);
//Tabla de movimientos se inicia por defecto en 7,7
int table[8][8];

void restart()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
          table[i][j] = 0;
        }
    }   
    table[7][0] = 2;  //se inicia sobre el tobogan
}

void move(int Direction)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            //se encuentra la posicion del puntero
            if(table[i][j] == 2)
            {
                //se mueve el puntero
                switch(Direction)
                {
                    // 1 = DERECHA , 2 = IZQUIERDA, 3 = ARRIBA, 4 = ABAJO
                    case 1:
                    if((j + 1) > 7) return;
                    table[i][j + 1] = 2;
                    break;
                    case 2:
                    if((j - 1) < 0) return;
                    table[i][j - 1] = 2;
                    break;
                    if((i - 1) < 0) return;
                    case 3:
                    table[i - 1][j] = 2;
                    break;
                    case 4:
                    if((i + 1) > 7) return;
                    table[i + 1][j] = 2;
                    break;
                }
                table[i][j] = 1;
                return;
            }
        }
    }   
}

void btn_pressed()
{
    if(Serial.available())
    {
        switch (Serial.read())
        {
        case 'a':
            move(2);
            break;
        case 'd':
            move(1);
            break;
        case 'w':
            move(3);
            break;
        case 's':
            move(4);
            break;
        default:
            break;
        }
    }
}

void show()
{
    //muestra la tabla en la matriz de led
    int to_show[8];
    for(int i = 0; i < 8; i++)
    {
        double byte_str = 0, byte_str_cnt = 0;
        for(int j = 0; j < 8; j++)
        {
            if(table[i][j] == 2)
            {
              
                byte_str +=  pow(2,(7 - j));
                byte_str_cnt += 0;
            }
            else
            {
                byte_str += table[i][j] * pow(2,(7 - j));
                byte_str_cnt += table[i][j] * pow(2,(7 - j));
            }
        }
        //mostrando la primer matriz
        int pin = (int)(byte_str + 0.1);
        lc.setRow(0, i,pin);
        //guardando el contraste
        to_show[i] = (int)(byte_str_cnt + 0.1);  
    }
    //mostrando el contraste para parpadear
    delay(500);
    for(int k = 0; k < 8; k++)
    {
        lc.setRow(0, k, to_show[k]);
    }
    delay(500);
}

void setup() 
{
  lc.shutdown(0, false);     // enciende la matriz
  lc.setIntensity(0, 1);    // establece brillo
  lc.clearDisplay(0);     // blanquea matriz
  table[7][0] = 2;  //se inicia sobre el tobogan
  Serial.begin(9600);
}

void loop()
{
  btn_pressed();
  show();
}