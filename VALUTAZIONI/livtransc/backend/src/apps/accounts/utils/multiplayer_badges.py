from apps.accounts.models import Badge, User
from apps.core.models import Notification, Statistic
from ...core.views.notifications import create_notification
from asgiref.sync import sync_to_async
from channels.db import database_sync_to_async

def perfectionist(ranking, user=None):
    if user["rank"] == 1 and ranking[0]["score"] == 15:
        return True
    return False

def sharpshooter(ranking, user):
    second = None
    for player in ranking:
        if player["rank"] == 2:
            second = player
            break
    if second == None:
        return False
    if user["rank"] == 1 and ranking[0]["score"] >= second["score"] + 5:
        return True
    return False

def check_multiplayer_badges(statistics, ranking, user):

    from apps.gameplay.utils.game_utils import add_xp

    badges = Badge.objects.filter(use='multiplayer')
    for badge in badges:
        if not statistics.badges.filter(name=badge.name).exists():
            func_name = badge.condition
            if not func_name:
                continue
            func = globals().get(func_name)
            if not func or not callable(func):
                continue
            try:
                if func(ranking, user):
                    statistics.badges.add(badge)
                    add_xp(badge.xp, statistics)
                    text = f"{badge.name}"
                    create_notification(text, statistics.user, 'badge')
            except Exception as e:
                print(e)
                continue
    return 0


@database_sync_to_async
def add_badge(user_id, badge_name):

    from apps.gameplay.utils.game_utils import add_xp

    try:
        stat = Statistic.objects.get(user__id=user_id)
        badge = Badge.objects.get(name=badge_name)
        if not stat.badges.filter(name=badge.name).exists():
            stat.badges.add(badge)
            add_xp(badge.xp, stat)
            text = f"{badge.name}"
            create_notification(text, stat.user, 'badge')
        return stat
    except Exception as e:
        print(e)
        return None


async def still_undecided_mp(result):
    for player in result:
            if player["badge_su"]:
                await(add_badge(player["user_id"], "badge.name22"))


async def the_know_it_all(results):
    
    count = 0
    for player in results:
        if player["correct"] == True:
            count += 1
            winner = player

    if count == 1:
        await(add_badge(winner["user_id"], "badge.name20"))