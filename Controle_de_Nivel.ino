//*************************************************************************************************/
//   Projeto    : Controle de Nivel
//   Arquivo    : Controle_de_Nivel.ino
//   Descricao  : Configuracoes e loop principal
//   Data       : 30/09/2019
//*************************************************************************************************/

//Parametros
#define TIMER_SEGUNDOS 300  //300 segundos = 5min

//Entradas
#define pinoSensorNivel1 2
#define pinoSensorNivel2 3

//Saidas
#define pinoAlarmeLed 4
#define pinoAlarmeBux 5
#define pinoBomba1    6
#define pinoBomba2    7

void setup()
{
    // coloque aqui o seu codigo de configuracao para ser executado uma vez:
    Serial.begin(9600);
    pinMode(pinoSensorNivel1, INPUT);
    pinMode(pinoSensorNivel2, INPUT);
    
    pinMode(pinoAlarmeLed, OUTPUT);
    pinMode(pinoAlarmeBux, OUTPUT);
    pinMode(pinoBomba1, OUTPUT);
    pinMode(pinoBomba2, OUTPUT);
}

void loop()
{
    // coloque seu codigo principal aqui, para executar repetidamente:
    if(nivelBaixo() && !nivelCritico())                 //Se o nivel esta baixo, mas n�o cr�tico
    {
        ligaBomba();                                        //Liga bomba
        timer_seconds(0);                                  //Reinicia temporizador
        while(!timer_seconds(TIMER_SEGUNDOS))              //Enquanto temporizador esta rodando
        {
            verificaNivelCritico();                             //Verifica se o nivel ficou cr�tico
        }
        desligaBomba();                                     //Desliga a bomba ao final do temporizador
    }
    verificaNivelCritico();                             //Sempre verifica o nivel critico ao final do ciclo
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
    if(nivelCritico() && !blk)                  //Se o nivel estiver critico e o bloqueio nao estiver ativo
    {
        blk = true;                                 //Bloqueia a alternacia das bombas
    }
    digitalWrite(pinoBomba1, !alternar);        //liga ou desliga bomba1 dependendo da variavel de alternancia
    digitalWrite(pinoBomba2, alternar);         //liga ou desliga bomba2 dependendo da variavel de alternancia
    Serial.print("Bomba " );
    Serial.print(alternar + 1);
    Serial.println(" acionada");
    if(!nivelCritico() && !blk)                 //Se o nivel nao estiver critico
    {
        alternar = !alternar;                       //Inverte a bomba que sera ligada no proximo ciclo
    }
}

void desligaBomba()
{
    digitalWrite(pinoBomba1, LOW);
    digitalWrite(pinoBomba2, LOW);
    Serial.println("Bombas Desligadas");
}

void acionaAlarme()
{
    digitalWrite(pinoAlarmeLed, pulse_1s());
    digitalWrite(pinoAlarmeBux, pulse_1s());
}

void verificaNivelCritico()
{
    static bool alrm = false;
    if(nivelCritico())                              //Se o nivel estiver critico
    {
        alrm = true;                                    //Habilita alarme
        ligaBomba();                                    //Liga a bomba
        timer_seconds(0);                              //Reinicia o timer
        while(!timer_seconds(TIMER_SEGUNDOS))          //Enquanto o timer estiver rodando
        {
            acionaAlarme();                                 //Mantem o alamre acionado
        }
        desligaBomba();                                 //Desliga a bomba ao final do timer
    }
    if(alrm)                                         //Se o alarme estiver habilitado
    {
        acionaAlarme();                                 //Mantem o alarme acionado
    }
}

bool timer_seconds(unsigned long tempo)       //Temporizador de segundos
{   
    static unsigned long timer = 0;
    int timeMonitor = ((timer + tempo * 1000) - millis())/1000;
    static int previousTime = 0;
    if(tempo == 0)
    {
        timer = millis();
        return(false);
    }
    if(timeMonitor != previousTime)
    {
        Serial.println(timeMonitor);
        previousTime = timeMonitor;
    }
    if(timer + tempo * 1000 < millis())
    {
        return(true);
    }
    return(false);
}

bool pulse_1s()                     //Pulso continuo de 1s
{
    static bool ret = true;
    static unsigned long timer = millis();
    if(timer + 500 < millis())
    {   if(ret) Serial.println("ALARME");
        timer = millis();
        ret   = !ret;
    }
    return(ret);
}
