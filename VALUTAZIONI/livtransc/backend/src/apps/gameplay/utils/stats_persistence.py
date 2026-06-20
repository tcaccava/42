import logging

from asgiref.sync import sync_to_async

from apps.accounts.models import User
from apps.accounts.utils.match_badges import check_match_badges
from apps.core.models import Statistic
from apps.gameplay.models import OldMatches

logger = logging.getLogger(__name__)

TOTAL_QUESTIONS = 15


def calculate_match_results(session):
    """Calculate per-player results from a completed session.

    Returns a list of dicts:
        [{user_id, correct, wrong, is_winner, display_name}]

    Handles ties: all players sharing the highest score are winners.
    Handles disconnected players: they get results based on answers
    submitted up to their disconnection point. Questions missed while a
    player was disconnected score zero but are NOT counted as wrong answers.
    """
    results = []
    max_score = max(session.scores.values()) if session.scores else 0

    for user_id, player_info in session.players.items():
        score = session.scores.get(user_id, 0)
        missed_count = len(session.missed_questions.get(user_id, []))
        wrong = max(0, TOTAL_QUESTIONS - score - missed_count)
        is_winner = score == max_score and max_score > 0
        results.append({
            "user_id": user_id,
            "correct": score,
            "wrong": wrong,
            "is_winner": is_winner,
            "display_name": player_info.display_name,
        })

    for user_id in session.disconnected_players:
        if user_id not in session.players:
            score = session.scores.get(user_id, 0)
            missed_count = len(session.missed_questions.get(user_id, []))
            wrong = max(0, TOTAL_QUESTIONS - score - missed_count)
            results.append({
                "user_id": user_id,
                "correct": score,
                "wrong": wrong,
                "is_winner": False,
                "display_name": f"Player {user_id}",
            })

    return results


@sync_to_async
def persist_match_results(session):
    """Persist match results to database. Called at END_QUIZ.

    Creates OldMatches records and updates Statistic for each player.
    Wrapped in try/except — logs errors but doesn't raise to avoid
    blocking the game_end broadcast.
    """
    try:
        results = calculate_match_results(session)

        for result in results:
            try:
                user = User.objects.get(id=result["user_id"])
            except User.DoesNotExist:
                logger.warning(
                    "User %s not found, skipping stats persistence",
                    result["user_id"],
                )
                continue

            OldMatches.objects.create(
                user=user,
                mode="play.multiplayer",
                correct_answers=result["correct"],
                wrong_answers=result["wrong"],
                status=0 if result["is_winner"] else -1,
            )

            stat, _ = Statistic.objects.get_or_create(user=user)
            stat.games += 1
            stat.multiplayer_games += 1
            stat.correct_answers += result["correct"]
            stat.wrong_answers += result["wrong"]

            xp_earned = 2 * result["correct"]

            if result["is_winner"]:
                stat.victories += 1
                stat.wins_in_a_row += 1
                xp_earned += 50
            else:
                stat.lost += 1
                stat.wins_in_a_row = 0

            stat.xp += xp_earned
            stat.save()

            check_match_badges(user)

    except Exception as e:
        logger.exception("Error persisting match results: %s", e)
