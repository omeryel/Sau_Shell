#ifndef my_h
#define my_h



//Maksimum girdi miktari
#define BUFFERSIZE 256
//Yeni istek bekleme belirteci
#define ISTEK "sauShell >> "
//Belirtecin boyutu
#define ISTEKBOYUTU sizeof(ISTEK)
//Programdaki hatalar icin genel tanımlama
#define HATA -1


void DosyaYonetimi(char **argv,char *kaynak,char *hedef,int secenek);
void PipeYonetimi(char **argv);
int KomutYoneticisi(char *argv[]);
void KomutBekle();

#endif