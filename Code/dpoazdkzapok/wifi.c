#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#elif __linux__
#include <dirent.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/if_arp.h>
#elif __APPLE__
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if_dl.h>
#endif

void printWiFiInfo(const char* interface, const char* macAddress, const char* model) {
    printf("  WiFi Interface: %s\n", interface);
    printf("    MAC Address: %s\n", macAddress);
    printf("    Model: %s", model);
    // Add more details as needed
}

void getWiFiInfo() {
#ifdef _WIN32
    PIP_ADAPTER_ADDRESSES adapterAddresses = NULL;
    PIP_ADAPTER_ADDRESSES adapter = NULL;
    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;

    printf("WiFi Information:\n");

    // Get adapter addresses
    adapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(sizeof(IP_ADAPTER_ADDRESSES));
    outBufLen = sizeof(IP_ADAPTER_ADDRESSES);

    if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapterAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(adapterAddresses);
        adapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
    }

    if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapterAddresses, &outBufLen) == NO_ERROR) {
        adapter = adapterAddresses;
        while (adapter) {
            if (adapter->IfType == IF_TYPE_ETHERNET_CSMACD || adapter->IfType == IF_TYPE_IEEE80211) {
                char macAddress[18];
                sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X",
                        adapter->PhysicalAddress[0], adapter->PhysicalAddress[1],
                        adapter->PhysicalAddress[2], adapter->PhysicalAddress[3],
                        adapter->PhysicalAddress[4], adapter->PhysicalAddress[5]);

                printWiFiInfo(adapter->AdapterName, macAddress, adapter->Description);
            }

            adapter = adapter->Next;
        }
    }

    free(adapterAddresses);
#elif __linux__
    DIR* dir;
    struct dirent* entry;
    char buffer[1024];

    printf("WiFi Information:\n");

    // Open the /sys/class/net directory
    dir = opendir("/sys/class/net");
    if (dir == NULL) {
        fprintf(stderr, "Error opening /sys/class/net directory on Linux.\n");
        return;
    }

    // Iterate over entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            FILE* fp;

            // Build the path to the wireless directory
            snprintf(buffer, sizeof(buffer), "/sys/class/net/%s/type", entry->d_name);

            // Check if it is a wireless interface
            fp = fopen(buffer, "r");
            if (fp != NULL) {
                int type;
                if (fscanf(fp, "%d", &type) == 1 && type == ARPHRD_IEEE80211) {
                    char macAddress[18];
                    sprintf(macAddress, "%s", "Not Available"); // For Linux, MAC address retrieval may vary

                    // Get the WiFi card model information
                    snprintf(buffer, sizeof(buffer), "lspci | grep Network");
                    fp = popen(buffer, "r");
                    char model[1024];
                    if (fp != NULL) {
                        fgets(model, sizeof(model) - 1, fp);
                        pclose(fp);
                    } else {
                        sprintf(model, "Not Available");
                    }

                    printWiFiInfo(entry->d_name, macAddress, model);
                }
                fclose(fp);
            }
        }
    }

    closedir(dir);
#elif __APPLE__
    int mib[6];
    size_t len;
    char* buf;

    printf("WiFi Information:\n");

    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;

    // Get the number of interfaces
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        fprintf(stderr, "Error retrieving WiFi information on macOS.\n");
        return;
    }

    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        fprintf(stderr, "Error retrieving WiFi information on macOS.\n");
        return;
    }

    buf = (char*)malloc(len);
    if (buf == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return;
    }

    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        fprintf(stderr, "Error retrieving WiFi information on macOS.\n");
        free(buf);
        return;
    }

    struct if_msghdr* ifm = (struct if_msghdr*)buf;
    struct sockaddr_dl* sdl = (struct sockaddr_dl*)(ifm + 1);
    unsigned char* mac = (unsigned char*)LLADDR(sdl);

    char macAddress[18];
    sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Run the system_profiler command to get WiFi card model information
    FILE* fp = popen("system_profiler SPAirPortDataType | awk '/Card Type/ {getline; print}'", "r");
    char model[1024];
    if (fp != NULL) {
        fgets(model, sizeof(model) - 1, fp);
        pclose(fp);
    } else {
        sprintf(model, "Not Available");
    }

    printWiFiInfo(if_indextoname(ifm->ifm_index, sdl->sdl_data), macAddress, model);

    free(buf);
#endif
}
