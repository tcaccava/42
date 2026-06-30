from datetime import datetime
from pydantic import BaseModel, Field, ValidationError


class SpaceStation(BaseModel):
    station_id: str = Field(min_length=3, max_length=10)
    name: str = Field(min_length=1, max_length=50)
    crew_size: int = Field(ge=1, le=20)
    power_level: float = Field(ge=0.0, le=100.0)
    oxygen_level: float = Field(ge=0.0, le=100.0)
    last_maintenance: datetime
    is_operational: bool = True
    notes: str | None = Field(default=None, max_length=200)

    def info(self) -> None:
        print("========================================")
        print("Valid station created:")
        print(f"ID: {self.station_id}")
        print(f"Name: {self.name}")
        print(f"Crew: {self.crew_size} people")
        print(f"Power: {self.power_level}%")
        print(f"Oxygen: {self.oxygen_level}%")
        status = "Operational" if self.is_operational else "Not Operational"
        print(f"Status: {status}")
        print("========================================")


def main() -> None:
    print("Space Station Data Validation")

    try:
        valid = SpaceStation(
            station_id="ISS001",
            name="International Space Station",
            crew_size=6,
            power_level=85.5,
            oxygen_level=92.3,
            last_maintenance=datetime.fromisoformat("2026-06-30T12:00:00")
        )
        valid.info()
    except ValidationError as e:
        print(f"Unexpected error on valid station:\n{e}")

    print("\nExpected validation error:")
    try:
        SpaceStation(
            station_id="ISS001",
            name="International Space Station",
            crew_size=25,
            power_level=85.5,
            oxygen_level=92.3,
            last_maintenance=datetime.fromisoformat("2026-09-01T10:00:00")
        )
    except ValidationError as e:
        for error in e.errors():
            print(error["msg"])


if __name__ == "__main__":
    main()
