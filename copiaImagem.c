#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* sourceFile = fopen("discord.png", "rb");
    if (!sourceFile) {
        printf("Erro ao abrir o arquivo de origem.\n");
        return 1;
    }

    FILE* destinationFile = fopen("destino.png", "wb");
    if (!destinationFile) {
        printf("Erro ao abrir o arquivo de destino.\n");
        fclose(sourceFile);
        return 1;
    }

    unsigned char buffer[63];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, sizeof(unsigned char), sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, sizeof(unsigned char), bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);

    printf("Cópia concluída com sucesso.\n");
    return 0;
}
