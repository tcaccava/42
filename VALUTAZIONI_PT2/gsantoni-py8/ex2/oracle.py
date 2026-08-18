import dotenv
from os import popen, getenv


if __name__ == "__main__":
    print("ORACLE STATUS: Reading the Matrix...")
    reading: bool = dotenv.load_dotenv()

    print("Configuration loaded:")

    matrix_mode: str | None = getenv("MATRIX_MODE")
    database_url: str | None = getenv("DATABASE_URL")
    api_key: str | None = getenv("API_KEY")
    log_level: str | None = getenv("LOG_LEVEL")
    zion_endpoint: str | None = getenv("ZION_ENDPOINT")

    if matrix_mode:
        print(f"Mode: {matrix_mode}")
    else:
        print("Mode: Unspecified")

    if database_url:
        if matrix_mode == "development":
            print("Database: Connected to local instance")
        elif matrix_mode == "production":
            print("Database: Connected to remote instance")
        else:
            print("Database: Connected to unspecified instance")
    else:
        print("Database: Not connected")

    if api_key:
        print("API Access: Authenticated")
    else:
        print("API Access: Not authenticated")

    if log_level:
        print(f"Log Level: {log_level}")
    else:
        print("Log Level: None")

    if zion_endpoint:
        print("Zion Network: Online\n")
    else:
        print("Zion Network: Offline\n")

    with open(__file__) as f:
        text: str = f.read()
        secret: bool = True
        for x in [api_key, database_url]:
            if x is None:
                pass
            elif x in text:
                secret = False
        if secret is True:
            print("[OK] No hardcoded secrets detected")
        else:
            print("[ERROR] Hardcoded secrets detected")
        f.close()

    if reading is True:
        print("[OK] .env file properly configured")
    else:
        print("[ERROR] .env file missing or incomplete")

    with popen("grep -rn 'load_dotenv(' | tail -2") as p:
        search: str = p.read()
    if "override=True" in search:
        print("[ERROR] Production overrides unavailable")
    else:
        print("[OK] Production overrides available")

    print("\nThe Oracle sees all configurations")
