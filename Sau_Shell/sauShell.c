
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "sauShell.h"



//Cocuk proses degiskeni
pid_t pid;




//İstek bekleme belirtecini yazdırır
void KomutBekle()
{
    printf("%s", ISTEK);
}

//Dosya yazma okuma işlemlerini yapar
void DosyaYonetimi(char **argv, char *kaynak, char *hedef, int secenek)
{
    int fd; //Dosya tanimlayici
    if((pid == fork()) == HATA) {
        perror("Hata: Cocuk proses olusturulamadi.\n");
        return;
    }

    if(pid == 0){
        //Dosya cıktı yonlendirmesi
        if(secenek == 0){
            //Dosya sadece kullanıcıya yazma okuma izinleriyle olusturur yazma modunda acılır .
            fd = open(hedef, O_CREAT | O_TRUNC | O_WRONLY, 0600);
            //STDOUT dosya ile degistiriliyor
            dup2(fd, STDOUT_FILENO);
            //Dosya kapatiliyor
            close(fd);
        }
        //Dosya girdi ve cıktı yonlendirmesi
        if(secenek == 1){
            //Dosya acılır yanlız oku modunda
            fd = open(kaynak, O_RDONLY, 0600);
            //STDIN dosya ile degistirir
            dup2(fd, STDIN_FILENO);
            close(fd);
            //Dosya olusturulur ve yazma modunda
            fd = open(hedef, O_CREAT | O_TRUNC | O_WRONLY, 0600);
            //Dosya STDOUT ile degistirilir
            dup2(fd, STDOUT_FILENO);
            //Dosya kapatılır
            close(fd);
        }
            //arguman kosturulamaz ise hata mesajı basılır
        if (execvp(argv[0], argv) == HATA) {
            perror("Hata:Komut bulunamadı.\n");
            //İsleme sonlandırma sinyalı verilir
            kill(getpid(), SIGTERM);
        }
    }
    //Ebeveyn proses cocuk prosesi bekler
    waitpid(pid, NULL, 0);
}

//Pipe(Boru) islemi icin fonksiyon
void pipeYonetimi(char **argv)
{
    int fd1[2], fd2[2], argumanSayisi = 0, girdi0 = 0, girdi1 = 0, girdi2 = 0, SonKomut;
    char *Komutlar[BUFFERSIZE];

    //Pipe("|") sayısını hesaplar 
    for(int i = 0; argv[i] != NULL; i++){
        //"|" kadar arttırma yapar.
        if(strcmp(argv[i], "|") == 0){
            argumanSayisi++;
        }
    }
    argumanSayisi++;

    //Pipe Dongusu
    while(argv[girdi0] != NULL && SonKomut != 1) {
        girdi1 = 0;
        //"|" a kadar olan girdileri dizide tutuyor
        while (strcmp(argv[girdi0], "|") != 0) {
            Komutlar[girdi1] = argv[girdi0];
            girdi0++;
            if (argv[girdi0] == NULL) {
                SonKomut = 1;
                girdi1++;
                break;
            }
            girdi1++;
        }
        //Dizinin son elemanına NULL degeri atıyor
        Komutlar[girdi1] = NULL;   
        girdi0++;
        //İki komut boruya baglanıyor
        if (girdi2 % 2 == 0) {
            pipe(fd2);
        } else {
            pipe(fd1);
        }

        //Cocuk proses olusturuluyor
        pid = fork();
        //Hata olusursa dosyalar kapatalıyor
        if (pid == HATA) {
            if (girdi2 != argumanSayisi - 1) {
                if (girdi2 % 2 == 0) {
                    close(fd2[1]);
                } else {
                    close(fd1[1]);
                }
            }
            perror("Hata: Cocuk proses olusturulamadi.\n");
            return;
        }
        //Cocuk proses basarılı ise pipe islemine geciliyor
        
        if (pid == 0) {
            //STDOUT komut  ile degistiriliyor."|" isaretinin solundaki
            //komut pipe yazma moduna ayarlanıyor.
            if (girdi2 == 0) {
                close(fd2[0]);
                dup2(fd2[1], STDOUT_FILENO);
            }
            //STDIN herbir pipe icin degistiriliyor
            else if (girdi2 == argumanSayisi - 1) {
                if (argumanSayisi % 2 == 0) {
                    close(fd2[1]);
                    dup2(fd2[0], STDIN_FILENO);
                } else {
                    close(fd1[1]);
                    dup2(fd1[0], STDIN_FILENO);
                }
                // 3 argumanlı pipe 
            } else {
                if (girdi2 % 2 == 0) {
                    close(fd1[1]);
                    close(fd2[0]);
                    dup2(fd1[0], STDIN_FILENO);
                    dup2(fd2[1], STDOUT_FILENO);
                } else {
                    close(fd2[1]);
                    close(fd1[0]);
                    dup2(fd2[0], STDIN_FILENO);
                    dup2(fd1[1], STDOUT_FILENO);
                }
            }

            if (execvp(Komutlar[0], Komutlar) == HATA) {
                perror("Hata: Komut bilinmiyor.\n");
                //Sonlandırma sinyali veriliyor
                kill(getpid(), SIGTERM);
            }
        }
        //Dosya tanımlayıcıları kapatılıyor
        if (girdi2 == argumanSayisi - 1) {
            if (argumanSayisi % 2 == 0) {
                close(fd2[0]);
            } else {
                close(fd1[0]);
            }
        } else if (girdi2 == 0) {
            close(fd2[1]);
        } else {
            if (girdi2 % 2 == 0) {
            close(fd1[0]);
            close(fd2[1]);
            } else {
                close(fd2[0]);
                close(fd1[0]);
            }
        }
        //Cocuk prosesin sonlanması bekleniyor ebeveyn proses tarafından 
        waitpid(pid, NULL, 0);
        girdi2++;
    }
}

//Kullanıcı girdilerini yonetir
int KomutYoneticisi(char *argv[])
{
    int i = 0, j = 0, arkaplanAktif = 0, cgirdi0, cgirdi1, cgirdi2;
    char *argumanlar[BUFFERSIZE-1];

    //"<", ">" ,"&" isretlerini bulana kadar argumanları argumanlar dizisine atar
    while(argv[j] != NULL){
        if((strcmp(argv[j], "<")) == 0 || (strcmp(argv[j], ">")) == 0 ||  (strcmp(argv[j], "&") == 0 ))
            break;
        argumanlar[j] = argv[j];
        j++;
    }
    //Varsayılan olarak verilen komutlar
    //Ekranı temizler
    if(strcmp(argv[0], "sil") == 0) {
        system("clear");
    //programı kapatır
    }else if(strcmp(argv[0], "cik") == 0) {
        exit(0);
    //cd(change directory) komutunu calıstırır."cd"den sonra girdi yoksa varsayılan
    // olarak "~" konumuna gonderiyor
    }else if(strcmp(argv[0], "yardim") == 0) {
        printf("Isletım Sistemleri Odevi\n Varsayılan komutlar :\n\t yardim\n\t cd\n\t cik\n\t sil\n\nDiger komutlar hakkında bilgi icin 'man (komut)'.\n");
    
    }else if(strcmp(argv[0], "cd") == 0) {        
        if (argv[1] == NULL) {
        chdir(getenv("HOME"));
        }else{
            chdir(argv[1]);
        }
    }else{
        while(argv[i] != NULL && arkaplanAktif == 0){
            //"|" isreti varsa komutlarda pipeYonetimi methoduna yonlendiriliyor
            if(strcmp(argv[i], "|") == 0) {
                pipeYonetimi(argv);
                return 1;

                //"&" isareti varsa arkaplanı true yapıyor ve 
            }else if (strcmp(argv[i], "&") == 0) {
                arkaplanAktif = 1;
            //Dosya giris cıkıs işlemleri 
            }else if (strcmp(argv[i], "<") == 0){
                cgirdi0 = i+1;
                cgirdi1 = i+2;
                cgirdi2 = i+3;
                //"<" isaretinden sonra girdi yoksa hata verir
                if(argv[cgirdi0] == NULL || argv[cgirdi1] == NULL || argv[cgirdi2] == NULL){
                    perror("Hata: Girdi eksik.\n");
                    return -1;
                }else{                    
                    if(strcmp(argv[cgirdi1], ">") != 0) {
                        perror("Hata: Eksik isaret '>' !\n");
                        return -1;
                    }
                }
                //argumanlar methoda gonderiliyor.Orada dosyaya yazma okuma islemi yapılacak
                DosyaYonetimi(argumanlar, argv[i+1], argv[i+3], 1);
                return 1;
                //Dosya cıktısı
            }else if(strcmp(argv[i], ">") == 0){
                if(argv[i+1] == NULL){
                    perror("Hata: Eksik girdi.\n");
                    return -1;
                }
                //argumanlar yazma islemi icin methoda veriliyor.Okuma islemi olmadıgı icin Null degeri atandi.
                DosyaYonetimi(argumanlar, NULL, argv[i+1], 0);
                return 1;
            }
            i++;
        }
        argumanlar[i] = NULL;

        if((pid = fork()) == HATA) {
            perror("Hata: Cocuk proses olusturulamadi.\n");
            return -1;
        }
        //Arka plan islemleri
        //Cocuk proses
        if (pid == 0){
            //Kesme sinyalı "SIG_IGN" ile goz ardı ediliyor
            signal(SIGINT, SIG_IGN);
            //Eger sistemde olmayan bir proses isteniyorsa yada gecersiz istek varsa hata verir
            if (execvp(argumanlar[0], argumanlar) == HATA) {
                perror("Hata: Komut bulunamadı.\n");
                kill(getpid(), SIGTERM);
            }
        }
        //Ebeveyn Proses
        if (arkaplanAktif == 0) {
            //Eger "&"isareti varsa arkaplan 1 yapılır . 1 oldugu icin ebeveyn proses beklemez cocuk prosesi
            //ve cocuk proses arkada calısır.  
            waitpid(pid, NULL, 0);
        }else{
            printf("Arka Plan Islemi, %d, id ile olusturuldu.\n", pid);
        }

    }
    return 1;
}

int main(int *argc, char **argv[])
{
    //tek satırdaki maksimum karakterli dizi
    char satirKomut[BUFFERSIZE];   
    //Komutlar dizisi 
    char *Komutlar[ISTEKBOYUTU];
    //Varsayılan Komut Sayısı   
    int KomutSayisi = 1;  
    //Cocuk proses hatası icin  pid degeri              
    pid = -10;                      

    
    //Dongu 
    while(1) {
        //Komut bekle belirteci
        KomutBekle();
        //satırKomut "\0(NULL)" degeri ile BUFFERSIZE kadar dolduruluyor.Her istekte 
        //komutların karısmaması icin
        memset(satirKomut, '\0', BUFFERSIZE);
        //Komutları diziye kaydediyor
        fgets(satirKomut, BUFFERSIZE, stdin);

        //Kullanıcı deger girmesse Komut bekleme belirteci yazdırılır
        if((Komutlar[0] = strtok(satirKomut, " \n\t")) == NULL)
            continue;
        //KomutSayisi ni 1 de getiriyor
        KomutSayisi = 1;
        //Argumanlar diziye atılana kadar doner
        while((Komutlar[KomutSayisi] = strtok(NULL, " \n\t")) != NULL) {
            KomutSayisi++;
        }
        //Argumanlar fonksiyona verilir.
        KomutYoneticisi(Komutlar);
    }
    exit(0);
}