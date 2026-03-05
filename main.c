#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>
#include <tlhelp32.h>

void main(){
    DWORD CSS_PID;
    printf("Hello\n");
    HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION, FALSE, 23504);

    if(hProcess == NULL){
        DWORD err = GetLastError();
        printf("Erreur : %lu\n", err);
    }

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32; // Création d'une instance de la structure
    pe32.dwSize = sizeof(PROCESSENTRY32); // obligatoire sinon ça crash


    if (Process32First(hSnapshot, &pe32)) {
    do {
        // pe32 contient les infos du processus courant
        // compare pe32.szExeFile ici
        if(strcmp(pe32.szExeFile, "cstrike_win64.exe") == 0){
            printf("Found !\n");
            CSS_PID = pe32.th32ProcessID;
            printf("Le PID est : %lu\n", CSS_PID);
        }
    } while (Process32Next(hSnapshot, &pe32));
}

    CloseHandle(hProcess);
}