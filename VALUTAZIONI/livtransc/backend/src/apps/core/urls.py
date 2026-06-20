from django.urls import path

from .views import leaderboard, notifications

urlpatterns = [
    path('get_leaderboard',
         leaderboard.get_leaderboard,
         name='api-get_leaderboard'),
    path('get_notifications',
         notifications.get_notifications,
         name='api-get_notifications'),
    path('delete_notification',
         notifications.delete_notification,
         name='api-delete_notification'),
]
