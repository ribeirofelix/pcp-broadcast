
/* inicia: inicializa biblioteca, avisando quantos threads de cada tipo existem 
(necessário para dimensionar  estruturas de dados apropriadamente */
int inicia (int transmissores, int receptores);

void envia (int val);

int recebe (int meu_id);