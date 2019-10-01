//*************************************************************************************************/
//   Projeto    : Controle de Nivel
//   Arquivo    : Controle_de_Nivel.ino
//   Descrição  : Configurações e loop principal
//   Data       : 30/09/2019
//*************************************************************************************************/

//Parametros
#define TIMER_SEGUNDOS 5*60  //5min

//Entradas
#define pinoSensorNivel1 2
#define pinoSensorNivel2 3

//Saídas
#define pinoAlarmeLed 4
#define pinoAlarmeBux 5
#define pinoBomba1    6
#define pinoBomba2    7

void setup()
{
    // coloque aqui o seu código de configuração para ser executado uma vez:
    pinMode(pinoSensorNivel1, INPUT);
    pinMode(pinoSensorNivel2, INPUT);
    
    pinMode(pinoAlarmeLed, OUTPUT);
    pinMode(pinoAlarmeBux, OUTPUT);
    pinMode(pinoBomba1, OUTPUT);
    pinMode(pinoBomba2, OUTPUT);
}

void loop()
{
    // coloque seu código principal aqui, para executar repetidamente:
    if(nivelBaixo() && !nivelCritico())                 //Se o nível esta baixo, mas não crítico
    {
        ligaBomba();                                        //Liga bomba
        timer_seconds(0);                                  //Reinicia temporizador
        while(!timer_seconds(TIMER_SEGUNDOS))              //Enquanto temporizador está rodando
        {
            verificaNivelCritico();                             //Verifica se o nível ficou crítico
        }
        desligaBomba();                                     //Desliga a bomba ao final do temporizador
    }
    verificaNivelCritico();                             //Sempre verifica o nível crítico ao final do ciclo
}

bool nivelBaixo()
{
    return(digitalRead(pinoSensorNivel1));
}

bool nivelCritico()
{
    return(digitalRead(pinoSensorNivel2));
}

void ligaBomba()    //Liga bombas de forma alternada
{
    static bool alternar = false;
    static bool blk = false;
    if(nivelCritico() && !blk)                  //Se o nível estiver crítico e o bloqueio não estiver ativo
    {
        blk = true;                                 //Bloqueia a alternacia das bombas
    }
    digitalWrite(pinoBomba1, !alternar);        //liga ou desliga bomba1 dependendo da variável de alternancia
    digitalWrite(pinoBomba2, alternar);         //liga ou desliga bomba2 dependendo da variável de alternancia
    if(!nivelCritico() && !blk)                 //Se o nível não estiver crítico
    {
        alternar = !alternar;                       //Inverte a bomba que será ligada no próximo ciclo
    }
}

void desligaBomba()
{
    digitalWrite(pinoBomba1, LOW);
    digitalWrite(pinoBomba2, LOW);
}

void acionaAlarme()
{
    digitalWrite(pinoAlarmeLed, pulse_1s());
    digitalWrite(pinoAlarmeBux, pulse_1s());
}

void verificaNivelCritico()
{
    static bool alrm = false;
    if(nivelCritico())                              //Se o nível estiver crítico
    {
        alrm = true;                                    //Habilita alarme
        ligaBomba();                                    //Liga a bomba
        timer_seconds(0);                              //Reinicia o timer
        while(!timer_seconds(TIMER_SEGUNDOS))          //Enquanto o timer estiver rodando
        {
            acionaAlarme();                                 //Mantém o alamre acionado
        }
        desligaBomba();                                 //Desliga a bomba ao final do timer
    }
    if(alrm)                                         //Se o alarme estiver habilitado
    {
        acionaAlarme();                                 //Mantém o alarme acionado
    }
}

bool timer_seconds(int tempo)       //Temporizador de segundos
{   
    static unsigned long timer = 0;
    if(tempo == 0)
    {
        timer = millis();
    }
    if(timer + tempo * 1000 < millis())
    {
        return(true);
    }
    return(false);
}

bool pulse_1s()                     //Pulso contínuo de 1s
{
    static bool ret = true;
    static unsigned long timer = millis();
    if(timer + 500 < millis())
    {
        timer = millis();
        ret   = !ret;
    }
    return(ret);
}
