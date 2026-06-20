import re

from django import forms

from apps.accounts.models import User

MAX_MESSAGE_LENGTH = 200

XSS_PATTERN = re.compile(
    r"(<.*?script.*?>.*?<.*?/.*?script.*?>)|(<.*?script.*?/.*?>)",
    re.IGNORECASE,
)


class MessageForm(forms.Form):
    """
    Form to validate message of the chat.
    """

    uuid = forms.UUIDField(required=False)
    message = forms.CharField(max_length=MAX_MESSAGE_LENGTH, required=True)
    author = forms.CharField(max_length=200, required=False)
    system = forms.BooleanField(initial=False, required=False)

    def clean_message(self):
        """
        Check that the message is valid.
        """
        message = self.cleaned_data["message"]
        if not message.strip():
            raise forms.ValidationError("Message cannot be empty.")
        if len(message) > MAX_MESSAGE_LENGTH:
            raise forms.ValidationError(
                f"Message must be {MAX_MESSAGE_LENGTH} characters or fewer.")
        if XSS_PATTERN.search(message):
            raise forms.ValidationError("Message contains invalid content.")
        return message

    def clean_uuid(self):
        """
        Check that the uuid is valid.
        """
        uuid = self.cleaned_data["uuid"]
        if uuid is None:
            return None
        return str(uuid)

    def clean_system(self):
        """
        Check that the system is valid.
        """
        system = self.cleaned_data["system"]
        if not isinstance(system, bool):
            raise forms.ValidationError("System flag must be boolean.")
        return system

    def clean(self):
        """
        Check that the message is valid.
        """
        cleaned_data = super().clean()
        author = cleaned_data.get("author")
        system = cleaned_data.get("system")

        if not author and not system:
            raise forms.ValidationError(
                "Author is required for non-system messages.")
        if author and not system and not User.objects.filter(
                username=author).exists():
            raise forms.ValidationError(f"Author '{author}' does not exist.")
        return cleaned_data
