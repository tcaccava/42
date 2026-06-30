from datetime import datetime
from enum import Enum
from pydantic import BaseModel, Field, model_validator, ValidationError


class Rank(str, Enum):
    cadet = "cadet"
    officer = "officer"
    lieutenant = "lieutenant"
    captain = "captain"
    commander = "commander"


class CrewMember(BaseModel):
    member_id: str = Field(min_length=3, max_length=10)
    name: str = Field(min_length=2, max_length=50)
    rank: Rank
    age: int = Field(ge=18, le=80)
    specialization: str = Field(min_length=3, max_length=30)
    years_experience: int = Field(ge=0, le=50)
    is_active: bool = True


class SpaceMission(BaseModel):
    mission_id: str = Field(min_length=5, max_length=15)
    mission_name: str = Field(min_length=3, max_length=100)
    destination: str = Field(min_length=3, max_length=50)
    launch_date: datetime
    duration_days: int = Field(ge=1, le=3650)
    crew: list[CrewMember]
    mission_status: str | None = "planned"
    budget_millions: float = Field(ge=1.0, le=10000.0)

    @model_validator(mode='after')
    def validation(self) -> "SpaceMission":
        if self.mission_id[0] != 'M':
            raise ValueError("Mission ID must start 'M'")
        has_commander_or_captain = any(
           e.rank in (Rank.captain, Rank.commander) for e in self.crew
        )
        if not has_commander_or_captain:
            raise ValueError("Mission must have at least"
                             " one Commander or Captain")
        if self.duration_days > 365:
            experts = [e for e in self.crew if e.years_experience >= 5]
            if len(experts) < len(self.crew)/2:
                raise ValueError("Long mission need 50% experienced crew")
        if not all(e.is_active for e in self.crew):
            raise ValueError("All crew members must be active")
        return self

    def info(self) -> None:
        print(f"Mission: {self.mission_name}")
        print(f"ID: {self.mission_id}")
        print(f"Destination: {self.destination}")
        print(f"Duration: {self.duration_days} days")
        print(f"Budget: ${self.budget_millions}M")
        print(f"Crew size: {len(self.crew)}")
        print("Crew members:")
        for member in self.crew:
            print(f"- {member.name}"
                  f" ({member.rank.value})"
                  f" - {member.specialization}")


def main() -> None:
    print("Space Mission Crew Validation")
    print("=" * 41)

    try:
        valid = SpaceMission(
            mission_id="M2024_MARS",
            mission_name="Mars Colony Establishment",
            destination="Mars",
            launch_date=datetime.fromisoformat("2026-09-01T10:00:00"),
            duration_days=900,
            budget_millions=2500.0,
            crew=[
                CrewMember(
                    member_id="C001",
                    name="Sarah Connor",
                    rank=Rank.commander,
                    age=45,
                    specialization="Mission Command",
                    years_experience=20,
                ),
                CrewMember(
                    member_id="C002",
                    name="John Smith",
                    rank=Rank.lieutenant,
                    age=34,
                    specialization="Navigation",
                    years_experience=8,
                ),
                CrewMember(
                    member_id="C003",
                    name="Alice Johnson",
                    rank=Rank.officer,
                    age=29,
                    specialization="Engineering",
                    years_experience=5,
                ),
            ],
        )
        print("Valid mission created:")
        valid.info()
    except ValidationError as e:
        print(f"Unexpected error on valid mission:\n{e}")

    print("=" * 41)
    print("Expected validation error:")
    try:
        SpaceMission(
            mission_id="M2024_FAIL",
            mission_name="Doomed Expedition",
            destination="Europa",
            launch_date=datetime.fromisoformat("2026-09-01T10:00:00"),
            duration_days=200,
            budget_millions=500.0,
            crew=[
                CrewMember(
                    member_id="C004",
                    name="Bob Lee",
                    rank=Rank.cadet,
                    age=22,
                    specialization="Logistics",
                    years_experience=1,
                ),
            ],
        )
    except ValidationError as e:
        for error in e.errors():
            print(error["msg"])


if __name__ == "__main__":
    main()
