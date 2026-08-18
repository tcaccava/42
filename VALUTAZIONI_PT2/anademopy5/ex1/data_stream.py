#!/usr/bin/env python3

import abc
import typing

EX_NAME = "Code Nexus - Data Stream"
SUBTEXT = "DataStream statistics"


class DataProcessor(abc.ABC):

    name: str
    label: str

    def __init__(self) -> None:
        self.index: int = 0
        self.processed_data: list[tuple[int, str]] = []

    def store(self, processed_data: str) -> None:
        self.processed_data.append((self.index, processed_data))
        self.index += 1

    def output(self) -> tuple[int, str]:
        return self.processed_data.pop(0)

    @abc.abstractmethod
    def validate(self, data: typing.Any) -> bool:
        pass

    @abc.abstractmethod
    def ingest(self, data: typing.Any) -> None:
        pass


class NumericProcessor(DataProcessor):

    name: str = "Numeric"
    label: str = "Numeric value"

    def is_numeric(self, data: typing.Any) -> bool:
        if isinstance(data, bool):
            return False
        return isinstance(data, (int, float))

    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, list):
            if len(data) == 0:
                return False
            for item in data:
                if not self.is_numeric(item):
                    return False
            return True
        return self.is_numeric(data)

    def ingest(self, data: int | float | list[int | float]) -> None:
        if not self.validate(data):
            raise ValueError("Improper numeric data")
        if isinstance(data, list):
            for item in data:
                self.store(str(item))
        else:
            self.store(str(data))


class TextProcessor(DataProcessor):

    name: str = "Text"
    label: str = "Text value"

    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, list):
            if len(data) == 0:
                return False
            for item in data:
                if not isinstance(item, str):
                    return False
            return True
        return isinstance(data, str)

    def ingest(self, data: str | list[str]) -> None:
        if not self.validate(data):
            raise ValueError("Improper text data")
        if isinstance(data, list):
            for item in data:
                self.store(item)
        else:
            self.store(data)


class LogProcessor(DataProcessor):

    name: str = "Log"
    label: str = "Log entry"

    def is_log(self, data: typing.Any) -> bool:
        if not isinstance(data, dict):
            return False
        if len(data) == 0:
            return False
        for key, value in data.items():
            if not isinstance(key, str):
                return False
            if not isinstance(value, str):
                return False
        return True

    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, list):
            if len(data) == 0:
                return False
            for item in data:
                if not self.is_log(item):
                    return False
            return True
        return self.is_log(data)

    def ingest(self,
               data: dict[str, str] | list[dict[str, str]]
               ) -> None:
        if not self.validate(data):
            raise ValueError("Improper log data")
        sep = ": "
        if isinstance(data, list):
            for item in data:
                self.store(sep.join(item.values()))
        else:
            self.store(sep.join(data.values()))


class DataStream:

    def __init__(self) -> None:
        self.processors: list[DataProcessor] = []

    def register_processor(self, proc: DataProcessor) -> None:
        self.processors.append(proc)

    def process_stream(self, stream: list[typing.Any]) -> None:
        for item in stream:
            taken = False
            for proc in self.processors:
                if proc.validate(item):
                    proc.ingest(item)
                    taken = True
                    break
            if not taken:
                print(f"DataStream error - Can't process element "
                      f"in stream: {item}")

    def print_processors_stats(self) -> None:
        print(f"== {SUBTEXT} ==")
        if not self.processors:
            print("No processor found, no data")
            return
        for proc in self.processors:
            total = proc.index
            left = len(proc.processed_data)
            print(f"{proc.name} Processor: total {total} items processed, "
                  f"remaining {left} on processor")


def banner() -> None:
    print(f"=== {EX_NAME} ===")


def test_consume(stream: DataStream,
                 plan: list[tuple[DataProcessor, int]]) -> None:
    parts = [f"{proc.name} {count}" for proc, count in plan]
    print("Consume some elements from the data processors: "
          + ", ".join(parts))
    for proc, count in plan:
        for _ in range(count):
            proc.output()
    stream.print_processors_stats()


def main() -> None:
    banner()
    print()
    print("Initialize Data Stream...")
    stream = DataStream()
    stream.print_processors_stats()

    numeric = NumericProcessor()
    print()
    print(f"Registering {numeric.name} Processor")
    stream.register_processor(numeric)

    batch: list[typing.Any] = [
        "Hello world",
        [3.14, -1, 2.71],
        [{"log_level": "WARNING",
          "log_message": "Telnet access! Use ssh instead"},
         {"log_level": "INFO",
          "log_message": "User wil is connected"}],
        42,
        ["Hi", "five"],
    ]
    print()
    print(f"Send first batch of data on stream: {batch}")
    stream.process_stream(batch)
    stream.print_processors_stats()

    text = TextProcessor()
    log = LogProcessor()
    print()
    print("Registering other data processors")
    stream.register_processor(text)
    stream.register_processor(log)
    print("Send the same batch again")
    stream.process_stream(batch)
    stream.print_processors_stats()

    consume_plan: list[tuple[DataProcessor, int]] = [
        (numeric, 3),
        (text, 2),
        (log, 1),
    ]
    print()
    test_consume(stream, consume_plan)


if __name__ == "__main__":
    main()
