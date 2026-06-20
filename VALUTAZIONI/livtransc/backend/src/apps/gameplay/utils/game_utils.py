from django.shortcuts import render
import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.exceptions import ObjectDoesNotExist
from django.db.models import Q
from asgiref.sync import sync_to_async
from channels.db import database_sync_to_async
from ...core.models import Statistic
from apps.gameplay.models import OldMatches
from apps.accounts.models import User
from apps.accounts.utils.match_badges import check_match_badges
from apps.accounts.utils.multiplayer_badges import check_multiplayer_badges

@csrf_exempt
def add_xp(num, statistics):
    from ...accounts.utils.general_badges import leader

    statistics.xp += num
    statistics.save()
    try:
        leader(statistics)
    except Exception:
        pass

async def update_games(users_id):
    for user_id in users_id:
        try:
            @database_sync_to_async
            def get_and_update_statistics():
                statistics = Statistic.objects.get(user_id=user_id)
                statistics.games += 1
                statistics.multiplayer_games += 1
                statistics.save()
                return statistics
            
            await get_and_update_statistics()
        except Statistic.DoesNotExist:
            print(f"Statistic not found for user_id {user_id}")
        except Exception as e:
            print(f"Error updating games for user_id {user_id}: {e}")

@csrf_exempt
def statistics_correct_answers(statistics):
    statistics.correct_answers += 1
    statistics.mp_correct_answers += 1
    statistics.consecutive_correct_answers += 1
    if statistics.consecutive_correct_answers > statistics.highest_streak:
        statistics.highest_streak = statistics.consecutive_correct_answers
    statistics.save()

@csrf_exempt
def statistics_wrong_answer(statistics):
    statistics.wrong_answers += 1
    statistics.consecutive_correct_answers = 0
    statistics.save()

async def calculate_points(rankings):
    for player in rankings:
        try:
            @database_sync_to_async
            def update(player):
                stat = Statistic.objects.get(user_id=player["user_id"])
                if player["rank"] == 1:
                    stat.tournament_wins += 1
                    stat.wins_in_a_row += 1
                    stat.save()
                if player["rank"] <= 3:    
                    add_xp(player["score"] * 10, stat)
                OldMatches.objects.create(
                    user=stat.user,
                    mode="play.multiplayer",
                    correct_answers=player["score"],
                    wrong_answers=15 - player["score"],
                    status=0 if player["rank"] == 1 else -1,
                    ranking=player["rank"],
                )
                check_match_badges(stat.user)
                check_multiplayer_badges(stat, rankings, player)
                return(stat)
            await update(player)
        except Exception as e:
            print(f"Error processing player {player['user_id']}: {e}")
            continue
