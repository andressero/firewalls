import random
import string
import hashlib
import base64
import os

def generate_random_key(min_length=12, max_length=20):
    length = random.randint(min_length, max_length)
    characters = string.ascii_letters + string.digits
    key = ''.join(random.choice(characters) for _ in range(length))
    return key

def sha256_hash(data):
    return hashlib.sha256(data.encode()).hexdigest()

def generate_salt(length=15):
    random_bytes = os.urandom(length)
    return base64.b64encode(random_bytes).decode('utf-8').rstrip('=')

def main():
    # Generate a random key
    key = "adMin99" #generate_random_key()
    print(f"KEY: {key}")

    # Compute the SHA256 hash of the key
    key_hash = sha256_hash(key)
    print(f"HASH(KEY): {key_hash}")

    # Generate a random salt
    salt = generate_salt()
    print(f"SALT: {salt}")

    # Compute the SHA256 hash of the concatenation of HASH(KEY) and SALT
    combined_hash = sha256_hash(key_hash + salt)
    print(f"HASH(HASH(KEY) + SALT): {combined_hash}")

if __name__ == "__main__":
    main()
