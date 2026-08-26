from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    def __init__(self) -> None:
        self._data: list[str] = []
        self._total_processed: int = 0

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @abstractmethod
    def ingest(self, data: Any) -> None:
        pass

    def output(self) -> tuple[int, str]:
        if not self._data:
            raise IndexError("No data available")

        rank = self._total_processed - len(self._data)
        value = self._data.pop(0)

        return rank, value

    def get_total_processed(self) -> int:
        return self._total_processed

    def get_remaining(self) -> int:
        return len(self._data)


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
                self._total_processed += 1
        else:
            self._data.append(str(data))
            self._total_processed += 1


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
            for item in data:
                self._data.append(item)
                self._total_processed += 1
        else:
            self._data.append(data)
            self._total_processed += 1


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
                self._total_processed += 1
        else:
            self._data.append(self._format_log(data))
            self._total_processed += 1

    def _format_log(self, data: dict[str, str]) -> str:
        level = data.get("log_level", "").strip()
        message = data.get("log_message", "").strip()

        return f"{level}: {message}"


class DataStream:
    def __init__(self) -> None:
        self._processors: list[DataProcessor] = []

    def register_processor(self, proc: DataProcessor) -> None:
        self._processors.append(proc)

    def process_stream(self, stream: list[Any]) -> None:
        for element in stream:
            processed = False

            for processor in self._processors:
                if processor.validate(element):
                    processor.ingest(element)
                    processed = True
                    break

            if not processed:
                print(
                    "DataStream error - Can't process element in stream:",
                    element
                )

    def print_processors_stats(self) -> None:
        print("== DataStream statistics ==")

        if not self._processors:
            print("No processor found, no data")
            return

        for processor in self._processors:
            name = processor.__class__.__name__.replace(
                "Processor", " Processor"
                )

            print(
                f"{name}: total "
                f"{processor.get_total_processed()} items processed, "
                f"remaining {processor.get_remaining()} on processor"
            )


def main() -> None:
    print("=== Code Nexus - Data Stream ===")
    print()

    print("Initialize Data Stream...")
    data_stream = DataStream()

    data_stream.print_processors_stats()

    # -------------------------
    # Numeric Processor
    # -------------------------

    print()
    print("Registering Numeric Processor")

    numeric = NumericProcessor()
    data_stream.register_processor(numeric)

    batch = [
        "Hello world",
        [3.14, -1, 2.71],
        [
            {
                "log_level": "WARNING",
                " log_message": "Telnet access! Use ssh instead"
            },
            {
                "log_level": "INFO",
                "log_message": "User wil is connected"
            }
        ],
        42,
        ["Hi", "five"]
    ]

    print()
    print("Send first batch of data on stream:", batch)

    data_stream.process_stream(batch)
    data_stream.print_processors_stats()

    # -------------------------
    # Other processors
    # -------------------------

    print()
    print("Registering other data processors")

    text = TextProcessor()
    log = LogProcessor()

    data_stream.register_processor(text)
    data_stream.register_processor(log)

    print("Send the same batch again")

    data_stream.process_stream(batch)
    data_stream.print_processors_stats()

    # -------------------------
    # Consume data
    # -------------------------

    print()
    print(
        "Consume some elements from the data processors: "
        "Numeric 3, Text 2, Log 1"
    )

    for _ in range(3):
        numeric.output()

    for _ in range(2):
        text.output()

    for _ in range(1):
        log.output()

    data_stream.print_processors_stats()


if __name__ == "__main__":
    main()
