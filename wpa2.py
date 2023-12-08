import itertools
import hashlib

def generate_passwords(charset, length):
    return (''.join(candidate) for candidate in itertools.product(charset, repeat=length))

def crack_wpa2(target_hash, dictionary_path):
    with open(dictionary_path, 'r', errors='ignore') as dictionary:
        for password in dictionary:
            password = password.strip()
            hashed_password = hashlib.sha256(password.encode()).hexdigest()

            if hashed_password == target_hash:
                return f"Password found: {password}"

    return "Password not found. Keep dreaming, loser."

# Example usage:
target_hash = "YourWPA2HashHere"
dictionary_path = "YourDictionaryPathHere.txt"

result = crack_wpa2(target_hash, dictionary_path)
print(result)
