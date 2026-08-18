#!/usr/bin/env python3

import abc
import typing

EX_NAME = "Code Nexus - Data Processor"


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


def banner() -> None:
    print(f"=== {EX_NAME} ===")


def sub_banner(proc: DataProcessor) -> None:
    print(f"Testing {proc.name} Processor...")


def test_validator(proc: DataProcessor, data: typing.Any) -> None:
    print(f" Trying to validate input '{data}': {proc.validate(data)}")


def test_invalid(proc: NumericProcessor, data: str) -> None:
    print(f" Test invalid ingestion of string '{data}'", end="")
    print(" without prior validation:")
    try:
        proc.ingest(data)  # type: ignore[arg-type]
    except Exception as error:
        print(f" Got exception: {error}")


def test_processor(proc: DataProcessor, data: typing.Any, i: int) -> None:
    print(f" Processing data: {data}")
    proc.ingest(data)
    if i == 1:
        word = "value"
    else:
        word = "values"
    print(f" Extracting {i} {word}...")
    for _ in range(i):
        rank, value = proc.output()
        print(f" {proc.label} {rank}: {value}")


def main() -> None:
    banner()

    print()
    numeric = NumericProcessor()
    sub_banner(numeric)
    test_validator(numeric, 42)
    test_validator(numeric, "Hello")
    test_invalid(numeric, "foo")
    test_processor(numeric, [1, 2, 3, 4, 5], 3)

    print()
    text = TextProcessor()
    sub_banner(text)
    test_validator(text, 42)
    test_processor(text, ["Hello", "Nexus", "World"], 1)

    print()
    log = LogProcessor()
    sub_banner(log)
    test_validator(log, "Hello")
    log_batch: list[dict[str, str]] = [
        {"log_level": "NOTICE", "log_message": "Connection to server"},
        {"log_level": "ERROR", "log_message": "Unauthorized access!!"},
    ]
    test_processor(log, log_batch, 2)


if __name__ == "__main__":
    main()
