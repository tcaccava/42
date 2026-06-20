from django.shortcuts import render
import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.hashers import make_password, check_password
from django.core.exceptions import ObjectDoesNotExist
from django.core import signing

from apps.accounts.models import Badge, User
from django.db.models import Q
from apps.core.models import Notification, Statistic
from ..models import Friends
from ...core.views.notifications import create_notification
from ...core.views.leaderboard import leaderboard_leader
from ...gameplay.utils.game_utils import add_xp

@csrf_exempt
def leader(statistics):
    badge = Badge.objects.get(name="badge.name19")
    if not statistics.badges.filter(name=badge.name):
        leader = leaderboard_leader()
        if statistics.user.username == leader:
            statistics.badges.add(badge)
            statistics.xp += badge.xp
            statistics.save()
            text = f"{badge.name}"
            create_notification(text, statistics.user, 'badge')

@csrf_exempt
def social_butterfly(user):
    try:
        statistics = Statistic.objects.get(user=user)
    except:
        return
    friends = Friends.objects.filter(
        (Q(user1=user) | Q(user2=user)) & Q(status='accepted')
    ).count()
    if friends >= 5:
        try:
            badge = Badge.objects.get(name="badge.name14")
        except:
            return
        if not statistics.badges.filter(name=badge.name).exists():
            statistics.badges.add(badge)
            add_xp(badge.xp, statistics)
            text = f"{badge.name}"
            create_notification(text, statistics.user, 'badge')

@csrf_exempt
def lightning_bolt(statistics, seconds):
    if (seconds < 3):
        try:
            badge = Badge.objects.get(name="badge.name16")
        except:
            return
        if not statistics.badges.filter(name=badge.name).exists():
            statistics.badges.add(badge)
            add_xp(badge.xp, statistics)
            text = f"{badge.name}"
            create_notification(text, statistics.user, 'badge')