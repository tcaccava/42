from django.urls import path

from apps.core import views

from .views import localMultiplayer, multiplayer, singleplayer

urlpatterns = [
    path('start_match', singleplayer.start_match, name='api-start_match'),
    path('update_match', singleplayer.update_match, name='api-update_match'),
    path('check_session', singleplayer.check_session,
         name='api-check_session'),
    path('use_help', singleplayer.use_help, name='api-use_help'),
    path('create_LMP_game',
         localMultiplayer.create_LMP_game,
         name='api-create_LMP_game'),
    path('get_MP_players',
         localMultiplayer.get_MP_players,
         name='api-get_MP_players'),
    path('add_MP_player',
         localMultiplayer.add_MP_player,
         name='api-add_MP_player'),
    path('get_next_question',
         localMultiplayer.get_next_question,
         name='api-get_next_question'),
    path('submit_answer',
         localMultiplayer.submit_answer,
         name='api-submit_answer'),
    path('get_answers', localMultiplayer.get_answers, name='api-get_answers'),
    path('check_session_local',
         localMultiplayer.check_session_local,
         name='api-check_session_local'),
    path('reset_LMP_game',
         localMultiplayer.reset_LMP_game,
         name='api-reset_LMP_game'),
    path('create_MP_game',
         multiplayer.create_MP_game,
         name='api-create_MP_game'),
    path('start_game', multiplayer.start_game, name='api-start_game'),
    path('multiplayer', multiplayer.create_lobby, name='multiplayer'),
]
