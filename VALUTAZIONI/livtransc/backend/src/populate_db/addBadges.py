import os
import sys
import django

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "config.settings")
django.setup()

from apps.accounts.models import Badge

badges = [
    {
        "name": "badge.name1",
        "description": "badge.description1",
        "image": "🎮",
        "condition": "first_match",
        "type": "match",
        "xp": 10
    },
    {
        "name": "badge.name2",
        "description": "badge.description2",
        "image": "🤯",
        "condition": "mastermind",
        "type": "match",
        "xp": 500
    },
    {
        "name": "badge.name3",
        "description": "badge.description3",
        "image": "💫",
        "condition": "streak_king",
        "type": "match",
        "xp": 100
    },
    {
        "name": "badge.name4",
        "description": "badge.description4",
        "image": "🏆",
        "condition": "tournament_winner",
        "type": "match",
        "xp": 20
    },
    {
        "name": "badge.name5",
        "description": "badge.description5",
        "image": "💲",
        "condition": "so_many_xp",
        "type": "match",
        "xp": 10
    },
    {
        "name": "badge.name6",
        "description": "badge.description6",
        "image": "🏋️‍♂️",
        "condition": "unstoppable",
        "type": "match",
        "xp": 500
    },
    {
        "name": "badge.name7",
        "description": "badge.description7",
        "image": "🏅",
        "condition": "winner_winner",
        "type": "match",
        "xp": 100
    },
    {
        "name": "badge.name8",
        "description": "badge.description8",
        "image": "👑",
        "condition": "trivia_king",
        "type": "match",
        "xp": 500
    },
    {
        "name": "badge.name10",
        "description": "badge.description10",
        "image": "🏃‍♂️‍➡️",
        "condition": "veteran",
        "type": "match",
        "xp": 300
    },
    {
        "name": "badge.name11",
        "description": "badge.description11",
        "image": "🪄",
        "condition": "perfectionist",
        "type": "multiplayer",
        "xp": 50
    },
    {
        "name": "badge.name13",
        "description": "badge.description13",
        "image": "💪🏻",
        "condition": "sharpshooter",
        "type": "multiplayer",
        "xp": 50
    },
    {
        "name": "badge.name14",
        "description": "badge.description14",
        "image": "🫂",
        "condition": "social_butterfly",
        "type": "friend",
        "xp": 10
    },
    {
        "name": "badge.name15",
        "description": "badge.description15",
        "condition": "one_step_away_from_glory",
        "image": "🤏",
        "type": "question",
        "xp": 50
    },
    {
        "name": "badge.name16",
        "description": "badge.description16",
        "condition": "lightning_bolt",
        "image": "⚡",
        "type": "question",
        "xp": 50
    },
    {
        "name": "badge.name17",
        "description": "badge.description17",
        "image": "🐺",
        "condition": "lone_wolf",
        "type": "question",
        "xp": 100
    },
    {
        "name": "badge.name19",
        "description": "badge.description19",
        "condition": "leader",
        "image": "🎖️",
        "type": "none",
        "xp": 100
    },
    {
        "name": "badge.name20",
        "description": "badge.description20",
        "condition": "the_know_it_all",
        "image": "🤯",
        "type": "multiplayer",
        "xp": 100
    },
    {
        "name": "badge.name21",
        "description": "badge.description21",
        "condition": "epic_fail",
        "image": "😵‍💫",
        "type": "question",
        "xp": 10
    },
    {
        "name": "badge.name22",
        "description": "badge.description22",
        "condition": "still_undecided",
        "image": "👎",
        "type": "question",
        "xp": 10
    },
    {
        "name": "badge.name23",
        "description": "badge.description23",
        "condition": "master_of_accuracy",
        "image": "🎯",
        "type": "match",
        "xp": 200
    }
]

existing = Badge.objects.count()
if existing == len(badges):
    print(f"{existing} badges already present in the database; skipping insert.")
else:
    print(f"{existing} badges currently in the database; have {len(badges)} new badges...")
    Badge.objects.all().delete()
    for badge in badges:
        Badge.objects.create(
            name=badge.get('name'),
            description=badge.get('description'),
            image=badge.get('image'),
            condition=badge.get('condition'),
            xp=badge.get('xp', 0),
            use=badge.get('type', 'none')
        )
    print(len(badges), "badges inserted successfully!")
