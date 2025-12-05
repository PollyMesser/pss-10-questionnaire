#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>

//PSS-10
// Stressfragebogen
// Quellen: 
//   https://www.clinical-psychology-and-neuropsychology.uni-mainz.de/files/2019/02/Perceived_Stress_Scale_German.pdf
//   https://www.das.nh.gov/wellness/docs/percieved%20stress%20scale.pdf

// Definition der MC-Fragen
struct MC_Question {
    int question_id;
    char *question_text;
    char *options[5];
    int reverse_scoring;
};

// Definition User
struct User {
    char pseudo_code[7];
    int age;
    char gender[10];
    int answers[10];
    float score;
    char interpretation[8];
};

struct MC_Question pss10_questions[] = {
    {1, "Wie oft waren Sie im letzten Monat aufgewühlt, weil etwas unerwartet passiert ist?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 0},
    {2, "Wie oft hatten Sie im letzten Monat das Gefühl, nicht in der Lage zu sein, die wichtigen Dinge in Ihrem Leben kontrollieren zu können?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 0},
    {3, "Wie oft haben Sie sich im letzten Monat nervös und gestresst gefühlt?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 0},
    {4, "Wie oft waren Sie im letzten Monat zuversichtlich, dass Sie fähig sind, Ihre persönlichen Probleme zu bewältigen?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 1},
    {5, "Wie oft hatten Sie im letzten Monat das Gefühl, dass sich die Dinge zu Ihren Gunsten entwickeln?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 1},
    {6, "Wie oft hatten Sie im letzten Monat den Eindruck, nicht all Ihren anstehenden Aufgaben gewachsen zu sein?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 0},
    {7, "Wie oft waren Sie im letzten Monat in der Lage, ärgerliche Situationen in Ihrem Leben zu beeinflussen?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 1},
    {8, "Wie oft hatten Sie im letzten Monat das Gefühl, alles im Griff zu haben?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 1},
    {9, "Wie oft haben Sie sich im letzten Monat über Dinge geärgert, über die Sie keine Kontrolle hatten?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 0},
    {10, "Wie oft hatten Sie im letzten Monat das Gefühl, dass sich so viele Schwierigkeiten angehäuft haben, dass Sie diese nicht mehr überwinden konnten?", 
        {"0 = Nie", "1 = Selten", "2 = Manchmal", "3 = Häufig", "4 = Sehr häufig"}, 0}
};

// Funktion Einleitungstext
void printIntroduction() {
    printf("Die folgenden Fragen beschäftigen sich mit Ihren Gedanken und Gefühlen während des letzten Monats.\n");
    printf("Bitte geben Sie für jede Frage an, wie oft Sie in entsprechender Art und Weise gedacht oder gefühlt haben.\n\n");
}

// Funktion zur Speicherung der aktuellen Jahreszahl
int getCurrentYear(void) {
    time_t rohzeit;
    struct tm *zeitinfo;
    time(&rohzeit);
    zeitinfo = localtime(&rohzeit);
    int jahr = zeitinfo -> tm_year + 1900;
    return jahr;
}

// Funktion zur Generierung des Pseudocodes
void generatePseudoCode(struct User* user1) {
    char mutter[15];
    char geburtsjahr_str[5];
    char geburtsstadt[15];
    int jahr = getCurrentYear();
    int mindest_jahr = jahr - 100;
    int max_jahr = jahr - 18;
    printf("Zur Erstellung Ihres persönlichen Pseudonymisierungscodes benötigen wir einige Angaben von Ihnen.\n");
    do {
        printf("Wie lautet der Vorname Ihrer Mutter?\n");
        scanf("%14s", mutter); // Speicher für den Vornamen der Mutter reservieren
        while (getchar() != '\n'); // Eingabepuffer leeren
    } while (strlen(mutter) <= 1 || !isalpha(mutter[0]) || !isalpha(mutter[1])); // Mindestens 2 Buchstaben, nur Buchstaben

    do {
        printf("In welchem Jahr sind Sie geboren?\n");
        scanf("%4s", geburtsjahr_str);
        while(getchar() != '\n'); // Eingabepuffer leeren
        if (atoi(geburtsjahr_str) < (mindest_jahr) || atoi(geburtsjahr_str) > (max_jahr) ) {
            printf("Bitte geben Sie ein gültiges Geburtsjahr ein (%d - %d).\n", mindest_jahr, max_jahr);
        } 
    } while (strlen(geburtsjahr_str) < 4 || strlen(geburtsjahr_str) > 4 || !isdigit(geburtsjahr_str[0]) || !isdigit(geburtsjahr_str[1]) || !isdigit(geburtsjahr_str[2]) || !isdigit(geburtsjahr_str[3]) || atoi(geburtsjahr_str) < mindest_jahr || atoi(geburtsjahr_str) > max_jahr);

    do {
        printf("In welcher Stadt sind Sie geboren?\n");
        scanf("%14s", geburtsstadt);
        while (getchar() != '\n'); // Eingabepuffer leeren
    } while (strlen(geburtsstadt) <= 1 || !isalpha(geburtsstadt[0]) || !isalpha(geburtsstadt[1]));

    snprintf((*user1).pseudo_code, sizeof((*user1).pseudo_code), "%c%c%c%c%c%c",
             toupper(mutter[0]),
             toupper(mutter[1]),
             geburtsjahr_str[2],
             geburtsjahr_str[3],
             toupper(geburtsstadt[0]),
             toupper(geburtsstadt[1]))  ;
}

// Funktion soziodemographische Fragen
void sociodemographicQuestions(struct User* user1) {
    printf("Nun noch einige soziodemographische Fragen.\n");
    // Alter abfragen
    do {
        printf("Wie alt sind Sie?\n");
        scanf("%d", &user1 -> age);
        while(getchar() != '\n'); // Eingabepuffer leeren
        if (user1 -> age < 18 || user1 -> age > 100) {
            printf("Bitte geben Sie ein gültiges Alter ein (18 - 100).\n");
        }
    } while (user1 -> age < 18 || user1 -> age > 100);

    // Geschlecht abfragen
    char gender_input[2];
    do {
        printf("Welchem Geschlecht gehören Sie an? (m/w/d)\n");
        scanf("%1s", gender_input);
        while (getchar() != '\n'); // Eingabepuffer leeren
        switch (gender_input[0]) {
        case 'm': strcpy(user1 -> gender,  "männlich"); break;
        case 'w': strcpy(user1 -> gender,  "weiblich"); break;
        case 'd': strcpy(user1 -> gender,  "divers"); break;
        default: strcpy(user1 -> gender, "unbekannt"); break;
        }
    } while (strlen(gender_input) != 1 || !isalpha(gender_input[0]));

    
}

// Funktion zur Berechnung des PSS-10 Scores
 void calculatePSS10Score(struct User* user1) {
    int total_score = 0;
    for (int i = 0; i < 10; i++) {
        int answer = user1 -> answers[i];
        if (pss10_questions[i].reverse_scoring) {
            answer = 4 - answer; // Umkehrung der Punktzahl für umgekehrte Fragen
        }
        total_score += answer;
    }
    user1 -> score = total_score; // Summenbildung
}

// Funktion zur Interpretation des PSS-10 Scores
void interpretPSS10Score(struct User* user1) {
    char empfehlung[50];
        if (user1 -> score <= 13) {
            strcpy(user1 -> interpretation, "niedrig");
            strcpy(empfehlung, "weiter so!");
        } else if (user1 -> score <= 26) {
            strcpy(user1 -> interpretation, "moderat");
            strcpy(empfehlung, "auch mal Sport und Entspannungsübungen!");
        } else {
            strcpy(user1 -> interpretation, "hoch");
            strcpy(empfehlung, "unbedingt mehr Sport und Enspannungsübungen!");
        }
        printf("Ihre wahrgenommener Stress ist %s.\n", user1 -> interpretation);
        printf("Machen Sie %s\n", empfehlung);

    }


// Funktion zur Durchführung des PSS-10 Fragebogens
void conductPSS10Questionnaire(struct User* user1) {
    printIntroduction();
    for (int i = 0; i < 10; i++) {
        printf("%s\n", pss10_questions[i].question_text);
        for (int j = 0; j < 5; j++) {
            printf("%s\n", pss10_questions[i].options[j]);
        }
        int answer;
        printf("Bitte geben Sie Ihre Antwort (0-4) ein:\n");
        while (scanf(" %d", &answer) != 1 || answer < 0 || answer > 4) {
        // Buffer leeren
        while (getchar() != '\n');
        printf("Ungültige Eingabe! Bitte nur 0-4 eingeben:\n");
        }
        user1->answers[i] = answer;
    }
    calculatePSS10Score(user1);
}

int main() {
    // Konsole für UTF-8 konfigurieren
    SetConsoleOutputCP(CP_UTF8);
    printf("Willkommen zum PSS-10 Fragebogen zur Erfassung des wahrgenommenen Stresses!\n\n");
    struct User user;
    generatePseudoCode(&user);
    sociodemographicQuestions(&user);
    conductPSS10Questionnaire(&user);
    printf("\n\nIhr Code ist %s und Sie sind %d Jahre alt, Geschlecht: %s.\n", user.pseudo_code, user.age, user.gender);
    printf("Sie haben einen Score von %.2f im PSS-10 Fragebogen.\n\n", user.score);
    interpretPSS10Score(&user);
    printf("Vielen Dank für Ihre Teilnahme am Fragebogen!\n\n");
    printf("Quellen:\n");
    printf("https://www.das.nh.gov/wellness/docs/percieved%%20stress%%20scale.pdf\n");
    printf("https://www.clinical-psychology-and-neuropsychology.uni-mainz.de/files/2019/02/Perceived_Stress_Scale_German.pdf\n");
    return 0;
}