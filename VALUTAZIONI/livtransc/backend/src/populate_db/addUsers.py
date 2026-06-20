import os
import django

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "config.settings")
django.setup()

from apps.accounts.models import User
from apps.core.models import Statistic

users = [
    {
        "username": "simone",
        "email": "simone@test.it",
        "password": "",
        "emoji": "🧙",
        "stats": {
            "games": 42,
            "xp": 25200,
            "correct_answers": 630,
            "wrong_answers": 18,
            "highest_streak": 110,
            "lifeline_used": 51,
            "single_player_games": 50,
            "victories": 50,
            "lost": 0,
            "sp_correct_answers": 380,
            "wins_in_a_row": 50,
            "multiplayer_games": 35,
            "tournament_wins": 35,
            "mp_correct_answers": 145,
        }
    },
    {
        "username": "liviana",
        "email": "liviana@test.it",
        "password": "",
        "emoji": "🦄",
        "stats": {
            "games": 28,
            "xp": 6800,
            "correct_answers": 95,
            "wrong_answers": 45,
            "highest_streak": 16,
            "lifeline_used": 22,
            "single_player_games": 15,
            "victories": 10,
            "lost": 5,
            "sp_correct_answers": 55,
            "wins_in_a_row": 5,
            "multiplayer_games": 13,
            "tournament_wins": 3,
            "mp_correct_answers": 40,
        }
    },
    {
        "username": "federico",
        "email": "federico@test.it",
        "password": "",
        "emoji": "👾",
        "stats": {
            "games": 50,
            "xp": 4100,
            "correct_answers": 180,
            "wrong_answers": 70,
            "highest_streak": 18,
            "lifeline_used": 6,
            "single_player_games": 30,
            "victories": 22,
            "lost": 8,
            "sp_correct_answers": 110,
            "wins_in_a_row": 7,
            "multiplayer_games": 20,
            "tournament_wins": 4,
            "mp_correct_answers": 70,
        }
    },
    {
        "username": "giuseppe",
        "email": "giuseppe@test.it",
        "password": "",
        "emoji": "🦊",
        "stats": {
            "games": 15,
            "xp": 3900,
            "correct_answers": 35,
            "wrong_answers": 40,
            "highest_streak": 21,
            "lifeline_used": 20,
            "single_player_games": 10,
            "victories": 3,
            "lost": 7,
            "sp_correct_answers": 20,
            "wins_in_a_row": 1,
            "multiplayer_games": 5,
            "tournament_wins": 0,
            "mp_correct_answers": 15,
        }
    },
    {
        "username": "lorenzo",
        "email": "lorenzo@test.it",
        "password": "",
        "emoji": "😎",
        "stats": {
            "games": 33,
            "xp": 2600,
            "correct_answers": 105,
            "wrong_answers": 60,
            "highest_streak": 25,
            "lifeline_used": 13,
            "single_player_games": 18,
            "victories": 11,
            "lost": 7,
            "sp_correct_answers": 60,
            "wins_in_a_row": 3,
            "multiplayer_games": 15,
            "tournament_wins": 1,
            "mp_correct_answers": 45,
        }
    } 
]

existing = User.objects.count()

if existing >= len(users):
    print(
        f"{existing} users already present in the database; skipping insert."
    )
else:
    print("Cleaning table...")

    User.objects.all().delete()

    for user_data in users:
        user = User.objects.create(
            username=user_data["username"],
            email=user_data["email"],
            password=user_data["password"],
            emoji=user_data.get("emoji"),
            intra_id=user_data.get("intra_id"),
        )
        
        stats = user_data["stats"]
        
        Statistic.objects.create(
            user=user,
            games=stats["games"],
            xp=stats["xp"],
            correct_answers=stats["correct_answers"],
            wrong_answers=stats["wrong_answers"],
            consecutive_correct_answers=0,
            highest_streak=stats["highest_streak"],
            lifeline_used=stats["lifeline_used"],
            single_player_games=stats["single_player_games"],
            victories=stats["victories"],
            lost=stats["lost"],
            sp_correct_answers=stats["sp_correct_answers"],
            wins_in_a_row=stats["wins_in_a_row"],
            multiplayer_games=stats["multiplayer_games"],
            tournament_wins=stats["tournament_wins"],
            mp_correct_answers=stats["mp_correct_answers"]
        )

    print(f"{len(users)} users and their statistics inserted successfully!")
