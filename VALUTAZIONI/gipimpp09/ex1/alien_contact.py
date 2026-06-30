from datetime import datetime
from enum import Enum
from pydantic import BaseModel, Field, model_validator, ValidationError


class ContactType(str, Enum):
    radio = "radio"
    visual = "visual"
    physical = "physical"
    telepathic = "telepathic"


class AlienContact(BaseModel):
    contact_id: str = Field(min_length=5, max_length=15)
    timestamp: datetime
    location: str = Field(min_length=3, max_length=100)
    contact_type: ContactType
    signal_strength: float = Field(ge=0.0, le=10.0)
    duration_minutes: int = Field(ge=1, le=1440)
    witness_count: int = Field(ge=1, le=100)
    message_received: str | None = Field(default=None, max_length=500)
    is_verified: bool = False

    @model_validator(mode="after")
    def validate_contact(self) -> "AlienContact":
        if not self.contact_id.startswith("AC"):
            raise ValueError('Contact ID must start with "AC"')

        if self.contact_type == ContactType.physical and not self.is_verified:
            raise ValueError("Physical contact reports must be verified")

        if (self.contact_type == ContactType.telepathic and
                self.witness_count < 3):
            raise ValueError("Telepathic contact "
                             "requires at least 3 witnesses")

        if self.signal_strength > 7.0 and not self.message_received:
            raise ValueError("Strong signals (>7.0) "
                             "should include received messages")

        return self

    def info(self) -> None:
        print(f"ID: {self.contact_id}")
        print(f"Type: {self.contact_type.value}")
        print(f"Location: {self.location}")
        print(f"Signal: {self.signal_strength}/10")
        print(f"Duration: {self.duration_minutes} minutes")
        print(f"Witnesses: {self.witness_count}")
        if self.message_received:
            print(f"Message: '{self.message_received}'")


def main() -> None:
    print("Alien Contact Log Validation")
    print("=" * 40)

    try:
        valid = AlienContact(
            contact_id="AC_2024_001",
            timestamp=datetime.fromisoformat("2024-01-15T08:30:00"),
            location="Area 51, Nevada",
            contact_type=ContactType.telepathic,
            signal_strength=8.5,
            duration_minutes=45,
            witness_count=6,
            message_received="a",
            is_verified = True
        )
        print("Valid contact report:")
        valid.info()
    except ValidationError as e:
        print(f"Unexpected error on valid report:\n{e}")

    print("=" * 40)
    print("Expected validation error:")
    try:
        AlienContact(
            contact_id="AC_2024_002",
            timestamp=datetime.fromisoformat("2024-01-15T08:30:00"),
            location="Roswell, New Mexico",
            contact_type=ContactType.telepathic,
            signal_strength=5.0,
            duration_minutes=10,
            witness_count=1,
            message_received=None,
        )
    except ValidationError as e:
        for error in e.errors():
            print(error["msg"])


if __name__ == "__main__":
    main()
