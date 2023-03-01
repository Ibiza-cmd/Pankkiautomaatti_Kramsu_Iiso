/*Pankkiautomaatti Iiso Kramsu
Kotitehtava 6.*/

#include <stdio.h>
#include <string.h>

#define KOKO 18

void valikko(char tilinumero[]);
void Talletus(char tilinumero[]);   //Rahan talletus funktio
void Nosto(char tilinumero[]);      //Rahan nosto funktio
void SaldoTarkistus(char tilinumero[]);      //Saldon tarkistus funktio

int LueKokonaisluku(void);   //syötteen tarkistus
void lueRoskat(void);

/*Funktio ei saa parametreja. Funktio palauttaa arvon eri toiminnan mukaan.*/

int main(void){ /*main-funktio*/

FILE *tiedLuku;
char tilinumero[KOKO];
char syottoPin[KOKO], tarkistusPin[KOKO];
int pinVaarin = 3;
int pinOikein = 0;

printf("Tervetuloa kayttamaan OTTO pankkiautomaattia\nSyota tilinumerosi");
fgets(tilinumero, KOKO, stdin);

if( tilinumero[ strlen(tilinumero) -1] == '\n'){
tilinumero[ strlen(tilinumero) -1] = '\0';
}

 else{
  lueRoskat();
 }

 strcat(tilinumero, ".tili");

 tiedLuku = fopen(tilinumero, "r");
 if (tiedLuku == NULL){
        printf("Tilia ei loytynyt\n");
        return 0;
 }

 fgets(tarkistusPin, KOKO, tiedLuku);
 while(pinVaarin  > 0 && pinOikein == 0){
      printf("Syota tilin PIN-koodi:");
      fgets(syottoPin, KOKO, stdin);

      if(strcmp(syottoPin, tarkistusPin) == 0){

         printf("PIN oikein!\n");
         fclose(tiedLuku);
         valikko(tilinumero);
         pinOikein = 1;

         }else{

            pinVaarin--;
            printf("PIN vaarin, %d yritysta jaljella\n", pinVaarin);
        }
 }

    printf("Kiitos kaynnista!");
    return 0;


}


/*Void valikko toimii pääfunktiona, josta käyttäjä voi valita seuraavat toiminnot syöttämällä näppäimmistöltä numeron:
1. RAHAN NOSTO
2. SALDON TARKISTUS
3. RAHAN TALLETUS
0. LOPETTAA FUNKTION TOIMINNON

Valikkofunktio saa parametrina tilinumeron, jonka kayttaja syotti.
*/

void valikko(char tilinumero[]){

//lue valinta
int valinta = 0;

do{
    printf("Valitse \n1 = NOSTO\n2 = SALDO\n3TALLETUS\n0 = LOPETUS");
	valinta = LueKokonaisluku();



       switch (valinta){      /*Switch-valintarakenne, jonka avulla valitaan käytettävä funktio*/

       case 1:
           Nosto(tilinumero);
           break;

       case 2:
           SaldoTarkistus(tilinumero);
           break;

       case 3:
           Talletus(tilinumero);
           break;

       case 0:

           break;

       default:

           break;
       }

    } while(valinta !=0);

}


/*RAHAN NOSTO FUNKTIO
Rahan nostofunktio saa parametrina tilinumeron, jonka kayttaja syotti.
*/

void Nosto(char tilinumero[]){

FILE *tiedLuku;
FILE *tiedKirjoitus;

int PinKoodi, tilinSaldo;
int NostoMaara = 0;
int NostoValinta = 0;
int seteli20 = 0;
int seteli50 = 0;

tiedLuku = fopen(tilinumero, "r");
fscanf(tiedLuku, "%d", &PinKoodi);
fscanf(tiedLuku, "%d", &tilinSaldo);
fclose(tiedLuku);





printf("Syota nostettava maara:\n");
NostoMaara = LueKokonaisluku();

   if(NostoMaara >= 20 || NostoMaara%10 == 0 || NostoMaara != 30 || NostoMaara <= tilinSaldo){
   tilinSaldo = tilinSaldo - NostoMaara;
   tiedKirjoitus = fopen(tilinumero, "w");
   fprintf(tiedKirjoitus, "%d\n", PinKoodi);
   fprintf(tiedKirjoitus, "%d", tilinSaldo);
   fclose(tiedKirjoitus);

do{

   if(NostoMaara == 20 || NostoMaara == 40 || NostoMaara == 60 || NostoMaara == 80){
    seteli20++;
    NostoMaara = NostoMaara - 20;

   }else{

    seteli50++;
    NostoMaara = NostoMaara - 50;
   }
   }while(NostoMaara > 0);

   printf("Saat %dkappaletta 20 seteleita ja %dkappaletta 50 seteleita\n", seteli20, seteli50);
   printf("Haluatko tilin saldon naytolle. \n1 = KYLLA\n2 = EI\n");
   NostoValinta = LueKokonaisluku();

   if(NostoValinta == 1){
	   SaldoTarkistus(tilinumero);
   }

}else if(tilinSaldo < NostoMaara){
	printf("Tilillasi ei ole tarpeeksi rahaa\n");
}else{
	printf("Virheellinen Nostosumma syotettu\n");

   }

}


/*RAHAN TALLETUS FUNKTIO
Rahan talletusfunktio saa parametrinä tilinumeron jonka käyttäjä alun perin syötti.
*/

void Talletus(char tilinumero[]){

FILE *tiedLuku;
FILE *tiedKirjoitus;

int talletusValinta;
int talletettavasumma = 0;
int PinKoodi, tilinSaldo;

tiedLuku = fopen(tilinumero, "r");
fscanf(tiedLuku, "%d", &PinKoodi);
fscanf(tiedLuku, "%d", &tilinSaldo);
fclose(tiedLuku);

do{
	printf("Syota rahat:");
	talletettavasumma = LueKokonaisluku();

	if(talletettavasumma <= 0){
		printf("Et syottanyt rahaa. Yrita uudelleen.\n");
	}

}while(talletettavasumma <= 0);

tilinSaldo = talletettavasumma + tilinSaldo;
tiedKirjoitus = fopen(tilinumero, "w");
fprintf(tiedKirjoitus, "%d\n", PinKoodi);
fprintf(tiedKirjoitus, "%d", tilinSaldo);
fclose(tiedKirjoitus);

printf("Talletit %d\n", talletettavasumma);
printf("Haluatko tilin saldon naytolle. \n1 = KYLLA\n2 = EI");
talletusValinta = LueKokonaisluku();

if(talletusValinta == 1){
	SaldoTarkistus(tilinumero);
    }
}


/*SALDON TARKISTUSFUNKTIO
Saldon tarkistusfunktio saa parametrinä tilinumeron jonka käyttäjä alun perin syötti.
*/
void SaldoTarkistus(char tilinumero[]){

 FILE *tiedLuku;
 char roska[KOKO];
 int saldoValinta, tilinSaldo = 0;

 tiedLuku = fopen(tilinumero, "r");
 fgets(roska, KOKO, tiedLuku);
 fscanf(tiedLuku, "%d", &tilinSaldo);
 fclose(tiedLuku);

 printf("tilin saldo on %d\n", tilinSaldo);
}


/* LueKokonaisluku funktio ja lueRoskat lainattu Moodlen esimerkkimateriaalista*/
int LueKokonaisluku(void){

 int luku;
 int result = 0;
 char merkki;

 while( (result=scanf("%d%c", &luku, &merkki) ) == 0 || ( result == 2 && merkki != '\n') ){

 printf("\nEt syottanyt kokonaislukua!\n");

 lueRoskat();

 printf("\nYrita uudelleen > ");
 }
 return luku;
}

void lueRoskat(void){
while( fgetc(stdin) != '\n');
}

