#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_CONTESTANTS 42  //Max antal deltagare
#define EXITNUMBER 100      //Används för att gå ur en sökfunktion
#define MAX_MINUTES 300
#define MAX_SECONDS 59

//Skapar en struct av typen "contestant" och döper den till "cont"
typedef struct contestant{
    int startnr;
    char firstname[50];
    char lastname[50];
    char club[50];
    float rideTime;
    int entered;
}cont;

//Prototyper nedan
void regCont();
void seeAllCont(cont *arrCont);
void saveANMAL(cont *newCont);
cont* loadAllANMAL();
void clearCont();
void genStartnr(cont *arrCont);
void saveStartnrSTARTFIL(cont *arrCont, int numbContestants);
cont* loadSTARTFIL();
void enterRideTime();
void saveRideTimeSTARTFIL(cont *arrContStart);
void resultSimple(cont *arrContStart);
void resultAdvanced(cont *arrContStart);
void menu();
void case1SubMenu();

//"main()" kallar funktionen "menu()" vilken styr hela programmet.
int main(){
    system("chcp 1252");
    system("cls");
    menu();
    return 0;
}

//"regCont()" promptar användaren att ange förnamn, efternamn och klubb för tävlande
//Den sätter "newCont.entered" till "1" för den anmälda användaren
//kallar sedan "saveANMAL()" med minnesadressen till &newCont för att spara tävlande
//till filen "ANMAL.DAT"
void regCont(){
    system("cls");
    cont newCont;

    printf("A N M Ä L A N   T I L L   30 K M\n\n");

    printf("Registrera åkare\n\n");
    printf("Ange förnamn: ");
    getchar();
    fgets(newCont.firstname, sizeof(newCont.firstname), stdin);
    newCont.firstname[strcspn(newCont.firstname, "\n")]=0;

    printf("\nAnge efternamn: ");
    fgets(newCont.lastname, sizeof(newCont.lastname), stdin);
    newCont.lastname[strcspn(newCont.lastname, "\n")]=0;

    printf("\nAnge klubb: ");
    fgets(newCont.club, sizeof(newCont.club), stdin);
    newCont.club[strcspn(newCont.club, "\n")]=0;

    newCont.entered=1;

    saveANMAL(&newCont);
    system("cls");
    printf("Tävlande %s har sparats!\n\n", newCont.firstname);
}

//"seeAllCont()" tar en pointer till en array av structar, räknar tävlande,
//om inga är anmälda gör den en tidig "return;" ur funktionen
//annars itererar den genom de som är anmälda (entered = 1) och printar deras
//förnamn, efternamn och klubb
void seeAllCont(cont *arrCont){
    system("cls");
    int i=0, numbContestants=0;

    for(i=0;i<MAX_CONTESTANTS;i++){
        if(arrCont[i].entered==1)
            numbContestants++;
    }

    if(numbContestants <= 1){
        system("cls");
        printf("Du måste anmäla fler tävlande!\n\n");
        return;
    }

    printf("A N M Ä L D A  Å K A R E\n\n");

    for(i=0;i<numbContestants;i++){
        if(arrCont[i].entered==1){
            printf("\n%s %s - %s\n\n",
                    arrCont[i].firstname,
                    arrCont[i].lastname,
                    arrCont[i].club);
        }
    }
}

//"saveANMAL()" sparar datan användaren angett för tävlande till "ANMAL.DAT"
//vi ska inte använda "ab" (append binary) men nu blev det så ändå
//den lägger alltså till data strukturerad efter structen "contestants"
//i slutet av filen "ANMAL.DAT"
void saveANMAL(cont *newCont){

    FILE *file;

    file=fopen("ANMAL.DAT", "ab");

    if(file==NULL){
        printf("Hittar inte filen!");
        exit(0);
    }

    fwrite(newCont, sizeof(struct contestant), 1, file);

    fclose(file);
}

//"loadAllANMAL()" skapar en array av structar och allokerar minne dynamiskt till denna
//samt initierar alla värden som inte är av typen "char" till "0"
//sen öppnar den filen "ANMAL.DAT" och läser dess innehåll till arrayen
//returnerar en pointer till första elementet i arrayen av structar "arrCont"
cont* loadAllANMAL(){
    int i=0;

    cont *arrCont=malloc(MAX_CONTESTANTS*sizeof(cont));
    if(arrCont!=NULL){
        for(i=0;i<MAX_CONTESTANTS;i++){
            arrCont[i].entered=0;
            arrCont[i].rideTime=0;
            arrCont[i].startnr=0;
        }
    }

    FILE *file;

    file=fopen("ANMAL.DAT", "rb");

    if(file==NULL){
        printf("Hittar inte filen!");
        exit(0);
    }

    fread(arrCont, sizeof(struct contestant), MAX_CONTESTANTS, file);

    fclose(file);

    return arrCont;
}

//"clearCont()" öppnar filerna "ANMAL.DAT" och "STARTFIL.DAT" och "tömmer dem" eller snarare
//skriver över dem med en tom fil.
void clearCont(){

    FILE *file1=fopen("ANMAL.DAT", "w");
    FILE *file2=fopen("STARTFIL.DAT", "w");

    if(file1==NULL||file2==NULL){
        printf("Hittar inte filen!");
        exit(0);
    }

    fclose(file1);
    fclose(file2);

    system("cls");
    printf("Tävlande har nollställts!\n\n");
}

//"genStartnr()" tar en pointer till första elementet i en array av structar,
//i det här fallet "arrCont" eftersom "genStartnr()" kallas i "menu()" med
//argumentet "loadAllANMAL()".
void genStartnr(cont *arrCont){
    srand(time(NULL));
    system("cls");
    int i=0, numbContestants=0, used=0, randNmbr=0;

    for(i=0;i<MAX_CONTESTANTS;i++){ //<-------------------------------Räknar antalet deltagare
        if(arrCont[i].entered==1){
            numbContestants++;
        }
    }

    if(numbContestants <= 1){   //<-----------------------------------Kontrollerar om antalet tävlande minre än eller lika med "1"
        printf("Du måste anmäla fler tävlande!\n\n");
        return;
    }

    int randArr[numbContestants];   //<--------------------------------Skapar en array med storleken "numbContestants"

    for(i=0;i<numbContestants;i++){     //<----------------------------Fyller randArr[numbContestants] med siffror från 1-numbContestants
        randArr[i]=(i+1);
    }

    printf("T Ä V L A N D E  M E D  S T A R T N U M M E R\n\n");

    for(i=0;i<numbContestants;i++){     //<----------------------------Genererar slumptal mellan 1 och "numbContestants" till "randNmbr" och byter sedan ut dessa med talen i "randArr[]"
        randNmbr=rand()%(numbContestants-1)+1;
        used=randArr[randNmbr];
        randArr[randNmbr]=randArr[i];
        randArr[i]=used;
    }

    for(i=0;i<numbContestants;i++){     //<----------------------------Tilldelar startnumren i "randArr[]" till "arrCont"
            arrCont[i].startnr=randArr[i];
            printf("\n%i - %s %s - %s\n",
                   arrCont[i].startnr,
                   arrCont[i].firstname,
                   arrCont[i].lastname,
                   arrCont[i].club);
    }

    printf("\n");

    saveStartnrSTARTFIL(arrCont, numbContestants);      //<------------Anropar "saveStartnrSTARTFIL()" för att spara startnummer, skickar också med "numbContestants" för enkel printning

    free(arrCont);      //<--------------------------------------------Frigör minne allokerat till "arrCont"

}

//"saveStartnrSTARTFIL()" tar en pointer till array av structar typ "cont" och int "numbContestants"
//öppnar "STARTFIL.DAT" och sparar all data den laddade tidigare + startnummer
void saveStartnrSTARTFIL(cont *arrCont, int numbContestants){
    FILE *file;

    file=fopen("STARTFIL.DAT", "wb");

    if(file==NULL){
        printf("Hittar inte filen!");
    }

    fwrite(arrCont, sizeof(cont), MAX_CONTESTANTS, file);

    fclose(file);

}

//"loadSTARTFIL()" skapar en array av structar och allokerar minne dynamiskt till denna
//samt initierar alla värden som inte är av typen "char" till "0"
//sen öppnar den filen "STARTFIL.DAT" och läser dess innehåll till arrayen
//returnerar en pointer till första elementet i arrayen av structar "arrContStart"
cont* loadSTARTFIL(){
    int i=0;

    cont *arrContStart=malloc(MAX_CONTESTANTS*sizeof(cont));
        if(arrContStart!=NULL){
            for(i=0;i<MAX_CONTESTANTS;i++){
                arrContStart[i].entered=0;
                arrContStart[i].startnr=0;
            }
        }

    FILE *file;

    file=fopen("STARTFIL.DAT", "rb");

    if(file==NULL){
        printf("Hittar inte filen!");
    }

    fread(arrContStart, sizeof(cont), MAX_CONTESTANTS, file);

    fclose(file);

    return arrContStart;
}

//"enterRideTime()" tar en pointer till en array av structar (i det här fallet arrContStart
//eftersom den kallas med argumentet "loadSTARTFIL()" i "menu()").
//Tävlande räknas och att tillräckligt många är anmälda kontrolleras
//Den skriver åkare och åktider till skärmen, och låter användaren välja åkare
//promptar användaren att ange tid i minuter och sekunder
//sparar sedan dessa till STARTFIL.DAT och gör om loopen till dess att användaren väljer att gå ur
void enterRideTime(cont *arrContStart){
    int i=0, j=0, numbContestants=0, srchNmbr=0;
    float rideTime=0, minutes=0, seconds=0;

    for(i=0;i<MAX_CONTESTANTS;i++){     //<----------------------------Räknar tävlande
        if(arrContStart[i].entered==1)
            numbContestants++;
    }

    if(numbContestants <= 1){       //<--------------------------------Kontrollerar om tillräckligt många är anmälda
        system("cls");
        printf("Du måste anmäla fler tävlande!\n\n");
        return;
    }

    system("cls");

        do{
            printf("Ange startnumret för åkaren som kommit i mål för att ange åktid\nAnge '100' för att återgå till menyn och spara åktiderna.\n\nÅkare:\n");

            for(j=0;j<numbContestants;j++){     //<-------------------Skriver deltagarnas data till skärmen
                printf("\n%i - %s %s - %s - %.2f\n",
                       arrContStart[j].startnr,
                       arrContStart[j].firstname,
                       arrContStart[j].lastname,
                       arrContStart[j].club,
                       arrContStart[j].rideTime);
            }

            printf("\nAnge startnummer: ");     //<-------------------Promptar användaren att söka efter ett startnummer
            scanf("%i", &srchNmbr);

            if(srchNmbr==EXITNUMBER){      //<------------------------Kollar om användaren vill gå ur loopen
                        system("cls");
                        return;
                    }

            if(srchNmbr<1||srchNmbr>numbContestants){   //<-----------Kollar om sökningen är inom range för "startnr"
                    system("cls");
                    printf("Det startnumret finns inte!\n\n");
                    srchNmbr=0;
                    continue;
                }

            for(i=0;i<numbContestants;i++){
                if(arrContStart[i].startnr==srchNmbr){      //<--------Skriver data för deltagaren vars startnummer angetts till skärmen
                    printf("\nAnge åktid för:\n%i - %s %s - %s\n\n",
                            arrContStart[i].startnr,
                            arrContStart[i].firstname,
                            arrContStart[i].lastname,
                            arrContStart[i].club);

                    printf("Åktid min: ");     //<--------------------Promptar användaren att fylla i åktid "minuter" och sparar det till en variabel av typen "float"
                    scanf("%f", &minutes);

                        if(minutes>MAX_MINUTES){    //<---------------Kontrollerar om siffran angedd är för hög
                            system("cls");
                            printf("Ogiltigt format!\n\n");
                            minutes=0;
                            break;
                        }

                    printf("\nÅktid sek: ");
                    scanf("%f", &seconds);

                        if(seconds>MAX_SECONDS){
                            system("cls");
                            printf("Ogiltigt format!\n\n");
                            seconds=0;
                            break;
                        }

                    rideTime=minutes+(seconds/100);     //<----------eftersom formatet ska vara "0.00" måste sekunderna bli decimaler till minuter, därför "/100". Sparar sedan datan till en variabel av typ "float"
                    arrContStart[i].rideTime=rideTime;  //<----------sätter den aktuella tävlandes tid till värdet i "rideTime"
                    saveRideTimeSTARTFIL(arrContStart); //<----------sparar hela smacket som laddats till STARTFIL
                    system("cls");
                    printf("Åktid uppdaterad för %s %s\n\n", arrContStart[i].firstname, arrContStart[i].lastname);
                    break;
                }

            }

        }while(1);

        free(arrContStart);     //<--------------------------------Frigör dynamiskt allokerat minne för "arrContStart"
}

//"saveRideTimeSTARTFIL()" tar en pointer till första elementet av av en array av structar
//Öppnar filen "STARTFIL.DAT" och sparar all data den laddade tidigare + rideTime
void saveRideTimeSTARTFIL(cont *arrContStart){
    FILE *file;

    file=fopen("STARTFIL.DAT", "wb");

    if(file==NULL){
        printf("Hittar inte filen!");
    }

    fwrite(arrContStart, sizeof(cont), MAX_CONTESTANTS, file);

    fclose(file);

}


//"resultSimple()" tar en pointer till första elementet i en array av structar
//räknar antalet tävlande och kollar om några är anmälda
//skriver först de tävlande som har en angedd tid till skärmen
//skriver sedan de tävlande som inte har en tid till skärmen
//Frigör tillslut dynamiskt allokerat minne för arrContStart
void resultSimple(cont *arrContStart){
    system("cls");
    int i=0, numbContestants=0;

    for(i=0;i<MAX_CONTESTANTS;i++){
        if(arrContStart[i].entered==1)
            numbContestants++;
    }

    if(numbContestants <= 1){
        system("cls");
        printf("Du måste anmäla fler tävlande!\n\n");
        return;
    }

    printf("O S O R T E R A D  R E S U L T A T L I S T A\n\n");

    for(i=0;i<numbContestants;i++){
        if(arrContStart[i].rideTime!=0.00){
            printf("\n%i - %s %s - %s - %.2f\n\n",
                    arrContStart[i].startnr,
                    arrContStart[i].firstname,
                    arrContStart[i].lastname,
                    arrContStart[i].club,
                    arrContStart[i].rideTime);
        }

    }

    for(i=0;i<numbContestants;i++){
        if(arrContStart[i].rideTime==0.00){
            printf("\n%i - %s %s - %s - %.2f\n\n",
                    arrContStart[i].startnr,
                    arrContStart[i].firstname,
                    arrContStart[i].lastname,
                    arrContStart[i].club,
                    arrContStart[i].rideTime);
        }
    }

    free(arrContStart);

}


//"resultAdvanced()" tar en pointer till första elementet i en array av structar
//räknar antalet tävlande och kollar om några är anmälda
//sorterar sedan hela structen utifrån "rideTime"
//skriver först de tävlande som har en angedd tid till skärmen
//skriver sedan de tävlande som inte har en tid till skärmen
//frigör till slut dynamiskt allokerat minne för arrContStart
void resultAdvanced(cont *arrContStart){
    system("cls");
    int numbContestants=0, i=0, j=0;
    cont used;

    for(i=0;i<MAX_CONTESTANTS;i++){
        if(arrContStart[i].entered==1)
            numbContestants++;
    }

    if(numbContestants <= 1){
        system("cls");
        printf("Du måste anmäla fler tävlande!\n\n");
        return;
    }

    for(i=0;i<numbContestants-1;i++){
        for(j=0;j<numbContestants-i-1;j++){
            if(arrContStart[j].rideTime > arrContStart[j+1].rideTime){
                used=arrContStart[j];
                arrContStart[j]=arrContStart[j+1];
                arrContStart[j+1]=used;
            }
        }
    }

    printf("S O R T E R A D  R E S U L T A T L I S T A\n\n");

    for(i=0;i<numbContestants;i++){
        if(arrContStart[i].rideTime!=0.00){
            printf("\n%i - %s %s - %s - %.2f\n\n",
                    arrContStart[i].startnr,
                    arrContStart[i].firstname,
                    arrContStart[i].lastname,
                    arrContStart[i].club,
                    arrContStart[i].rideTime);
        }

    }

    for(i=0;i<numbContestants;i++){
        if(arrContStart[i].rideTime==0.00){
            printf("\n%i - %s %s - %s - %.2f\n\n",
                    arrContStart[i].startnr,
                    arrContStart[i].firstname,
                    arrContStart[i].lastname,
                    arrContStart[i].club,
                    arrContStart[i].rideTime);
        }
    }

    free(arrContStart);
}

//"menu()" sköter meny-operationerna i programmet och kallar funktionerna.
void menu(){
    int choice=0;

    while(1){
        printf("S K I D Å K N I N G   30KM\n\n");
        printf("1.) Nollställ för ny tävling\n");
        printf("2.) Registrera åkare\n");
        printf("3.) Se alla anmälda åkare\n");
        printf("4.) Lotta startordning\n");
        printf("5.) Registrera åktider\n");
        printf("6.) Visa osorterad resultatlista\n");
        printf("7.) Visa sorterad resultatlista\n");
        printf("8.) Avsluta och radera tävlande\n\n");
        printf("Gör ett val: ");
        scanf("%i", &choice);

        switch(choice){
        case 1:
            case1SubMenu();
            break;

        case 2:
            regCont();
            break;

        case 3:
            seeAllCont(loadAllANMAL());
            break;

        case 4:
            genStartnr(loadAllANMAL());
            break;

        case 5:
            enterRideTime(loadSTARTFIL());
            break;

        case 6:
            resultSimple(loadSTARTFIL());
            break;

        case 7:
            resultAdvanced(loadSTARTFIL());
            break;

        case 8:
            system("cls");
            clearCont();
            printf("Programmet avslutas\n\n");
            exit(0);

        default:
            system("cls");
            printf("Det finns bara åtta val!\n\n");
            break;

        }

    }
}

//"case1SubMenu()" skapar ett ytterligare menylager för att säkerställa att
//användaren verkligen vill nollställa tävlande
void case1SubMenu(){
    int choice;
    system("cls");

    printf("Är du säker på att du vill nollställa startfältet?\n\n");
    printf("1.) Ja");
    printf("\n2.) Nej");
    printf("\n\nGör ett val: ");
    scanf("%i", &choice);
        if(choice==1)
            clearCont();
        else{
            system("cls");
            menu();
            }

}

