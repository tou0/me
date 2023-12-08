#include <stdio.h>
#include <stdlib.h>

// Function declarations for hardware information retrieval
void getCPUInfo();
void getRAMInfo();
void getStorageInfo();
void getGPUInfo();
void checkAndControlLED();
void getWiFiInfo();
void getUsagePercentage();
void getScreenInfo();

int main() {
    // Call functions to get and display hardware information
    getCPUInfo();
    getRAMInfo();
    getStorageInfo();
    getGPUInfo();
    checkAndControlLED();
    getWiFiInfo();
    getUsagePercentage();
    getScreenInfo();

    return 0;
}
