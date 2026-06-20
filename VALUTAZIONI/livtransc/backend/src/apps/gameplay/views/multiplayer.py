from django.shortcuts import render
import json
import string
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.exceptions import ObjectDoesNotExist
from django.db.models import Q

from apps.accounts.models import User
from apps.gameplay.models import LocalMultiplayer, Multiplayer
import random
from django.utils import timezone
from ...accounts.utils.match_badges import check_match_badges
from .localMultiplayer import questions


def add_player(user, match):
    if user not in match.users:
        match.users.add(user)
        match.save()


@csrf_exempt
def create_MP_game(request):
    if (request.method != 'POST'):
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'user not authenticated'}, status=400)
    try:
        current_user = User.objects.get(id=user_id)
    except:
        return JsonResponse({'error': 'user not found'}, status=404)

    try:
        data = json.loads(request.body)
        code = data.GET.get('roomcode')
    except:
        return JsonResponse({'error': 'invalid json'}, status=500)

    if code:
        match = Multiplayer.objects.get(code=code, status='waiting')
    else:
        match = Multiplayer.objects.get(status='waiting')
    if not match:
        question_ids = questions(15)
        code = "".join(random.choices(string.digits, k=6))
        while (code in LocalMultiplayer.objects.values_list('code')):
            code = "".join(random.choices(string.digits, k=6))
        new_MP = LocalMultiplayer(code=code, question_ids=question_ids)
        new_MP.save()
        add_player(user, new_MP)
        return JsonResponse(
            {
                'success': True,
                'match_id': new_MP.id,
                'code': new_MP.code,
            },
            status=200)
    else:
        add_player(user, match)
        return JsonResponse(
            {
                'success': True,
                'match_id': match.id,
                'code': match.code,
            },
            status=200)


@csrf_exempt
def start_game(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Invalid Method'}, status=405)

    try:
        data = json.loads(request.body)
        code = data.GET.get('roomcode')
        if not code:
            return JsonResponse({'error': 'missing field'}, status=400)
    except:
        return JsonResponse({'error': 'invalid json'}, status=500)

    try:
        match = Multiplayer.objects.get(code=roomcode)
        if match.status != 'waiting' or len(match.players) < 2:
            return JsonResponse({'error': 'Cannot start the match'},
                                status=402)
    except:
        return JsonResponse({'error': 'Match not found'}, status=404)

    match.status = 'playing'
    match.save()
    return JsonResponse(
        {
            'success': True,
            'match_id': match.id,
            'next_question': {
                'number': match.question_number + 1,
                'text': question.text,
                'options': [question.a, question.b, question.c, question.d],
            }
        },
        status=200)

@csrf_exempt
def create_lobby(request):
    from ..multiplayer_session import MultiplayerSession, active_sessions, GameState

    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'Authentication required'}, status=401)

    active_player_states = {
        GameState.LOBBY,
        GameState.START,
        GameState.QUESTION,
        GameState.PAUSED,
    }

    # If the user is already part of an active multiplayer session, reuse it.
    for session in active_sessions.values():
        if session.game_state in active_player_states and user_id in session.players:
            return JsonResponse({
                'success': True,
                'session_id': session.session_id,
            })

    # Search for an existing active session that is still in LOBBY phase and not full
    for session in active_sessions.values():
        if session.game_state == GameState.LOBBY and not session.is_full():
            return JsonResponse({
                'success': True,
                'session_id': session.session_id
            })

    new_session = MultiplayerSession(user_id)
    active_sessions[new_session.session_id] = new_session

    return JsonResponse({
        'success': True,
        'session_id': new_session.session_id
    })
