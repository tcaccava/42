from django.urls import path, re_path
from .consumer import GlobalChatConsumer, GameConsumer, NotificationConsumer

websocket_urlpatterns = [
    re_path(r'^ws/notifications/$', NotificationConsumer.as_asgi()),
    path('ws/chat/', GlobalChatConsumer.as_asgi()),
    re_path(r'^ws/game/(?P<session_id>[\w-]+)/$', GameConsumer.as_asgi()),
]
