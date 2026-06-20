from django.urls import path

from .views import auth, friend, profile

urlpatterns = [
    path('get_session', auth.get_session, name='api-get_session'),
    path('friends', friend.getFriends, name='api-friends'),
    path('search_users', friend.get_users, name='api-users'),
    path('change_emoji', profile.change_emoji, name='api-change_emoji'),
	path(
        "upload_profile_picture",
        profile.upload_profile_picture,
        name="api-upload_profile_picture",
    ),
    path(
        "remove_profile_picture",
        profile.remove_profile_picture,
        name="api-remove_profile_picture",
    ),
    path('send_request', friend.send_request, name='api-send_request'),
    path('accept_request', friend.accept_request, name='api-accept_request'),
    path('remove_friend', friend.remove_friend, name='api-remove_friend'),
    path('invite_player', friend.invite_player, name='api-invite_player'),
    path('get_statistics', profile.get_statistics, name='api-get_statistics'),
    path('get_old_matches',
         profile.get_old_matches,
         name='api-get_old_matches'),
    path('get_all_badges', profile.get_all_badges, name='api-get_all_badges'),
    path('change_username',
         profile.change_username,
         name='api-change_username'),
]
