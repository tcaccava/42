from django.shortcuts import render
import json
import re
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.hashers import make_password, check_password
from django.core.exceptions import ObjectDoesNotExist
from django.core import signing
from django.core.files.storage import default_storage

from apps.accounts.models import Badge, User
from apps.core.models import Statistic
from apps.gameplay.models import OldMatches, Question, SinglePlayerMatch
from ..utils.match_badges import check_match_badges


def build_avatar_payload(user, request=None):
    profile_picture = None
    if user.profile_picture:
        if request is not None:
            profile_picture = request.build_absolute_uri(user.profile_picture.url)
        else:
            profile_picture = user.profile_picture.url
    return {
        "emoji": user.emoji,
        "profile_picture": profile_picture,
    }


@csrf_exempt
def change_emoji(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    emojiOptions = [
        '👤', '😃', '😎', '🦊', '🐱', '🐶', '🦄', '🐸', '🐵', '👽', '🤖', '👾', '🧑', '👩',
        '🧙', '🧛', '🧟', '🧞', '🧜', '🧚', '🧝', '🧔', '👩‍🦰', '👨‍🦰', '👩‍🦱', '👨‍🦲',
        '👩‍🦳', '👨‍🦳'
    ]

    try:
        data = json.loads(request.body)
        new_emoji = data.get('emoji', '').strip()

        if not new_emoji:
            return JsonResponse({'error': 'Emoji required'}, status=400)
        elif new_emoji not in emojiOptions:
            return JsonResponse({'error': 'Emoji not in list'}, status=400)

        user = User.objects.get(id=request.session['user_id'])
        user.emoji = new_emoji
        user.save()
        return JsonResponse(
            {
                'success': True,
                'message': 'Emoji updated',
                'emoji': user.emoji,
                "profile_picture": build_avatar_payload(user, request)[
                    "profile_picture"
                ],
            },
            status=200)
    except Exception as e:
        return JsonResponse({'error': str(e)}, status=500)


@csrf_exempt
def upload_profile_picture(request):
    if request.method != "POST":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"error": "User not authenticated"}, status=400)

    try:
        current_user = User.objects.get(id=user_id)
    except User.DoesNotExist:
        return JsonResponse({"error": "User not found"}, status=404)

    uploaded_file = request.FILES.get("profile_picture")
    if not uploaded_file:
        return JsonResponse({"error": "Profile picture required"}, status=400)

    allowed_types = {"image/jpeg", "image/png", "image/webp", "image/gif"}
    if uploaded_file.content_type not in allowed_types:
        return JsonResponse({"error": "Unsupported file type"}, status=400)

    max_size = 5 * 1024 * 1024
    if uploaded_file.size > max_size:
        return JsonResponse({"error": "File too large"}, status=400)

    old_picture_name = (
        current_user.profile_picture.name if current_user.profile_picture else None
    )
    current_user.profile_picture = uploaded_file
    current_user.save()

    if old_picture_name and old_picture_name != current_user.profile_picture.name:
        default_storage.delete(old_picture_name)

    avatar_payload = build_avatar_payload(current_user, request)
    request.session["profile_picture"] = avatar_payload["profile_picture"]

    return JsonResponse(
        {
            "success": True,
            **avatar_payload,
        },
        status=200,
    )


@csrf_exempt
def remove_profile_picture(request):
    if request.method != "POST":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"error": "User not authenticated"}, status=400)

    try:
        current_user = User.objects.get(id=user_id)
    except User.DoesNotExist:
        return JsonResponse({"error": "User not found"}, status=404)

    if current_user.profile_picture:
        current_user.profile_picture.delete(save=False)
        current_user.profile_picture = None
        current_user.save()

    avatar_payload = build_avatar_payload(current_user, request)
    request.session["profile_picture"] = None

    return JsonResponse(
        {
            "success": True,
            **avatar_payload,
        },
        status=200,
    )


@csrf_exempt
def change_username(request):
    if (request.method != 'POST'):
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'User not authenticated'}, status=400)

    try:
        current_user = User.objects.get(id=user_id)
    except ObjectDoesNotExist:
        return JsonResponse({'error': 'User not found'}, status=404)

    try:
        data = json.loads(request.body)
        new_username = data.get('username')
        if not re.match(r'^[a-zA-Z0-9_]{3,30}$', new_username):
            return JsonResponse({
                'error': 'profile.username_invalid'
            }, status=400)
        if User.objects.filter(username=new_username).exists():
            return JsonResponse({'error': 'profile.username_taken'},
                                status=401)
    except:
        return JsonResponse({'error': 'Invalid Json'}, status=402)

    try:
        current_user = User.objects.get(id=user_id)
        current_user.username = new_username
        current_user.save()
        return JsonResponse({'success': True}, status=200)
    except:
        return JsonResponse({'error': 'User not found'}, status=404)


@csrf_exempt
def get_statistics(request):
    if request.method != 'GET':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user = request.GET.get('username', '').strip()
    if not user:
        return JsonResponse({'error': 'Missing field'}, status=201)
    try:
        current_user = User.objects.get(username=user)
        statistics, _ = Statistic.objects.get_or_create(user=current_user)
    except:
        return JsonResponse({'error': 'User not found'}, status=404)

    check_match_badges(current_user)
    badges = statistics.badges.all().values('name', 'description', 'image')
    mp_average_score = 0
    sp_average_score = 0
    if statistics.single_player_games > 0:
        sp_average_score = statistics.sp_correct_answers / statistics.single_player_games
    if statistics.multiplayer_games > 0:
        mp_average_score = statistics.mp_correct_answers / statistics.multiplayer_games
    
    return JsonResponse(
        {
            **build_avatar_payload(current_user, request),
            'success':
            True,
            'sp_victories': statistics.victories,
            'lost': statistics.lost,
            'xp': statistics.xp,
            'wrong_answers': statistics.wrong_answers,
            'correct_answers': statistics.correct_answers,
            'badges': list(badges),
            'games': statistics.games,
            'sp_average_score': sp_average_score,
            'mp_average_score': mp_average_score,
            'lifeline_used': statistics.lifeline_used,
            'mp_victories': statistics.tournament_wins,
            'wins_in_a_row': statistics.wins_in_a_row,
            'highest_streak': statistics.highest_streak,
            'last_matches':
            list(
                OldMatches.objects.filter(
                    user=current_user).order_by('-finished_at').values(
                        'finished_at', 'correct_answers', 'status',
                        'mode', 'ranking')[:5])
        },
        status=200)


@csrf_exempt
def get_old_matches(request):
    if request.method != 'GET':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'Missing field'}, status=201)
    try:
        current_user = User.objects.get(id=user_id)
    except:
        return JsonResponse({'error': 'User not found'}, status=404)

    old_matches = OldMatches.objects.filter(user=current_user)
    old_matches = old_matches.order_by('-finished_at').values(
        'finished_at', 'correct_answers', 'status', 'mode', 'ranking')
    return JsonResponse({'old_matches': list(old_matches)}, status=200)


@csrf_exempt
def get_all_badges(request):
    if request.method != 'GET':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'user not authenticated'}, status=400)

    try:
        current_user = User.objects.get(id=user_id)
        statistics = Statistic.objects.get(user=current_user)
    except:
        return JsonResponse({'error': 'Object not found'}, status=404)
    user_badges = statistics.badges.all()
    completed_badges = user_badges.values('name', 'description', 'image')
    to_complete_badges = Badge.objects.exclude(
        name__in=user_badges.values('name')).values('name', 'description',
                                                    'image')
    return JsonResponse(
        {
            'completed': list(completed_badges),
            'to_complete': list(to_complete_badges)
        },
        status=200)
