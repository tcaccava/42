from django.shortcuts import render
import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.hashers import make_password, check_password
from django.core.exceptions import ObjectDoesNotExist
from django.core import signing

from apps.accounts.models import Badge, User
from apps.core.models import Notification, Statistic
from ...core.views.notifications import create_notification

@csrf_exempt
def first_match(statistics):
    if statistics.games >= 1:
        return True
    return False

@csrf_exempt
def mastermind(statistics):
    if statistics.victories >= 50:
        return True
    return False

@csrf_exempt
def streak_king(statistics):
    if statistics.highest_streak >= 31:
        return True
    return False

@csrf_exempt
def tournament_winner(statistics):
    if statistics.tournament_wins >= 1:
        return True
    return False

@csrf_exempt
def so_many_xp(statistics):
    if statistics.xp >= 5000:
        return True
    return False

@csrf_exempt
def unstoppable(statistics):
    if statistics.wins_in_a_row >= 5:
        return True
    return False

@csrf_exempt
def winner_winner(statistics):
    if statistics.victories >= 1:
        return True
    return False

@csrf_exempt
def trivia_king(statistics):
    if statistics.highest_streak > 100:
        return True
    return False

@csrf_exempt
def veteran(statistics):
    if statistics.games > 100:
        return True
    return False

@csrf_exempt
def master_of_accuracy(statistics):
    if statistics.games > 5 and statistics.correct_answers / (statistics.correct_answers + statistics.wrong_answers) > 0.8:
        return True
    return False
    
@csrf_exempt
def check_match_badges(user):
    from apps.gameplay.utils.game_utils import add_xp

    try:
        statistics = Statistic.objects.get(user=user)
    except:
        return 

    badges = Badge.objects.filter(use='match')
    for badge in badges:
        if not statistics.badges.filter(name=badge.name):
            func_name = badge.condition
            if not func_name:
                continue
            func = globals().get(func_name)
            if not func or not callable(func):
                continue
            try:
                if func(statistics):
                    statistics.badges.add(badge)
                    add_xp(badge.xp, statistics)
                    statistics.save()
                    text = f"{badge.name}"
                    create_notification(text, user, 'badge')
            except Exception:
                continue
    return 0
