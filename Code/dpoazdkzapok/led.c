#include <stdio.h>

#ifdef _WIN32
    #include <Windows.h>
    #include <powrprof.h>
#elif __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
#endif

// Function to check LED status and optionally turn it off
void checkAndControlLED() {
    #ifdef _WIN32
        // Windows specific code to check and control LED
        SYSTEM_POWER_STATUS powerStatus;
        if (GetSystemPowerStatus(&powerStatus)) {
            if (powerStatus.ACLineStatus == AC_LINE_ONLINE) {
                printf("LED is powered on.\n");

                // Ask if the user wants to turn off the LED
                char response;
                printf("Do you want to turn off the LED? (y/n): ");
                scanf(" %c", &response);

                if (response == 'y' || response == 'Y') {
                    // Turn off the LED (placeholder code, actual implementation depends on hardware)
                    printf("LED is turned off.\n");
                }
            } else {
                printf("LED is not powered on.\n");
            }
        } else {
            fprintf(stderr, "Error getting system power status on Windows.\n");
            exit(EXIT_FAILURE);
        }
    #elif __linux__
        // Linux specific code to check and control LED
        FILE* fp = popen("cat /sys/class/leds/led0/trigger", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error reading LED trigger file on Linux.\n");
            exit(EXIT_FAILURE);
        }

        char trigger[256];
        fscanf(fp, "%s", trigger);

        // Check if the LED is powered on
        if (strcmp(trigger, "mmc0") == 0) {
            printf("LED is powered on.\n");

            // Ask if the user wants to turn off the LED
            char response;
            printf("Do you want to turn off the LED? (y/n): ");
            scanf(" %c", &response);

            if (response == 'y' || response == 'Y') {
                // Turn off the LED (placeholder code, actual implementation depends on hardware)
                FILE* fpOff = fopen("/sys/class/leds/led0/brightness", "w");
                if (fpOff != NULL) {
                    fprintf(fpOff, "0");
                    fclose(fpOff);
                    printf("LED is turned off.\n");
                } else {
                    fprintf(stderr, "Error writing to LED brightness file on Linux.\n");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            printf("LED is not powered on.\n");
        }

        pclose(fp);
    #endif
}
