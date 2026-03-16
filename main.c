#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>
#include <tlhelp32.h>

void main(){

    DWORD CSS_PID = 0;

    HANDLE hSnapshot_Process = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //Créer une snapshot de tout les process
    PROCESSENTRY32 pe32; // Création d'une instance de la structure
    pe32.dwSize = sizeof(PROCESSENTRY32); // obligatoire sinon ça crash


    if (Process32First(hSnapshot_Process, &pe32)) {
    do {
        // pe32 contient les infos du processus courant
        // compare pe32.szExeFile ici
        if(strcmp(pe32.szExeFile, "cstrike_win64.exe") == 0){
            printf("Found !\n");
            CSS_PID = pe32.th32ProcessID;
            printf("Le PID est : %lu\n", CSS_PID);
            break;
        }
    } while (Process32Next(hSnapshot_Process, &pe32));
    
}

    
    printf("Hello\n");
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, CSS_PID);

    if(hProcess == NULL){
        DWORD err = GetLastError();
        printf("Erreur : %lu\n", err);
    }

    


    //Trouver la base address de client.dll

    HANDLE hSnapshot_Module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, CSS_PID);
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);
    uintptr_t clientBase = 0;

    if (Module32First(hSnapshot_Module, &me32)) {
        do {
            if (strcmp(me32.szModule, "client.dll") == 0) {
                clientBase = (uintptr_t)me32.modBaseAddr; // get base address
                break;
            
            }
        } while (Module32Next(hSnapshot_Module, &me32));
    }

    if(clientBase == 0 ){ //check si on a trouvé l'address, si non, exit
        return 1;
    }

    CloseHandle(hProcess);
    CloseHandle(hSnapshot_Process);
    CloseHandle(hSnapshot_Module);

}