/*エニグマ暗号機（プラグボードなし、３ローター簡易型）*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ROTORS 3
#define PATTERNS 3

void print_logo(void) {
    printf("  _____ _   _ _____ _____ __  __   ___  \n");
    printf(" | ____| \\ | |_   _|  ___|  \\/  | / _ \\ \n");
    printf(" |  _| |  \\| | | | | |  _| \\  / || |_| |\n");
    printf(" | |___| |\\  |_| |_| |_| | |  | ||  _  |\n");
    printf(" |_____|_| \\_|_____|_____|_|  |_||_| |_|\n");
}


/*ローターのパターン定義（0=A, 1=B ... 25=Z）*/
int rotor_patterns[PATTERNS][ROTORS][26] = {
    {   /*Aパターン*/
        {4,10,12,5,11,6,3,16,21,25,13,19,14,22,24,7,23,20,18,15,0,8,1,17,2,9},
        {0,9,3,10,18,8,17,20,23,1,11,7,22,19,12,2,16,6,25,13,15,24,5,21,14,4},
        {1,3,5,7,9,11,2,15,17,19,23,21,25,13,24,4,8,22,6,0,10,12,20,18,16,14}
    },
    {   /*Bパターン*/
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25},
        {25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0},
        {4,0,2,6,7,1,3,5,9,8,12,10,11,15,14,16,18,17,20,19,21,22,23,24,25,13}
    },
    {   /*Cパターン*/
        {5,8,15,1,17,4,2,23,12,10,6,11,3,25,18,7,24,16,20,14,0,22,21,19,13,9},
        {1,7,12,0,9,5,22,17,3,16,23,14,11,25,6,2,20,24,19,21,13,4,18,10,8,15},
        {3,14,10,1,6,23,2,7,25,18,12,5,16,20,9,4,0,8,24,21,13,19,11,22,15,17}
    }
};

/*リフレクター*/
int reflector[26] = { 24,17,20,7,16,18,11,3,15,23,13,6,14,10,12,8,4,1,5,25,2,22,21,9,0,19 };

/*ローター位置*/
int rotor_pos[ROTORS];

/*選択したローターパターン*/
int (*rotor[ROTORS])[26];

/*逆ローター計算*/
int rotor_inverse(int r[], int input) {
    for (int i = 0; i < 26; i++) if (r[i] == input) return i;
    return 0;
}

/*エニグマ処理（encrypt=1で暗号化、0で復号化）*/
char enigma_process(char ch) {
    if (!isalpha(ch)) return ch;

    /*ローター回転*/
    rotor_pos[ROTORS - 1]++;
    for (int i = ROTORS - 1; i > 0; i--) {
        if (rotor_pos[i] >= 26) { rotor_pos[i] = 0; rotor_pos[i - 1]++; }
    }
    if (rotor_pos[0] >= 26) rotor_pos[0] = 0;


    int c = isupper(ch) ? ch - 'A' : ch - 'a';

    /*前進ローター通過*/
    for (int i = ROTORS - 1; i >= 0; i--) {
        c = (rotor[i][0][(c + rotor_pos[i]) % 26] - rotor_pos[i] + 26) % 26;
    }

    /*リフレクター*/
    c = reflector[c];

    /*逆ローター通過*/
    for (int i = 0; i < ROTORS; i++) {
        int adjusted_c = (c + rotor_pos[i]) % 26;
        c = (rotor_inverse(rotor[i][0], adjusted_c) - rotor_pos[i] + 26) % 26;
    }

    return isupper(ch) ? c + 'A' : c + 'a';

}

int main(void) {
    print_logo();
    int pattern_choice;
    char text[512];

    printf("【エニグマ暗号機（プラグボードなし、簡易型）】\n\n");

    /*パターン選択*/
    printf("ローターパターンを選択（0=A,1=B,2=C）: ");
    scanf("%d", &pattern_choice);
    getchar();

    for (int i = 0; i < ROTORS; i++) rotor[i] = rotor_patterns[pattern_choice][i];

    /*初期ローター位置*/
    printf("ローター初期位置3つを入力（例: 0 0 0）: ");
    for (int i = 0; i < ROTORS; i++) scanf("%d", &rotor_pos[i]);
    getchar();

    /*暗号化/復号化選択*/
    char choice;
    printf("暗号化する場合は E, 復号化する場合は D: ");
    scanf(" %c", &choice);
    getchar();
    int encrypt = toupper(choice) == 'E';

    /*文字列入力*/
    printf("文字列を入力: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;

    char result[512];
    for (int i = 0; text[i] != '\0'; i++)
        result[i] = enigma_process(text[i]);
    result[strlen(text)] = '\0';

    if (encrypt) printf("暗号化: %s\n", result);
    else printf("復号化: %s\n", result);

    return 0;
}
