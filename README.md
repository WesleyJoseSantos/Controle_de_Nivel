# Controle_de_Nivel
Software para controle de nível baseado na plataforma Arduino

1 Sensor de nível baixo: aciona uma das bombas. A cada acionamento alterna a bomba a ser ligada.
1 Sensor de nível crítico: aciona a bomba que estiver parada e bloqueia a bomba que estava rodando. Aciona o alarme.
2 Bombas: controle do nível do tanque
1 led e 1 buzzer para alarme: acionam para sinalizar anomalia no sistema

Funcionamento:

O sensor 1 aciona as bombas de forma alternada, mantendo uma delas ligada pelo tempo especificado em TIMER_SEGUNDOS (5min). A cada acionamento do sensor 1, a bomba a ser ligada é alterada, desta forma nenhuma das bombas será mais utilizada que a outra, distribuindo assim o seu uso, e maximizando a vida útil de ambas.

O sensor 2 é um sensor de emergência, ao ser acionado irá imediatamente inverter as bombas, desligando a que estiver em funcionamento, e ligando a que estiver desligada. Além disso será acionado o alarme sinalizando que há uma anomalia no sistema. A bomba que estava antes em funcionamento será bloqueada, e o sistema não irá mais acionar ela, utilizando apenas a bomba na qual não houve anomalia.
