import uuid
from dataclasses import dataclass, field
from datetime import datetime
from enum import Enum
from math import floor
from random import shuffle
from typing import Optional, Tuple
from .views.localMultiplayer import questions
from .utils.game_utils import add_xp, statistics_correct_answers, statistics_wrong_answer, calculate_points
from ..core.models import Statistic
import asyncio
active_sessions = {}

MAX_PLAYERS = 8
MAX_SPECTATORS = 10


@dataclass
class PlayerInfo:
    user_id: int
    display_name: str
    channel_name: str
    connected: bool
    disconnect_time: Optional[datetime]


@dataclass
class HintState:
    """Per-player hint availability and active state."""
    fifty_fifty_available: bool = True
    double_chance_available: bool = True
    scrying_available: bool = True
    double_chance_active: bool = False
    double_chance_first_answer: Optional[int] = None
    scrying_active: bool = False
    fifty_fifty_active: bool = False


class GameState(Enum):
    LOBBY = 1
    START = 2
    QUESTION = 3
    WAITING = 4
    NEXT_QUESTION = 5
    END_QUIZ = 6
    END_SESSION = 7
    PAUSED = 8


class MultiplayerSession:
    def __init__(self, host: int):
        self.host = host
        self.session_id = str(uuid.uuid4())
        self.game_state = GameState.LOBBY
        self.questions = questions(15)
        self.question_number = 0
        self.current_answers = {}
        self.scores = {}

        # Extended fields for remote multiplayer
        self.players: dict[int, PlayerInfo] = {}
        self.disconnected_players: set[int] = set()
        self.spectators: set[int] = set()
        self.join_order: list[int] = []
        self.question_start_time: Optional[datetime] = None
        self.created_at: datetime = datetime.now()

        # Pause/resume context (game-pause-reconnect feature)
        self.pre_pause_state: Optional[GameStactivate_fiate] = None
        self.remaining_time: Optional[float] = None
        self.pause_reason: Optional[str] = None
        self.paused_disconnected_names: list[str] = []
        self.missed_questions: dict[int, list[int]] = {}
        self.paused_at: Optional[datetime] = None

        # Hint states (multiplayer-hints feature)
        self.hint_states: dict[int, HintState] = {}

    def add_player(self, user_id: int, display_name: str, channel_name: str) -> bool:
        """Add a player to the session.

        Returns True if the player was added successfully, False if the session is full
        or the player is already in the session.
        """
        if self.is_full():
            return False
        if user_id in self.players:
            return False

        self.players[user_id] = PlayerInfo(
            user_id=user_id,
            display_name=display_name,
            channel_name=channel_name,
            connected=True,
            disconnect_time=None,
        )
        self.join_order.append(user_id)
        self.scores[user_id] = 0
        self.init_hint_state(user_id)
        return True

    def init_hint_state(self, user_id: int) -> None:
        """Initialize hint availability for a player joining the session."""
        self.hint_states[user_id] = HintState()

    def validate_hint_request(self, user_id: int, hint_name: str) -> Tuple[bool, Optional[str]]:
        """Validate preconditions for hint usage.

        Returns (allowed, error_code). Checks:
        - game state is QUESTION
        - player is connected (not in disconnected_players)
        - hint not already used
        - for fifty_fifty and double_chance: player hasn't already answered
        """
        if self.game_state != GameState.QUESTION:
            return (False, "INVALID_STATE")
        if user_id in self.disconnected_players:
            return (False, "NOT_CONNECTED")
        if user_id not in self.hint_states:
            return (False, "NOT_CONNECTED")

        hs = self.hint_states[user_id]
        if hint_name == "fifty_fifty" and not hs.fifty_fifty_available:
            return (False, "HINT_ALREADY_USED")
        elif hint_name == "double_chance" and not hs.double_chance_available:
            return (False, "HINT_ALREADY_USED")
        elif hint_name == "scrying" and not hs.scrying_available:
            return (False, "HINT_ALREADY_USED")

        # For fifty_fifty and double_chance: reject if already answered
        if hint_name in ("fifty_fifty", "double_chance") and user_id in self.current_answers:
            return (False, "ALREADY_ANSWERED")

        return (True, None)

    def activate_double_chance(self, user_id: int) -> None:
        """Mark double_chance as used and set active for this question."""
        hs = self.hint_states[user_id]
        hs.double_chance_available = False
        hs.double_chance_active = True

    def activate_scrying(self, user_id: int) -> None:
        """Mark scrying as used and set active for this question."""
        hs = self.hint_states[user_id]
        hs.scrying_available = False
        hs.scrying_active = True

    def is_player_mid_retry(self, user_id: int) -> bool:
        """True if the player has Double Chance active, submitted a wrong first answer, but no second yet."""
        if user_id not in self.hint_states:
            return False
        hs = self.hint_states[user_id]
        return (hs.double_chance_active
                and hs.double_chance_first_answer is not None
                and user_id not in self.current_answers)

    def get_scrying_players(self) -> list[int]:
        """Return user_ids of players with active Scrying this question."""
        return [uid for uid, hs in self.hint_states.items() if hs.scrying_active]

    def check_answer_correct(self, option_index: int) -> bool:
        """Check if an option index is the correct answer for the current question.

        Loads the question from DB (same pattern as transition_to_waiting).
        Returns True if the option at option_index matches the correct answer.
        """
        from apps.gameplay.models import Question
        question_id = self.questions[self.question_number]
        question = Question.objects.get(id=question_id)
        options = [question.a, question.b, question.c, question.d]
        return options[option_index] == question.answer

    def get_correct_answer_index(self) -> int:
        """Get the correct answer's option index (0-3) for the current question."""
        from apps.gameplay.models import Question
        question_id = self.questions[self.question_number]
        question = Question.objects.get(id=question_id)
        options = [question.a, question.b, question.c, question.d]
        for idx, opt in enumerate(options):
            if opt == question.answer:
                return idx
        return 0  # fallback (should never happen with valid data)

    def activate_fifty_fifty(self, user_id: int) -> list[int]:
        """Mark fifty_fifty as used. Pick 2 random incorrect option indices.

        Returns a list of 2 eliminated indices (both different from the correct answer).
        """
        import random
        hs = self.hint_states[user_id]
        hs.fifty_fifty_available = False
        hs.fifty_fifty_active = True

        correct_idx = self.get_correct_answer_index()
        incorrect_indices = [i for i in range(4) if i != correct_idx]
        # Pick 2 out of 3 incorrect indices at random
        eliminated = random.sample(incorrect_indices, 2)
        return eliminated

    def still_undecided(self, user_id) -> None:
        hs = self.hint_states[user_id]
        if hs.fifty_fifty_active:
            return True
        return False

    def reset_question_hint_state(self) -> None:
        """Clear per-question hint active flags for all players.

        Called at question transition. Resets double_chance_active,
        double_chance_first_answer, and scrying_active to defaults.
        """
        for hs in self.hint_states.values():
            hs.double_chance_active = False
            hs.double_chance_first_answer = None
            hs.scrying_active = False
            hs.fifty_fifty_active = False

    def remove_player(self, user_id: int) -> bool:
        """Remove a player from the session entirely.

        Returns True if the player was removed, False if not found.
        """
        if user_id not in self.players:
            return False

        del self.players[user_id]
        self.disconnected_players.discard(user_id)
        if user_id in self.join_order:
            self.join_order.remove(user_id)
        return True

    def add_spectator(self, user_id: int) -> bool:
        """Add a spectator to the session.

        Returns True if the spectator was added, False if the spectator limit is reached
        or the user is already a spectator.
        """
        if len(self.spectators) >= MAX_SPECTATORS:
            return False
        if user_id in self.spectators:
            return False

        self.spectators.add(user_id)
        return True

    def remove_spectator(self, user_id: int) -> bool:
        """Remove a spectator from the session.

        Returns True if the spectator was removed, False if not found.
        """
        if user_id not in self.spectators:
            return False

        self.spectators.discard(user_id)
        return True

    def get_next_host(self) -> Optional[int]:
        """Get the next host based on join order.

        Returns the user_id of the earliest-joined connected player,
        or None if no connected players remain.
        """
        for user_id in self.join_order:
            if user_id in self.players and self.players[user_id].connected:
                return user_id
        return None

    def is_full(self) -> bool:
        """Check if the session has reached the maximum number of players."""
        return len(self.players) >= MAX_PLAYERS

    def submit_answer(self, user_id: int, option_index: int, question_number: int) -> Tuple[bool, Optional[str]]:
        """Validate and record a player's answer.

        Returns a tuple (success, error_message).
        """
        if self.game_state != GameState.QUESTION:
            return (False, "Not in question phase")

        if option_index < 0 or option_index > 3:
            return (False, "Invalid option index")

        if user_id in self.current_answers:
            return (False, "Already answered this question")

        if question_number != self.question_number + 1:
            return (False, "Question number mismatch")

        self.current_answers[user_id] = {
            "option_index": option_index,
            "timestamp": datetime.now(),
        }
        return (True, None)

    def calculate_rankings(self) -> list[dict]:
        """Return sorted scores. Players with the same score get the same rank."""
        player_data = []
        for user_id, score in self.scores.items():
            if user_id not in self.players:
                continue
            player_data.append({
                "user_id": user_id,
                "display_name": self.players[user_id].display_name,
                "score": score,
            })

        # Ordina semplicemente per punteggio dal più alto al più basso
        player_data.sort(key=lambda x: x["score"], reverse=True)

        rankings = []
        current_rank = 1
        
        for i, entry in enumerate(player_data):
            # Se non è il primo giocatore e ha lo stesso punteggio di quello precedente,
            # prende lo stesso rank. Altrimenti, il rank diventa la sua posizione corrente + 1.
            if i > 0 and entry["score"] == player_data[i - 1]["score"]:
                # Mantiene il rank del giocatore precedente
                pass 
            else:
                current_rank = i + 1

            rankings.append({
                "user_id": entry["user_id"],
                "display_name": entry["display_name"],
                "score": entry["score"],
                "rank": current_rank,
            })

        return rankings

    def transition_to_question(self) -> None:
        """Transition to QUESTION state.

        Sets game_state to QUESTION, clears current_answers,
        and sets question_start_time to now.
        Does NOT increment question_number.
        """
        self.reset_question_hint_state()
        self.game_state = GameState.QUESTION
        self.current_answers = {}
        self.question_start_time = datetime.now()

    def transition_to_waiting(self) -> dict:
        """Transition to WAITING state.

        Evaluates answers, updates scores.
        Returns a results dict for broadcasting.
        """
        from apps.gameplay.models import Question

        self.game_state = GameState.WAITING

        question_id = self.questions[self.question_number]
        question = Question.objects.get(id=question_id)

        option_map = {0: question.a, 1: question.b, 2: question.c, 3: question.d}

        # Evaluate each player's answer
        player_answers = []
        for user_id in self.players:
            statistics = Statistic.objects.get(user_id=user_id)
            if user_id in self.current_answers:
                answer_data = self.current_answers[user_id]
                selected_option_index = answer_data["option_index"]
                selected_text = option_map[selected_option_index]
                correct = selected_text == question.answer
                badge_su = False
                if correct:
                    statistics_correct_answers(statistics)
                    add_xp(5, statistics)
                    self.scores[user_id] = self.scores.get(user_id, 0) + 1
                else:
                    statistics_wrong_answer(statistics)
                    badge_su = self.still_undecided(user_id)
                player_answers.append({
                    "user_id": user_id,
                    "option_index": selected_option_index,
                    "correct": correct,
                    "badge_su": badge_su,
                })
            else:
                # Player did not answer — treated as incorrect
                statistics_wrong_answer(statistics)
                badge_su = self.still_undecided(user_id)
                player_answers.append({
                    "user_id": user_id,
                    "option_index": None,
                    "correct": False,
                    "badge_su": badge_su,
                })

        correct_option_index = None
        for idx, text in option_map.items():
            if text == question.answer:
                correct_option_index = idx
                break

        return {
            "correct_option_index": correct_option_index,
            "player_answers": player_answers,
        }

    def transition_to_end(self) -> list[dict]:
        """Transition to END_QUIZ state.

        Returns final rankings via calculate_rankings().
        """
        self.game_state = GameState.END_QUIZ
        try:
            rankings = self.calculate_rankings()
            asyncio.create_task(calculate_points(rankings))
        except Exception as e:
            print(f"Error calculating rankings: {e}")
            rankings = []
        return rankings

    def transition_to_paused(
        self,
        reason: str,
        elapsed_seconds: float,
        timer_duration: float,
        disconnected_names: list[str],
    ) -> None:
        """Transition to PAUSED state, storing pre-pause context.

        Records the current game_state so it can be restored on resume, and
        computes the remaining time as the timer duration minus the elapsed
        whole seconds, clamped to a minimum of 1 second.
        """
        self.pre_pause_state = self.game_state
        self.remaining_time = max(1, timer_duration - floor(elapsed_seconds))
        self.game_state = GameState.PAUSED
        self.pause_reason = reason
        self.paused_disconnected_names = disconnected_names
        self.paused_at = datetime.now()

    def transition_from_paused(self) -> Tuple[GameState, float]:
        """Resume from PAUSED state.

        Restores game_state to the pre-pause state, clears the stored pause
        context, and returns the (restored_state, remaining_time) tuple.
        """
        restored_state = self.pre_pause_state
        remaining_time = self.remaining_time

        self.game_state = self.pre_pause_state

        self.pre_pause_state = None
        self.remaining_time = None
        self.pause_reason = None
        self.paused_at = None
        self.paused_disconnected_names = []

        return (restored_state, remaining_time)

    def can_pause(self, user_id: int) -> Tuple[bool, Optional[str]]:
        """Check if a user can trigger a manual pause.

        Returns (allowed, error_code). Only the host may pause, and only while
        the game is in WAITING or NEXT_QUESTION state.
        """
        if user_id != self.host:
            return (False, "NOT_HOST")
        if self.game_state == GameState.PAUSED:
            return (False, "ALREADY_PAUSED")
        if self.game_state in (
            GameState.QUESTION,
            GameState.LOBBY,
            GameState.END_QUIZ,
            GameState.END_SESSION,
        ):
            return (False, "INVALID_STATE")
        return (True, None)

    def can_resume(self, user_id: int) -> Tuple[bool, Optional[str]]:
        """Check if a user can resume the game.

        Returns (allowed, error_code). Only the host may resume, the game must
        be PAUSED, and at least one connected player must remain.
        """
        if user_id != self.host:
            return (False, "NOT_HOST")
        if self.game_state != GameState.PAUSED:
            return (False, "NOT_PAUSED")
        if self.get_connected_player_count() == 0:
            return (False, "NO_CONNECTED_PLAYERS")
        return (True, None)

    def record_missed_question(self, user_id: int, question_number: int) -> None:
        """Record that a disconnected player missed a question."""
        self.missed_questions.setdefault(user_id, []).append(question_number)

    def get_connected_player_count(self) -> int:
        """Return the count of currently connected players."""
        return sum(1 for player in self.players.values() if player.connected)

    def migrate_host(self) -> Optional[int]:
        """Assign host to earliest-joined connected player.

        Returns the new host user_id or None if no connected players remain.
        """
        new_host = self.get_next_host()
        if new_host is not None:
            self.host = new_host
        return new_host

    def mark_disconnected(self, user_id: int) -> None:
        """Mark a player as disconnected.

        Sets connected=False, records disconnect_time, adds to disconnected_players set.
        """
        if user_id in self.players:
            self.players[user_id].connected = False
            self.players[user_id].disconnect_time = datetime.now()
            self.disconnected_players.add(user_id)

    def reconnect_player(self, user_id: int, channel_name: str) -> None:
        """Reconnect a previously disconnected player.

        Sets connected=True, clears disconnect_time, updates channel_name,
        removes from disconnected_players set.
        """
        if user_id in self.players:
            self.players[user_id].connected = True
            self.players[user_id].disconnect_time = None
            self.players[user_id].channel_name = channel_name
            self.disconnected_players.discard(user_id)
