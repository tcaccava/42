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
from ...gameplay.utils.game_utils import add_xp

@csrf_exempt
def one_step_away_from_glory(match, id):
    if not id and match.question_number == 15:
        return True
    return False

@csrf_exempt
def lone_wolf(match, id):
    if id and match.fifty_fifty and match.refill_time and match.double_chance and match.change_question:
        return True
    return False

@csrf_exempt
def epic_fail(match, id):
    if not id and match.question_number == 1:
        return True
    return False

@csrf_exempt
def still_undecided(match, id, do=False):
    if not hasattr(still_undecided, 'state'):
        still_undecided.state = False

    if do:
        still_undecided.state = True
        still_undecided.question_number = id
        return

    try:
        state = getattr(still_undecided, 'state', False)
        question = getattr(still_undecided, 'question_number', False)
        if state and not id:
            print(question)
            print(match.question_ids[match.question_number - 1])
            if question == match.question_ids[match.question_number - 1]:
                state = False
                return True
        else:
            state = False
            return False
    except:
        return False

@csrf_exempt
def check_question_badges(match, user, id):
    try:
        statistics = Statistic.objects.get(user=user)
    except:
        return 

    badges = Badge.objects.filter(use='question')
    for badge in badges:
        if not statistics.badges.filter(name=badge.name):
            func_name = badge.condition
            if not func_name:
                continue
            func = globals().get(func_name)
            if not func or not callable(func):
                continue
            try:
                if func(match, id):
                    statistics.badges.add(badge)
                    add_xp(badge.xp, statistics)
                    text = f"{badge.name}"
                    create_notification(text, user, 'badge')
            except Exception:
                continue
    return 0