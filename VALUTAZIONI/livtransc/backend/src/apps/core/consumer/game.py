import json
from apps.gameplay.utils.game_utils import update_games
from apps.accounts.utils.multiplayer_badges import the_know_it_all
from channels.db import database_sync_to_async
from channels.generic.websocket import AsyncWebsocketConsumer
from apps.accounts.utils.multiplayer_badges import still_undecided_mp

class GameConsumer(AsyncWebsocketConsumer):
    """WebSocket consumer for remote multiplayer game sessions.

    Handles connection lifecycle, authentication, and message routing.
    Delegates all game logic to MultiplayerSession methods.
    """

    @database_sync_to_async
    def increment_lifeline(self, user_id):
        from apps.core.models import Statistic
        try:
            statistics, _ = Statistic.objects.get_or_create(user_id=user_id)
            statistics.lifeline_used += 1
            statistics.save()
        except Exception:
            pass

    async def connect(self):
        from apps.gameplay.multiplayer_session import active_sessions, GameState

        user_id = self.scope['session'].get('user_id')
        if not user_id:
            await self.accept()
            await self.send_error("Authentication required", "AUTH_REQUIRED")
            await self.close(code=4401)
            return

        self.session_id = self.scope['url_route']['kwargs']['session_id']
        self.user_id = user_id
        self.role = None
        if self.session_id not in active_sessions:
            await self.accept()
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            await self.close(code=4004)
            return

        session = active_sessions[self.session_id]

        if user_id in session.disconnected_players:
            await self.accept()
            await self.channel_layer.group_add(self.session_id,
                                               self.channel_name)

            session.reconnect_player(user_id, self.channel_name)
            self.role = 'player'

            host_connected = (session.host in session.players
                              and session.players[session.host].connected)
            host_reassigned = False
            if not host_connected:
                session.host = user_id
                host_reassigned = True

            await self.send_current_state(session, "player")

            display_name = session.players[user_id].display_name
            if host_reassigned:
                await self.broadcast_to_session({
                    "type": "host_migrated",
                    "payload": {
                        "new_host_id": user_id,
                        "display_name": display_name,
                    }
                })
            await self.broadcast_to_session({
                "type": "player_reconnected",
                "payload": {
                    "user_id": user_id,
                    "display_name": display_name,
                }
            })
            return
        await self.accept()
        await self.channel_layer.group_add(self.session_id, self.channel_name)

    async def disconnect(self, close_code):
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from apps.gameplay.timer_manager import timer_manager

        if not hasattr(self, 'session_id'):
            return

        session = active_sessions.get(self.session_id)
        if self.role == 'spectator':
            if session:
                session.remove_spectator(self.user_id)
            await self.channel_layer.group_discard(self.session_id,
                                                   self.channel_name)
            return

        if self.role == 'player' and session:
            intentional = close_code in (1000, 1001)
            if intentional:
                display_name = session.players[
                    self.
                    user_id].display_name if self.user_id in session.players else "Unknown"
                was_host = (session.host == self.user_id)
                session.remove_player(self.user_id)

                await self.broadcast_to_session({
                    "type": "player_left",
                    "payload": {
                        "user_id": self.user_id,
                        "display_name": display_name,
                    }
                })

                if was_host and session.get_connected_player_count() > 0:
                    new_host = session.migrate_host()
                    if new_host is not None:
                        await self.broadcast_to_session({
                            "type": "host_migrated",
                            "payload": {
                                "new_host_id":
                                new_host,
                                "display_name":
                                session.players[new_host].display_name,
                            },
                        })

                if session.game_state == GameState.LOBBY and len(
                        session.players) == 0:
                    del active_sessions[self.session_id]
                    timer_manager.cancel_all_session_timers(self.session_id)
                elif len(session.players) == 0:
                    # All players left intentionally in a non-LOBBY state —
                    # session is unreachable, tear it down.
                    session.game_state = GameState.END_SESSION
                    del active_sessions[self.session_id]
                    timer_manager.cancel_all_session_timers(self.session_id)

            else:
                from datetime import datetime

                display_name = session.players[
                    self.
                    user_id].display_name if self.user_id in session.players else "Unknown"
                session.mark_disconnected(self.user_id)
                await self.broadcast_to_session({
                    "type": "player_disconnected",
                    "payload": {
                        "user_id": self.user_id,
                        "display_name": display_name,
                    }
                })
                if session.host == self.user_id:
                    new_host = session.migrate_host()
                    if new_host is not None:
                        new_host_name = session.players[new_host].display_name
                        await self.broadcast_to_session({
                            "type": "host_migrated",
                            "payload": {
                                "new_host_id": new_host,
                                "display_name": new_host_name,
                            }
                        })

                active_states = (
                    GameState.QUESTION,
                    GameState.WAITING,
                    GameState.NEXT_QUESTION,
                )
                if session.game_state == GameState.PAUSED:
                    session.paused_disconnected_names.append(display_name)
                    await self.broadcast_to_session({
                        "type": "game_paused",
                        "payload": {
                            "reason":
                            session.pause_reason or "disconnect",
                            "disconnected_players":
                            list(session.paused_disconnected_names),
                            "pre_pause_state":
                            session.pre_pause_state.name
                            if session.pre_pause_state else None,
                            "remaining_time":
                            session.remaining_time,
                        }
                    })
                elif session.game_state in active_states:
                    if session.game_state == GameState.QUESTION:
                        timer_duration = 21
                        timer_type = "question"
                        if session.question_start_time:
                            elapsed = (
                                datetime.now() -
                                session.question_start_time).total_seconds()
                        else:
                            elapsed = 0
                    else:
                        timer_duration = 5
                        timer_type = "waiting"
                        elapsed = 0

                    session.transition_to_paused(
                        reason="disconnect",
                        elapsed_seconds=elapsed,
                        timer_duration=timer_duration,
                        disconnected_names=[display_name],
                    )
                    timer_manager.cancel_timer(self.session_id, timer_type)
                    await self.broadcast_to_session({
                        "type": "game_paused",
                        "payload": {
                            "reason":
                            "disconnect",
                            "disconnected_players":
                            list(session.paused_disconnected_names),
                            "pre_pause_state":
                            session.pre_pause_state.name
                            if session.pre_pause_state else None,
                            "remaining_time":
                            session.remaining_time,
                        }
                    })
                if (session.get_connected_player_count() == 0
                        and len(session.disconnected_players) == 0
                        and len(session.players) == 0):
                    session.game_state = GameState.END_SESSION
                    if self.session_id in active_sessions:
                        del active_sessions[self.session_id]
                    timer_manager.cancel_all_session_timers(self.session_id)
        await self.channel_layer.group_discard(self.session_id,
                                               self.channel_name)

    async def receive(self, text_data):
        """Parse JSON, enforce message size limit, and route by type field."""
        if len(text_data.encode('utf-8')) > 1024:
            await self.close(code=1009)
            return

        try:
            data = json.loads(text_data)
        except json.JSONDecodeError:
            await self.send_error("Invalid JSON format", "PARSE_ERROR")
            return
        msg_type = data.get('type')
        if not msg_type:
            await self.send_error("Missing message type", "MISSING_TYPE")
            return
        if msg_type == 'join':
            await self.handle_join(data)
        elif msg_type == 'spectate':
            await self.handle_spectate(data)
        elif msg_type == 'start':
            await self.handle_start(data)
        elif msg_type == 'answer':
            await self.handle_answer(data)
        elif msg_type == 'pause':
            await self.handle_pause(data)
        elif msg_type == 'resume':
            await self.handle_resume(data)
        elif msg_type == 'use_hint':
            await self.handle_use_hint(data)
        else:
            await self.send_error(f"Unknown message type: {msg_type}",
                                  "UNKNOWN_TYPE")

    async def handle_join(self, data):
        """Handle a player join request.

        Validates lobby state, session not full, adds player, broadcasts lobby_update.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState

        if self.role is not None:
            await self.send_error("Already joined", "ALREADY_JOINED")
            return

        session = active_sessions.get(self.session_id)
        if session is None:
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            await self.close(code=4004)
            return
        if session.game_state != GameState.LOBBY:
            await self.send_error("Game already started", "GAME_STARTED")
            return
        if session.is_full():
            await self.send_error("Session is full", "SESSION_FULL")
            await self.close(code=4001)
            return

        payload = data.get('payload', {})
        display_name = payload.get('display_name') if payload else None
        if not display_name:
            await self.send_error("Missing display name",
                                  "MISSING_DISPLAY_NAME")
            return

        success = session.add_player(self.user_id, display_name,
                                     self.channel_name)
        if not success:
            await self.send_error("Could not join session", "JOIN_FAILED")
            return

        self.role = 'player'
        await self.broadcast_lobby_update(session)

    async def handle_spectate(self, data):
        """Handle a spectator join request.

        Validates spectator limit, adds to spectator set, sends current state.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState

        if self.role is not None:
            await self.send_error("Already joined", "ALREADY_JOINED")
            return
        session = active_sessions.get(self.session_id)
        if session is None:
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            await self.close(code=4004)
            return
        if session.game_state == GameState.END_SESSION:
            await self.send_error("Session has ended", "SESSION_ENDED")
            await self.close(code=4002)
            return
        if len(session.spectators) >= 10:
            await self.send_error("Spectator limit reached", "SPECTATOR_LIMIT")
            await self.close(code=4003)
            return

        success = session.add_spectator(self.user_id)
        if not success:
            await self.send_error("Could not join as spectator",
                                  "SPECTATE_FAILED")
            return

        self.role = 'spectator'

        if session.game_state != GameState.LOBBY:
            await self.send_current_state(session, "spectator")
        else:
            await self.broadcast_lobby_update(session)

    async def handle_start(self, data):
        """Handle a game start request from the host.

        Validates host, ≥2 players, transitions to START, broadcasts game_start.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState

        if self.role == 'spectator':
            await self.send_error("Spectators cannot perform this action",
                                  "SPECTATOR_ONLY")
            return
        if self.role != 'player':
            await self.send_error("Must join first", "NOT_JOINED")
            return
        session = active_sessions.get(self.session_id)
        if session is None:
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            return
        if session.game_state != GameState.LOBBY:
            await self.send_error("Game already started", "GAME_STARTED")
            return
        if self.user_id != session.host:
            await self.send_error("Only the host can start the game",
                                  "NOT_HOST")
            return
        if len(session.players) < 2:
            await self.send_error("Need at least 2 players to start",
                                  "INSUFFICIENT_PLAYERS")
            return

        session.game_state = GameState.START
        await update_games(list(session.players.keys()))
        await self.broadcast_to_session({
            "type": "game_start",
            "payload": {
                "total_questions": 15,
            }
        })

        await self.start_game_loop()

    async def handle_answer(self, data):
        """Handle a player's answer submission.

        Validates QUESTION state, valid option_index, not already answered;
        handles Double Chance retry flow; records answer; sends scrying updates;
        checks if all connected players have answered (excluding mid-retry).
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from asgiref.sync import sync_to_async

        if self.role == 'spectator':
            await self.send_error("Spectators cannot perform this action",
                                  "SPECTATOR_ONLY")
            return
        if self.role != 'player':
            await self.send_error("Must join first", "NOT_JOINED")
            return
        session = active_sessions.get(self.session_id)
        if session is None:
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            return

        payload = data.get('payload', {})
        option_index = payload.get('option_index') if payload else None
        question_number = payload.get('question_number') if payload else None

        if option_index is None or question_number is None:
            await self.send_error("Missing answer data", "MISSING_ANSWER_DATA")
            return
        if session.game_state != GameState.QUESTION:
            await self.send_error("Question timer expired", "TIMER_EXPIRED")
            return

        # --- Double Chance: first incorrect answer → retry ---
        hint_state = session.hint_states.get(self.user_id)
        if (hint_state and hint_state.double_chance_active
                and self.user_id not in session.current_answers):
            if hint_state.double_chance_first_answer is None:
                is_correct = await sync_to_async(session.check_answer_correct
                                                 )(option_index)
                if not is_correct:
                    hint_state.double_chance_first_answer = option_index
                    await self.send(text_data=json.dumps({
                        "type": "double_chance_retry",
                        "payload": {
                            "incorrect_option_index": option_index
                        }
                    }))
                    # Send scrying update for this (wrong) first attempt
                    await self._send_scrying_updates(session, self.user_id,
                                                     option_index)
                    return
                else:
                    await self.send(text_data=json.dumps({
                        "type": "double_chance_success",
                        "payload": {
                            "correct_option_index": option_index
                        }
                    }))
            else:
                is_correct = await sync_to_async(session.check_answer_correct
                                                 )(option_index)
                if is_correct:
                    await self.send(text_data=json.dumps({
                        "type": "double_chance_success",
                        "payload": {
                            "correct_option_index": option_index
                        }
                    }))

        # --- Normal answer recording ---
        success, error_msg = session.submit_answer(self.user_id, option_index,
                                                   question_number)

        if not success:
            error_code_map = {
                "Not in question phase": "TIMER_EXPIRED",
                "Invalid option index": "INVALID_OPTION",
                "Already answered this question": "ALREADY_ANSWERED",
                "Question number mismatch": "QUESTION_MISMATCH",
            }
            code = error_code_map.get(error_msg, "ANSWER_ERROR")
            await self.send_error(error_msg, code)
            return

        # Send scrying updates for this answer
        await self._send_scrying_updates(session, self.user_id, option_index)

        answered_count = len(session.current_answers)
        connected_player_count = session.get_connected_player_count()
        await self.broadcast_to_session({
            "type": "answer_submitted",
            "payload": {
                "answered_count": answered_count,
                "total_players": connected_player_count,
            }
        })

        # --- Modified all-answered check: exclude mid-retry players ---
        all_connected_ids = {
            uid
            for uid, p in session.players.items() if p.connected
        }
        mid_retry_ids = {
            uid
            for uid in all_connected_ids if session.is_player_mid_retry(uid)
        }
        effectively_answered = all(uid in session.current_answers
                                   for uid in (all_connected_ids -
                                               mid_retry_ids))

        if effectively_answered and len(mid_retry_ids) == 0:
            await self.end_question()

    async def handle_pause(self, data):
        """Handle a manual pause request from the host.

        Validates via session.can_pause, freezes the active waiting timer,
        transitions to PAUSED, and broadcasts a manual game_paused notification.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from apps.gameplay.timer_manager import timer_manager

        session = active_sessions.get(self.session_id)
        if session is None:
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            return

        allowed, error_code = session.can_pause(self.user_id)
        if not allowed:
            error_messages = {
                "NOT_HOST": "Only the host can perform this action",
                "ALREADY_PAUSED": "Game is already paused",
                "INVALID_STATE": "Cannot pause in current game state",
            }
            await self.send_error(
                error_messages.get(error_code, "Cannot pause"), error_code)
            return

        session.transition_to_paused(
            reason="manual",
            elapsed_seconds=0,
            timer_duration=5,
            disconnected_names=[],
        )
        timer_manager.cancel_timer(self.session_id, "waiting")

        await self.broadcast_to_session({
            "type": "game_paused",
            "payload": {
                "reason":
                "manual",
                "disconnected_players":
                list(session.paused_disconnected_names),
                "pre_pause_state":
                session.pre_pause_state.name
                if session.pre_pause_state else None,
                "remaining_time":
                session.remaining_time,
            }
        })

    async def handle_resume(self, data):
        """Handle a resume request from the host.

        Validates via session.can_resume, restores the pre-pause state,
        restarts the appropriate timer with the stored remaining time, and
        broadcasts a game_resumed notification.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from apps.gameplay.timer_manager import timer_manager

        session = active_sessions.get(self.session_id)
        if session is None:
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            return

        allowed, error_code = session.can_resume(self.user_id)
        if not allowed:
            error_messages = {
                "NOT_HOST": "Only the host can perform this action",
                "NOT_PAUSED": "Game is not paused",
                "NO_CONNECTED_PLAYERS":
                "Cannot resume with no connected players",
            }
            await self.send_error(
                error_messages.get(error_code, "Cannot resume"), error_code)
            return

        restored_state, remaining_time = session.transition_from_paused()

        if restored_state == GameState.QUESTION:
            timer_manager.start_question_timer(self.session_id,
                                               self._on_question_timeout,
                                               duration=remaining_time)
            await self.broadcast_question(session)
        elif restored_state in (GameState.WAITING, GameState.NEXT_QUESTION):
            timer_manager.start_waiting_timer(self.session_id,
                                              self._on_waiting_timeout,
                                              duration=remaining_time)

        await self.broadcast_to_session({
            "type": "game_resumed",
            "payload": {
                "restored_state":
                restored_state.name if restored_state else None,
                "remaining_time": remaining_time,
            }
        })

    async def handle_use_hint(self, data):
        """Handle a hint activation request.

        Validates role, session, preconditions; dispatches to the appropriate
        hint activation method; sends unicast result back to the requesting player.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from asgiref.sync import sync_to_async

        # 1. Role check
        if self.role == 'spectator':
            await self.send_error("Spectators cannot use hints",
                                  "SPECTATOR_ONLY")
            return
        if self.role != 'player':
            await self.send_error("Must join first", "NOT_JOINED")
            return

        session = active_sessions.get(self.session_id)
        if session is None:
            await self.send_error("Session not found", "SESSION_NOT_FOUND")
            return

        # 2. Check player is connected
        if self.user_id in session.disconnected_players:
            await self.send_error("Player is disconnected", "NOT_CONNECTED")
            return

        # 3. Extract hint name
        payload = data.get('payload', {})
        hint_name = payload.get('hint') if payload else None
        if hint_name not in ('fifty_fifty', 'double_chance', 'scrying'):
            await self.send_error("Invalid hint name", "INVALID_HINT")
            return

        # 4. Validate via session
        allowed, error_code = session.validate_hint_request(
            self.user_id, hint_name)
        if not allowed:
            await self.send_error(f"Cannot use hint: {error_code}", error_code)
            return

        # Increment lifeline statistic on successful activation
        await self.increment_lifeline(self.user_id)

        # 5. Dispatch by hint type
        if hint_name == 'fifty_fifty':
            eliminated = await sync_to_async(session.activate_fifty_fifty
                                             )(self.user_id)
            await self.send(text_data=json.dumps({
                "type": "hint_result",
                "payload": {
                    "hint": "fifty_fifty",
                    "success": True,
                    "eliminated": eliminated,
                }
            }))

        elif hint_name == 'double_chance':
            session.activate_double_chance(self.user_id)
            await self.send(text_data=json.dumps({
                "type": "hint_result",
                "payload": {
                    "hint": "double_chance",
                    "success": True,
                }
            }))

        elif hint_name == 'scrying':
            session.activate_scrying(self.user_id)
            # Send current answers already submitted by other players
            existing_answers = []
            for uid, answer_data in session.current_answers.items():
                if uid != self.user_id and uid in session.players:
                    existing_answers.append({
                        "user_id":
                        uid,
                        "display_name":
                        session.players[uid].display_name,
                        "option_index":
                        answer_data["option_index"],
                    })
            await self.send(text_data=json.dumps({
                "type": "hint_result",
                "payload": {
                    "hint": "scrying",
                    "success": True,
                    "existing_answers": existing_answers,
                }
            }))

    async def _on_question_timeout(self):
        """Question timer callback. Ends the question if still in QUESTION state."""
        from apps.gameplay.multiplayer_session import active_sessions as _sessions, GameState
        sess = _sessions.get(self.session_id)
        if sess is None:
            return
        if sess.game_state != GameState.QUESTION:
            return
        await self.end_question()

    async def _on_waiting_timeout(self):
        """Waiting timer callback. Ends the waiting period if still in WAITING state."""
        from apps.gameplay.multiplayer_session import active_sessions as _sessions, GameState
        sess = _sessions.get(self.session_id)
        if sess is None:
            return
        if sess.game_state != GameState.WAITING:
            return
        await self.end_waiting()

    async def _send_scrying_updates(self, session, answering_user_id: int,
                                    option_index: int):
        """Send scrying_update to all players with active Scrying (excluding the answerer)."""
        scrying_players = session.get_scrying_players()
        if not scrying_players:
            return
        display_name = session.players[answering_user_id].display_name
        for scrying_uid in scrying_players:
            if scrying_uid == answering_user_id:
                continue
            if scrying_uid not in session.players:
                continue
            channel_name = session.players[scrying_uid].channel_name
            await self.channel_layer.send(
                channel_name, {
                    "type":
                    "game.update",
                    "message":
                    json.dumps({
                        "type": "scrying_update",
                        "payload": {
                            "user_id": answering_user_id,
                            "display_name": display_name,
                            "option_index": option_index,
                        }
                    })
                })

    async def start_game_loop(self):
        """Start the game loop by broadcasting the first question.

        Transitions to QUESTION state and starts the question timer.
        Will be fully implemented in Task 4.3.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from apps.gameplay.timer_manager import timer_manager

        session = active_sessions.get(self.session_id)
        if session is None:
            return

        session.transition_to_question()

        await self.broadcast_question(session)

        timer_manager.start_question_timer(self.session_id,
                                           self._on_question_timeout,
                                           duration=21)

    async def end_question(self):
        """End the current question, transition to WAITING, broadcast results.

        Will be fully implemented in Task 4.3.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from apps.gameplay.timer_manager import timer_manager
        from asgiref.sync import sync_to_async
        from datetime import datetime

        session = active_sessions.get(self.session_id)
        if session is None:
            return

        # Record missed questions for players who are disconnected during this
        # QUESTION -> WAITING transition. These questions score zero (null) and
        # are tracked separately so they are not counted as wrong answers.
        for uid in list(session.disconnected_players):
            session.record_missed_question(uid, session.question_number)

        # Handle mid-retry players (Double Chance): record their first (wrong)
        # answer as the final answer since the timer expired before they could
        # submit a second attempt.
        for uid in list(session.players.keys()):
            if session.is_player_mid_retry(uid):
                hint_state = session.hint_states[uid]
                session.current_answers[uid] = {
                    "option_index": hint_state.double_chance_first_answer,
                    "timestamp": datetime.now(),
                }

        results = await sync_to_async(session.transition_to_waiting)()
        await (the_know_it_all)(results["player_answers"])
        await (still_undecided_mp)(results["player_answers"])

        timer_manager.cancel_timer(self.session_id, "question")

        await self.broadcast_to_session({
            "type": "answer_result",
            "payload": results,
        })

        scores_list = [{
            "user_id": uid,
            "display_name": session.players[uid].display_name,
            "score": score,
        } for uid, score in session.scores.items() if uid in session.players]
        scores_list.sort(key=lambda x: -x["score"])

        await self.broadcast_to_session({
            "type": "scores_update",
            "payload": {
                "scores": scores_list,
                "question_number": session.question_number,
            }
        })

        timer_manager.start_waiting_timer(self.session_id,
                                          self._on_waiting_timeout,
                                          duration=5)

    async def end_waiting(self):
        """End the waiting period. Advance to next question or end game.

        Will be fully implemented in Task 4.3.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from apps.gameplay.timer_manager import timer_manager

        session = active_sessions.get(self.session_id)
        if session is None:
            return

        if session.question_number < 14:
            session.question_number += 1
            session.transition_to_question()
            await self.broadcast_question(session)
            timer_manager.start_question_timer(self.session_id,
                                               self._on_question_timeout,
                                               duration=21)
        else:
            await self.end_game()

    async def end_game(self):
        """End the game, broadcast final rankings, start cleanup timer.

        Will be fully implemented in Task 4.3.
        """
        from apps.gameplay.multiplayer_session import active_sessions, GameState
        from apps.gameplay.timer_manager import timer_manager

        session = active_sessions.get(self.session_id)
        if session is None:
            return

        # Never end the game while paused. The game only ends through normal
        # completion of all questions, not while frozen waiting for a resume.
        if session.game_state == GameState.PAUSED:
            return

        rankings = session.transition_to_end()

        await self.broadcast_to_session({
            "type": "game_end",
            "payload": {
                "rankings": rankings,
            }
        })

        sid = self.session_id

        async def on_cleanup_timeout():
            from apps.gameplay.multiplayer_session import active_sessions as _sessions
            sess = _sessions.get(sid)
            if sess is None:
                return
            sess.game_state = GameState.END_SESSION
            if sid in _sessions:
                del _sessions[sid]
            timer_manager.cancel_all_session_timers(sid)

        timer_manager.start_cleanup_timer(self.session_id,
                                          on_cleanup_timeout,
                                          duration=30)

    async def broadcast_lobby_update(self, session):
        """Broadcast lobby_update message to all clients in the session."""
        players_list = [{
            "user_id": p.user_id,
            "display_name": p.display_name,
            "connected": p.connected,
        } for p in session.players.values()]
        await self.broadcast_to_session({
            "type": "lobby_update",
            "payload": {
                "players": players_list,
                "spectator_count": len(session.spectators),
                "host_id": session.host,
            }
        })

    async def broadcast_question(self, session):
        """Broadcast the current question to all clients in the session."""
        from asgiref.sync import sync_to_async
        from apps.gameplay.models import Question

        question_id = session.questions[session.question_number]
        question = await sync_to_async(Question.objects.get)(id=question_id)

        await self.broadcast_to_session({
            "type": "question",
            "payload": {
                "text": question.text,
                "options": [question.a, question.b, question.c, question.d],
                "question_number": session.question_number + 1,
                "total_questions": 15,
            }
        })

    async def game_update(self, event):
        """Channel layer handler for game.update messages.

        Sends the message payload to the WebSocket client.
        """
        message = event.get("message")
        if message:
            await self.send(text_data=message)

    async def send_error(self, reason: str, code: str):
        """Send an error message to the client."""
        await self.send(text_data=json.dumps({
            "type": "error",
            "payload": {
                "reason": reason,
                "code": code,
            }
        }))

    async def broadcast_to_session(self, message: dict):
        """Broadcast a message to all clients in the session group."""
        await self.channel_layer.group_send(self.session_id, {
            "type": "game.update",
            "message": json.dumps(message),
        })

    async def send_current_state(self, session, role: str):
        """Send the current game state to a reconnecting player or spectator.

        Sends enough information for the client to render the correct phase.
        """
        from apps.gameplay.multiplayer_session import GameState
        from datetime import datetime

        state_data = {
            "type": "state_sync",
            "payload": {
                "your_role":
                role,
                "game_state":
                session.game_state.name,
                "question_number":
                session.question_number,
                "scores": {
                    str(uid): score
                    for uid, score in session.scores.items()
                },
                "players": [{
                    "user_id": p.user_id,
                    "display_name": p.display_name,
                    "connected": p.connected,
                } for p in session.players.values()],
                "host_id":
                session.host,
                "spectator_count":
                len(session.spectators),
            }
        }

        if session.game_state == GameState.QUESTION:
            from apps.gameplay.models import Question
            from asgiref.sync import sync_to_async

            if session.question_start_time:
                elapsed = (datetime.now() -
                           session.question_start_time).total_seconds()
                time_remaining = max(0, 20 - elapsed)
                state_data["payload"]["time_remaining"] = round(
                    time_remaining, 1)

            # Include question data for spectators joining during play
            question_id = session.questions[session.question_number]
            question = await sync_to_async(Question.objects.get)(id=question_id
                                                                 )
            state_data["payload"]["question"] = {
                "text": question.text,
                "options": [question.a, question.b, question.c, question.d],
            }
            state_data["payload"]["total_questions"] = 15

        if session.game_state == GameState.PAUSED:
            disconnected_names = [
                session.players[uid].display_name
                for uid in session.disconnected_players
                if uid in session.players
            ]
            state_data["payload"]["remaining_time"] = session.remaining_time
            state_data["payload"]["pause_reason"] = session.pause_reason
            state_data["payload"]["pre_pause_state"] = (
                session.pre_pause_state.name
                if session.pre_pause_state else None)
            state_data["payload"]["paused_disconnected_names"] = list(
                session.paused_disconnected_names)
            state_data["payload"][
                "disconnected_player_names"] = disconnected_names

        await self.send(text_data=json.dumps(state_data))
