import json

from apps.accounts.models import User
from asgiref.sync import sync_to_async
from channels.generic.websocket import AsyncWebsocketConsumer


class NotificationConsumer(AsyncWebsocketConsumer):
    """Handle user notifications via WebSocket."""

    @sync_to_async
    def set_user_online_status(self, is_online: bool) -> None:
        """Update user online status."""
        User.objects.filter(id=self.user_id).update(is_online=is_online)

    async def connect(self) -> None:
        """Accept connection and set user online."""
        user_id = self.scope['session'].get('user_id')
        if not user_id:
            await self.close()
            return

        self.user_id = user_id
        self.group_name = f'user_{user_id}'

        await self.channel_layer.group_add(self.group_name, self.channel_name)
        await self.set_user_online_status(True)
        await self.accept()

    async def disconnect(self, close_code: int) -> None:
        """Set user offline and remove from group."""
        if hasattr(self, 'group_name'):
            await self.set_user_online_status(False)
            await self.channel_layer.group_discard(self.group_name,
                                                   self.channel_name)

    async def notify_user(self, event: dict) -> None:
        """Send notification to client."""
        await self.send(text_data=json.dumps({
            'type': 'notification',
        }))
