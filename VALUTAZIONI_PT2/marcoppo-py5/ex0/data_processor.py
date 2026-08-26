from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    def __init__(self) -> None:
        self._data: list[str] = []

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @abstractmethod
    def ingest(self, data: Any) -> None:
        pass

    def output(self) -> tuple[int, str]:
        if not self._data:
            raise IndexError("No data available")

        rank = 0
        value = self._data.pop(0)

        return rank, value


class NumericProcessor(DataProcessor):
    def validate(self, data: Any) -> bool:
        if isinstance(data, (int, float)):
            return True

        if isinstance(data, list):
            return all(isinstance(item, (int, float)) for item in data)

        return False

    def ingest(self, data: int | float | list[int | float]) -> None:
        if not self.validate(data):
            raise TypeError("Improper numeric data")

        if isinstance(data, list):
            for item in data:
                self._data.append(str(item))
        else:
            self._data.append(str(data))


class TextProcessor(DataProcessor):
    def validate(self, data: Any) -> bool:
        if isinstance(data, str):
            return True

        if isinstance(data, list):
            return all(isinstance(item, str) for item in data)

        return False

    def ingest(self, data: str | list[str]) -> None:
        if not self.validate(data):
            raise TypeError("Improper text data")

        if isinstance(data, list):
            self._data.extend(data)
        else:
            self._data.append(data)


class LogProcessor(DataProcessor):
    def validate(self, data: Any) -> bool:
        if isinstance(data, dict):
            return all(
                isinstance(key, str) and isinstance(value, str)
                for key, value in data.items()
            )

        if isinstance(data, list):
            return all(
                isinstance(item, dict)
                and all(
                    isinstance(key, str) and isinstance(value, str)
                    for key, value in item.items()
                )
                for item in data
            )

        return False

    def ingest(
        self,
        data: dict[str, str] | list[dict[str, str]]
    ) -> None:
        if not self.validate(data):
            raise TypeError("Improper log data")

        if isinstance(data, list):
            for item in data:
                self._data.append(self._format_log(item))
        else:
            self._data.append(self._format_log(data))

    def _format_log(self, data: dict[str, str]) -> str:
        level = data.get("log_level", "").strip()
        message = data.get("log_message", "")

        return f"{level}: {message}"


def main() -> None:
    print("=== Code Nexus - Data Processor ===")

    # -------------------------
    # Numeric Processor
    # -------------------------
    print("\nTesting Numeric Processor...")

    numeric = NumericProcessor()

    print("Trying to validate input '42':", numeric.validate(42))
    print("Trying to validate input 'Hello':", numeric.validate("Hello"))

    print("Test invalid ingestion of string 'foo' without prior validation:")
    try:
        # Questo è volutamente invalido.
        # type checker come mypy segnaleranno un errore.
        numeric.ingest("foo")  # type: ignore[arg-type]
    except TypeError as error:
        print("Got exception:", error)

    print("Processing data: [1, 2, 3, 4, 5]")
    numeric.ingest([1, 2, 3, 4, 5])

    print("Extracting 3 values...")
    for i in range(3):
        rank, value = numeric.output()
        print(f"Numeric value {rank}: {value}")

    # -------------------------
    # Text Processor
    # -------------------------
    print("\nTesting Text Processor...")

    text = TextProcessor()

    print("Trying to validate input '42':", text.validate(42))

    print("Processing data: ['Hello', 'Nexus', 'World']")
    text.ingest(["Hello", "Nexus", "World"])

    print("Extracting 1 value...")
    rank, value = text.output()
    print(f"Text value {rank}: {value}")

    # -------------------------
    # Log Processor
    # -------------------------
    print("\nTesting Log Processor...")

    log = LogProcessor()

    print("Trying to validate input 'Hello':", log.validate("Hello"))

    logs = [
        {
            "log_level": "NOTICE",
            "log_message": "Connection to server",
        },
        {
            "log_level": "ERROR ",
            "log_message": "Unauthorized access!!",
        },
    ]

    print("Processing data:", logs)
    log.ingest(logs)

    print("Extracting 2 values...")
    for i in range(2):
        rank, value = log.output()
        print(f"Log entry {rank}: {value}")


if __name__ == "__main__":
    main()
