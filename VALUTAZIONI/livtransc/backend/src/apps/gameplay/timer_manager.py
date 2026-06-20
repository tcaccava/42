"""
GameTimerManager: Manages asyncio tasks for session timers.

Handles question timers (20s), waiting periods (5s), reconnection timers (60s),
and cleanup timers (30s). Each timer is an asyncio task that sleeps for the
specified duration then calls a provided async callback.

All callbacks are idempotent — they should check current state before acting.
The timer manager is decoupled from game logic; it only handles scheduling
and cancellation.
"""

import asyncio
import logging
from typing import Awaitable, Callable

logger = logging.getLogger(__name__)


class GameTimerManager:
    """Manages asyncio timer tasks for multiplayer game sessions.

    Timers are stored in a dict keyed by (session_id, timer_type) where
    timer_type is a string like "question", "waiting", "reconnect_<user_id>",
    or "cleanup". This allows targeted cancellation of specific timers.
    """

    def __init__(self):
        self._timers: dict[tuple[str, str], asyncio.Task] = {}

    def start_question_timer(
        self,
        session_id: str,
        callback: Callable[[], Awaitable[None]],
        duration: int = 20,
    ) -> None:
        """Start a question timer (default 20s).

        When the timer expires, the callback is invoked. The callback should
        check if the session is still in QUESTION state before acting.

        Args:
            session_id: The session this timer belongs to.
            callback: Async function to call when the timer expires.
            duration: Timer duration in seconds (default 20).
        """
        timer_type = "question"
        self._cancel_existing(session_id, timer_type)
        task = asyncio.create_task(
            self._run_timer(session_id, timer_type, duration, callback)
        )
        self._timers[(session_id, timer_type)] = task

    def start_waiting_timer(
        self,
        session_id: str,
        callback: Callable[[], Awaitable[None]],
        duration: int = 10,
    ) -> None:
        """Start a waiting/display timer (default 5s).

        When the timer expires, the callback is invoked. The callback should
        check if the session is still in WAITING state before advancing.

        Args:
            session_id: The session this timer belongs to.
            callback: Async function to call when the timer expires.
            duration: Timer duration in seconds (default 5).
        """
        timer_type = "waiting"
        self._cancel_existing(session_id, timer_type)
        task = asyncio.create_task(
            self._run_timer(session_id, timer_type, duration, callback)
        )
        self._timers[(session_id, timer_type)] = task

    def start_reconnection_timer(
        self,
        session_id: str,
        user_id: int,
        callback: Callable[[], Awaitable[None]],
        duration: int = 60,
    ) -> None:
        """Start a reconnection timer for a specific player (default 60s).

        When the timer expires, the callback is invoked. The callback should
        check if the player is still in the disconnected set before removing.

        Args:
            session_id: The session this timer belongs to.
            user_id: The user_id of the disconnected player.
            callback: Async function to call when the timer expires.
            duration: Timer duration in seconds (default 60).
        """
        timer_type = f"reconnect_{user_id}"
        self._cancel_existing(session_id, timer_type)
        task = asyncio.create_task(
            self._run_timer(session_id, timer_type, duration, callback)
        )
        self._timers[(session_id, timer_type)] = task

    def start_cleanup_timer(
        self,
        session_id: str,
        callback: Callable[[], Awaitable[None]],
        duration: int = 30,
    ) -> None:
        """Start a session cleanup timer (default 30s).

        When the timer expires, the callback is invoked. The callback should
        check if the session still exists before removing it.

        Args:
            session_id: The session this timer belongs to.
            callback: Async function to call when the timer expires.
            duration: Timer duration in seconds (default 30).
        """
        timer_type = "cleanup"
        self._cancel_existing(session_id, timer_type)
        task = asyncio.create_task(
            self._run_timer(session_id, timer_type, duration, callback)
        )
        self._timers[(session_id, timer_type)] = task

    def cancel_timer(self, session_id: str, timer_type: str) -> bool:
        """Cancel a specific timer for a session.

        Args:
            session_id: The session the timer belongs to.
            timer_type: The type of timer (e.g., "question", "waiting",
                        "reconnect_<user_id>", "cleanup").

        Returns:
            True if a timer was found and cancelled, False otherwise.
        """
        key = (session_id, timer_type)
        task = self._timers.pop(key, None)
        if task is not None and not task.done():
            task.cancel()
            return True
        return False

    def cancel_all_session_timers(self, session_id: str) -> int:
        """Cancel all timers associated with a session.

        Args:
            session_id: The session whose timers should be cancelled.

        Returns:
            The number of timers that were cancelled.
        """
        keys_to_remove = [
            key for key in self._timers if key[0] == session_id
        ]
        cancelled_count = 0
        for key in keys_to_remove:
            task = self._timers.pop(key)
            if not task.done():
                task.cancel()
                cancelled_count += 1
        return cancelled_count

    def is_timer_active(self, session_id: str, timer_type: str) -> bool:
        """Check if a specific timer is currently active.

        Args:
            session_id: The session to check.
            timer_type: The type of timer to check.

        Returns:
            True if the timer exists and is not done, False otherwise.
        """
        key = (session_id, timer_type)
        task = self._timers.get(key)
        return task is not None and not task.done()

    def get_active_timer_count(self, session_id: str) -> int:
        """Get the number of active timers for a session.

        Args:
            session_id: The session to check.

        Returns:
            The number of active (not done) timers for the session.
        """
        count = 0
        for key, task in self._timers.items():
            if key[0] == session_id and not task.done():
                count += 1
        return count

    def _cancel_existing(self, session_id: str, timer_type: str) -> None:
        """Cancel an existing timer of the same type if one exists.

        This ensures only one timer of each type runs per session at a time.
        """
        key = (session_id, timer_type)
        existing = self._timers.pop(key, None)
        if existing is not None and not existing.done():
            existing.cancel()

    async def _run_timer(
        self,
        session_id: str,
        timer_type: str,
        duration: int,
        callback: Callable[[], Awaitable[None]],
    ) -> None:
        """Internal coroutine that sleeps then invokes the callback.

        Handles cancellation gracefully and cleans up the timer entry
        from the dict after completion.
        """
        try:
            await asyncio.sleep(duration)
            await callback()
        except asyncio.CancelledError:
            logger.debug(
                "Timer cancelled: session=%s, type=%s", session_id, timer_type
            )
        except Exception:
            logger.exception(
                "Timer callback error: session=%s, type=%s",
                session_id,
                timer_type,
            )
        finally:
            key = (session_id, timer_type)
            if key in self._timers:
                task = self._timers[key]
                if task.done():
                    del self._timers[key]


timer_manager = GameTimerManager()
