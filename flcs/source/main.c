/*---------------------------------------------------------------------------------
    DS Application with Embedded Metadata and a Loading Progress Bar
---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------
// Simplified DS ROM Header structure (metadata)
// (A full DS header is 512 bytes; this is a minimal example for demo)
//---------------------------------------------------------------------
typedef struct {
    char title[12];              // Game title (12 characters -- pad as needed)
    char gameCode[4];            // Game code (4 characters)
    char makerCode[2];           // Maker code (2 characters)
    unsigned char unitCode;      // Usually 0x00 for DS
    unsigned char encryptionConstant; // Typically 0x00
    unsigned char deviceType;    // Usually 0x00
    unsigned char deviceCapacity;// 0x00 if unknown
    unsigned char reserved[9];   // Reserved bytes (set to 0)
    /* ... Additional fields required for a real header ... */
} DSHeader;

// Embed our metadata header in its own section (".ndsHeader")
// so that it resides at the beginning of the ROM image.
__attribute__((section(".ndsHeader")))
const DSHeader ndsHeader = {
    .title = "Lite Tatra",      // Exactly 12 characters (pad with spaces if needed)
    .gameCode = "LTTL",          // 4-character game code
    .makerCode = "01",           // 2-character maker code
    .unitCode = 0x00,
    .encryptionConstant = 0x00,
    .deviceType = 0x00,
    .deviceCapacity = 0x00,
    .reserved = {0}
};

//---------------------------------------------------------------------
// DS Application Code with a Looping Loading Progress Bar
//---------------------------------------------------------------------
#define BAR_WIDTH 24   // Width of the progress bar in characters

int main(int argc, char* argv[]) {
    int progress, i, j;

    // Initialize the DS console for text output
    consoleDemoInit();

    iprintf("Tatralandia Channel Distribution Box\n");
    iprintf("Not to be reproduced\n\n");

    // Simulate a loading process from 0% to 100%
    for (progress = 0; progress <= 100; progress++) {
        consoleClear();
        iprintf("Loading, please wait...\n\n");
        iprintf("+------------------------+\n");
        iprintf("| ");

        // Calculate number of blocks to fill based on progress (BAR_WIDTH characters)
        int blocks = (progress * BAR_WIDTH) / 100;
        for (i = 0; i < blocks; i++) {
            iprintf("#");
        }
        for (; i < BAR_WIDTH; i++) {
            iprintf(" ");
        }

        // Display the percentage and print the bottom border
        iprintf(" |   %3d%%\n", progress);
        iprintf("+------------------------+\n");

        // Added delay to simulate loading (approx. 0.66 sec delay at 60Hz)
        for (j = 0; j < 40; j++) {
            swiWaitForVBlank();
        }
    }

    // After loading completes, prompt user with a completion message
    consoleClear();
    iprintf("Loading complete!\n");
    iprintf("Press START to continue...\n");

    // Wait for the user to press the START key.
    while(pmMainLoop()) {
        scanKeys();
        int pressed = keysDown();
        if (pressed & KEY_START) break;
        swiWaitForVBlank();
    }

    return 0;
}
