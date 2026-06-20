import json

from apps.accounts.models import User
from apps.core.forms import MessageForm
from apps.core.models import Message
from apps.core.decorators import retry
from asgiref.sync import sync_to_async
from channels.db import database_sync_to_async
from channels.generic.websocket import AsyncWebsocketConsumer
from detoxify import Detoxify
from django.conf import settings
from django.utils import timezone

_toxicity_model = None


def _get_toxicity_model():
    global _toxicity_model
    if _toxicity_model is None:
        _toxicity_model = Detoxify(settings.TOXICITY_MODEL)
    return _toxicity_model


def check_profanity(text: str) -> bool:
    results = _get_toxicity_model().predict(text)
    return results["toxicity"] > settings.TOXICITY_THRESHOLD


class GlobalChatConsumer(AsyncWebsocketConsumer):
    """Handle global chat WebSocket connections."""
    GROUP_NAME = "global_chat"
    MAX_MESSAGE = 50

    def __init__(self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        self.connected = False
        self.user: User | None = None

    @retry(3)
    async def connect(self) -> None:
        """Accept connection, load history, and broadcast join message."""
        try:
            user_id = self.scope['session'].get('user_id')
            if user_id:
                self.user = await sync_to_async(
                    User.objects.filter(id=user_id).first)()

            if not await self._check_authenticated():
                await self.accept()
                await self.close(code=1000)
                return

            self.connected = True
            await self.channel_layer.group_add(self.GROUP_NAME,
                                               self.channel_name)
            await self.accept()

            # Send old messages to client
            messages = await database_sync_to_async(Message.latest_for_chat
                                                    )(self.MAX_MESSAGE)
            for message in messages:
                await self.send_message_to_client(message)

            await self.send_system_message_to_group(
                f"{self.user.username} has joined the chat.")  # type: ignore
        except Exception as e:
            await self.send_general_error()
            await self.close()

    @retry(3)
    async def disconnect(self, close_code: int = 1000) -> None:
        """Remove from group and broadcast leave message."""
        try:
            if self.user and self.connected:
                # Send message to room group that a user has left
                await self.send_system_message_to_group(
                    f"{self.user.username} has left the chat.")

            # Leave room group
            await self.channel_layer.group_discard(self.GROUP_NAME,
                                                   self.channel_name)

        except Exception as e:
            await self.send_general_error()
        finally:
            self.connected = False
            await self.close(code=close_code)

    @retry(3)
    async def receive(self, text_data: str) -> None:
        """Receive and broadcast message from WebSocket client."""
        if not self.connected or not await self._check_user_permission():
            return

        try:
            text_data_json = json.loads(text_data)
            if "message" not in text_data_json:
                await self.send_system_message_to_client(
                    "Invalid message format.", error=True)
                return

            valid, cleaned_data = await self.message_form(
                message=text_data_json["message"],
                author=self.user.username)  # type: ignore

            if valid:

                # check for profanity
                if await sync_to_async(check_profanity)(
                    cleaned_data["message"]
                ):
                    await self._handle_profanity_warning()
                    return

                message = await self.create_message(cleaned_data["message"])
                if message:
                    await self.channel_layer.group_send(
                        self.GROUP_NAME,
                        await self._build_message_data(message),
                    )
            else:
                await self.send_system_message_to_client(cleaned_data["error"],
                                                         error=True)
        except Exception as e:
            await self.send_general_error()

    @retry(3)
    async def send_message_to_client(self, message: Message) -> None:
        """Send message to client WebSocket with author details."""
        try:
            data = await self._build_message_data(message)
            await self.send(text_data=json.dumps(data))
        except Exception as e:
            await self.send_general_error()

    @retry(3)
    async def send_system_message_to_client(self,
                                            message: str,
                                            error: bool = False,
                                            save: bool = False) -> None:
        """Send system message to client WebSocket."""
        try:
            uuid = await self._save_system_message(message) if save else None
            await self.send(text_data=json.dumps({
                "type": "chat_message",
                "message": message,
                "author": "System",
                "system": True,
                "error": error,
                "uuid": uuid,
            }))
        except Exception as e:
            await self.close(code=1011)

    @retry(3)
    async def send_system_message_to_group(self,
                                           message: str,
                                           save: bool = False) -> None:
        """Broadcast system message to all group members."""
        try:
            uuid = await self._save_system_message(message) if save else None
            await self.channel_layer.group_send(
                self.GROUP_NAME,
                {
                    "type": "chat_message",
                    "author": "System",
                    "message": message,
                    "system": True,
                    "uuid": uuid,
                },
            )
        except Exception as e:
            await self.close(code=1011)

    @retry(3)
    async def chat_message(self, event: dict) -> None:
        """Handle message received from room group and forward to client."""
        try:
            valid, cleaned_data = await self.message_form(**event)
            if valid and cleaned_data is not None:
                data = {
                    "type": "chat_message",
                    "message": cleaned_data["message"],
                    "author": cleaned_data["author"],
                    "system": cleaned_data["system"],
                    "uuid": cleaned_data["uuid"],
                }

                if cleaned_data["uuid"]:
                    message = await self.get_message(cleaned_data["uuid"])
                    if message:
                        data["profile_url"] = await self.get_profile_url(
                            message)
                        data["photo"] = await self.get_user_photo(message)

                await self.send(text_data=json.dumps(data))
            else:
                await self.send_system_message_to_client(cleaned_data["error"],
                                                         error=True)
        except Exception as e:
            await self.send_general_error()

    async def send_general_error(self) -> None:
        """Send generic error message to client."""
        try:
            await self.send_system_message_to_client(
                "An error has occurred. Please try to reload the page.",
                error=True)
        except Exception as e:
            pass

    @database_sync_to_async
    def get_author(self, message: Message) -> str:
        """Get message author username or System/Unknown."""
        if message.author and message.author.username:
            return message.author.username
        return "System" if message.system else "Unknown"

    @database_sync_to_async
    def get_profile_url(self, message: Message) -> str:
        """Get author profile URL or empty string."""
        return f"/user/{message.author.username}" if message.author else ""

    @database_sync_to_async
    def get_user_photo(self, message: Message) -> str:
        """Get author emoji or empty string."""
        return message.author.emoji or "" if message.author else ""

    @database_sync_to_async
    def get_message(self, uuid: str) -> Message | None:
        """Retrieve message by UUID."""
        return Message.objects.filter(uuid=uuid).first()

    @database_sync_to_async
    def message_form(self, **kwargs) -> tuple[bool, dict]:
        """Validate message with form."""
        form = MessageForm(kwargs)
        if form.is_valid():
            return True, form.cleaned_data
        message_error = form.errors.get_json_data()
        if message_error:
            field, errors = next(iter(message_error.items()))
            if errors:
                return False, {"error": f"{field}: {errors[0]['message']}"}
        return False, {"error": "Invalid message."}

    @database_sync_to_async
    def create_message(self, message: str, system: bool = False) -> Message:
        """Create a new message in database."""
        return Message.objects.create(
            content=message,
            author=self.user if not system else None,
            system=system,
        )

    async def _check_authenticated(self) -> bool:
        """Check if user is authenticated."""
        if self.user is None:
            await self.send_system_message_to_client(
                "You don't have permission to access this chat.", error=True)
            await self.close(1000)
            return False
        return True

    async def _check_user_permission(self) -> bool:
        """Check if user is authenticated and not muted."""
        if not await self._check_authenticated():
            return False
        if self.user.is_muted():  # type: ignore
            await self.send_system_message_to_client(
                "You are currently muted and cannot send messages until "
                f"{self._format_muted_until()}.",
                error=True)
            return False
        return True

    def _format_muted_until(self) -> str:
        """Format the user's muted_until timestamp in local time."""
        local_time = timezone.localtime(self.user.muted_until)  # type: ignore
        return local_time.strftime('%Y-%m-%d %H:%M:%S')

    async def _save_system_message(self, message: str) -> str | None:
        """Save system message and return UUID."""
        msg = await self.create_message(message, system=True)
        return str(msg.uuid) if msg else None

    async def _handle_profanity_warning(self) -> None:
        """Handle profanity detection: warn user and send notification."""
        is_muted = await self._warn_user()

        await self.send_system_message_to_client(
            "Your message contains profanity, you received a warning."
            if not is_muted else
            "You have been muted for repeated profanity violations until "
            f"{self._format_muted_until()}.",
            error=True)

    @database_sync_to_async
    def _warn_user(self) -> bool:
        """Warn the user for profanity. Returns True if muted, False if warned.
        Also refreshes self.user to reflect database changes.
        """
        if self.user is None:
            return False

        is_muted = self.user.warn()  # Modifies user and saves to DB

        # Refresh user from database to get updated muted_until
        self.user.refresh_from_db()

        return is_muted

    async def _build_message_data(self, message: Message) -> dict:
        """Build message data dict with all details."""
        return {
            "type": "chat_message",
            "system": message.system,
            "message": message.content,
            "author": await self.get_author(message),
            "profile_url": await self.get_profile_url(message),
            "photo": await self.get_user_photo(message),
            "uuid": str(message.uuid),
        }
