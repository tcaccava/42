import abc
import typing


class DataProcessor(abc.ABC):
    def __init__(self) -> None:
        self._data: list[str] = []
        self._total_processed: int = 0

    @abc.abstractmethod
    def validate(self, data: typing.Any) -> bool:
        pass

    @abc.abstractmethod
    def ingest(self, data: typing.Any) -> None:
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
    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, (int, float)):
            return True

        if isinstance(data, list):
            return all(
                isinstance(item, (int, float))
                for item in data
            )

        return False

    def ingest(
        self,
        data: int | float | list[int | float]
    ) -> None:
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
    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, str):
            return True

        if isinstance(data, list):
            return all(
                isinstance(item, str)
                for item in data
            )

        return False

    def ingest(
        self,
        data: str | list[str]
    ) -> None:
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
    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, dict):
            return all(
                isinstance(key, str)
                and isinstance(value, str)
                for key, value in data.items()
            )

        if isinstance(data, list):
            return all(
                isinstance(item, dict)
                and all(
                    isinstance(key, str)
                    and isinstance(value, str)
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


# ============================================================
# Export plugin interface
# ============================================================

class ExportPlugin(typing.Protocol):
    def process_output(
        self,
        data: list[tuple[int, str]]
    ) -> None:
        ...


# ============================================================
# CSV Plugin
# ============================================================

class CSVExportPlugin:
    def process_output(
        self,
        data: list[tuple[int, str]]
    ) -> None:
        values: list[str] = []

        for _, value in data:
            values.append(self._escape_csv(value))

        print("CSV Output:")
        print(",".join(values))

    def _escape_csv(self, value: str) -> str:
        if (
            "," in value
            or '"' in value
            or "\n" in value
        ):
            value = value.replace('"', '""')
            return f'"{value}"'

        return value


# ============================================================
# JSON Plugin
# ============================================================

class JSONExportPlugin:
    def process_output(
        self,
        data: list[tuple[int, str]]
    ) -> None:
        items: list[str] = []

        for rank, value in data:
            escaped = self._escape_json(value)
            items.append(f'"item_{rank}": "{escaped}"')

        result = "{"
        result += ", ".join(items)
        result += "}"

        print("JSON Output:")
        print(result)

    def _escape_json(self, value: str) -> str:
        value = value.replace("\\", "\\\\")
        value = value.replace('"', '\\"')
        value = value.replace("\n", "\\n")
        value = value.replace("\r", "\\r")
        value = value.replace("\t", "\\t")

        return value


# ============================================================
# DataStream
# ============================================================

class DataStream:
    def __init__(self) -> None:
        self._processors: list[DataProcessor] = []

    def register_processor(
        self,
        proc: DataProcessor
    ) -> None:
        self._processors.append(proc)

    def process_stream(
        self,
        stream: list[typing.Any]
    ) -> None:
        for element in stream:
            processed = False

            for processor in self._processors:
                if processor.validate(element):
                    processor.ingest(element)
                    processed = True
                    break

            if not processed:
                print(
                    "DataStream error - Can't process "
                    f"element in stream: {element}"
                )

    def output_pipeline(
        self,
        nb: int,
        plugin: ExportPlugin
    ) -> None:
        for processor in self._processors:
            output_data: list[tuple[int, str]] = []

            for _ in range(nb):
                if processor.get_remaining() == 0:
                    break

                output_data.append(processor.output())

            if output_data:
                plugin.process_output(output_data)

    def print_processors_stats(self) -> None:
        print("== DataStream statistics ==")

        if not self._processors:
            print("No processor found, no data")
            return

        for processor in self._processors:
            name = (
                processor.__class__.__name__
                .replace("Processor", " Processor")
            )

            print(
                f"{name}: total "
                f"{processor.get_total_processed()} items processed, "
                f"remaining {processor.get_remaining()} on processor"
            )


# ============================================================
# Test
# ============================================================

def main() -> None:
    print("=== Code Nexus - Data Pipeline ===")
    print()

    print("Initialize Data Stream...")
    data_stream = DataStream()

    print()
    data_stream.print_processors_stats()

    # --------------------------------------------------------
    # Register processors
    # --------------------------------------------------------

    print()
    print("Registering Processors")

    numeric = NumericProcessor()
    text = TextProcessor()
    log = LogProcessor()

    data_stream.register_processor(numeric)
    data_stream.register_processor(text)
    data_stream.register_processor(log)

    # --------------------------------------------------------
    # First batch
    # --------------------------------------------------------

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

    print()
    data_stream.print_processors_stats()

    # --------------------------------------------------------
    # CSV export
    # --------------------------------------------------------

    print()
    print(
        "Send 3 processed data from each processor "
        "to a CSV plugin:"
    )

    csv_plugin = CSVExportPlugin()

    data_stream.output_pipeline(
        3,
        csv_plugin
    )

    print()
    data_stream.print_processors_stats()

    # --------------------------------------------------------
    # Second batch
    # --------------------------------------------------------

    batch2 = [
        21,
        ["I love AI", "LLMs are wonderful", "Stay healthy"],
        [
            {
                "log_level": " ERROR",
                "log_message": "500 server crash"
            },
            {
                "log_level": "NOTICE",
                "log_message": "Certificate expires in 10 days"
            }
        ],
        [32, 42, 64, 84, 128, 168],
        "World hello"
    ]

    print()
    print("Send another batch of data:", batch2)

    data_stream.process_stream(batch2)

    print()
    data_stream.print_processors_stats()

    # --------------------------------------------------------
    # JSON export
    # --------------------------------------------------------

    print()
    print(
        "Send 5 processed data from each processor "
        "to a JSON plugin:"
    )

    json_plugin = JSONExportPlugin()

    data_stream.output_pipeline(
        5,
        json_plugin
    )

    print()
    data_stream.print_processors_stats()


if __name__ == "__main__":
    main()
