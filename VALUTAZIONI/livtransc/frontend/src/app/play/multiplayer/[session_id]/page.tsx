"use client";

import { useState, useEffect, useRef, useCallback } from "react";
import { useParams, useRouter, useSearchParams } from "next/navigation";
import { useUsername } from "@/hooks/navbarDatas";
import { useTranslation } from "@/hooks/useTranslation";
import LobbyScreen from "../components/LobbyScreen";
import GameUI from "../components/GameUI";
import LoadingScreen from "../components/LoadingScreen";
import RankingScreen from "../components/RankingScreen";
import ResultsScreen from "../components/ResultsScreen";
import PauseOverlay from "../components/PauseOverlay";
import PauseButton from "../components/PauseButton";

// --- Types ---

type GamePhase =
  | "connecting"
  | "lobby"
  | "playing"
  | "loading"
  | "ranking"
  | "results"
  | "paused"
  | "disconnected"
  | "error";

interface PlayerInfo {
  user_id: number;
  display_name: string;
  connected: boolean;
}

interface Question {
  text: string;
  options: string[];
  number: number;
  total: number;
}

interface ScoreEntry {
  user_id: number;
  display_name: string;
  score: number;
  rank: number;
}

interface GameState {
  phase: GamePhase;
  players: PlayerInfo[];
  spectators: number;
  question: Question | null;
  scores: ScoreEntry[];
  timeRemaining: number;
  isHost: boolean;
  myUserId: number;
  answeredPlayers: number[];
  isSpectator: boolean;
  correctAnswers: number;
  playerAnswers: Record<string, string>;
  pauseReason: "disconnect" | "manual" | null;
  disconnectedPlayerNames: string[];
  hints: { fifty_fifty: boolean; double_chance: boolean; scrying: boolean };
  scryingActive: boolean;
  scryingAnswers: Record<string, string>;
  disabledOptions: number[];
  doubleChanceRetry: boolean;
}

// --- Component ---

export default function MultiplayerSessionPage() {
  const params = useParams();
  const router = useRouter();
  const searchParams = useSearchParams();
  const isSpectatorParam = searchParams.get("spectator") === "true";
  const username = useUsername();
  const { t } = useTranslation();

  const sessionId = params.session_id as string;

  const wsRef = useRef<WebSocket | null>(null);
  const reconnectAttemptsRef = useRef(0);
  const reconnectTimeoutRef = useRef<NodeJS.Timeout | null>(null);
  const timerIntervalRef = useRef<NodeJS.Timeout | null>(null);
  const intentionalCloseRef = useRef(false);
  const gameStartedRetryRef = useRef(false);

  const [gameState, setGameState] = useState<GameState>({
    phase: "connecting",
    players: [],
    spectators: 0,
    question: null,
    scores: [],
    timeRemaining: 20,
    isHost: false,
    myUserId: 0,
    answeredPlayers: [],
    isSpectator: isSpectatorParam,
    correctAnswers: 0,
    playerAnswers: {},
    pauseReason: null,
    disconnectedPlayerNames: [],
    hints: { fifty_fifty: true, double_chance: true, scrying: true },
    scryingActive: false,
    scryingAnswers: {},
    disabledOptions: [],
    doubleChanceRetry: false,
  });

  const [isLocked, setIsLocked] = useState(false);
  const [optionStatuses, setOptionStatuses] = useState<
    Record<number, "green" | "red" | "yellow" | "none">
  >({});
  const [errorMessage, setErrorMessage] = useState<string | null>(null);
  const [noticeMessage, setNoticeMessage] = useState<string | null>(null);

  const setTerminalError = useCallback((message: string) => {
    reconnectAttemptsRef.current = 3;
    if (reconnectTimeoutRef.current) {
      clearTimeout(reconnectTimeoutRef.current);
      reconnectTimeoutRef.current = null;
    }
    intentionalCloseRef.current = true;
    setGameState((prev) => ({ ...prev, phase: "error" }));
    setErrorMessage(message);
  }, []);

  // --- Timer ---

  const startTimer = useCallback((duration: number) => {
    if (timerIntervalRef.current) {
      clearInterval(timerIntervalRef.current);
    }
    setGameState((prev) => ({ ...prev, timeRemaining: duration }));
    timerIntervalRef.current = setInterval(() => {
      setGameState((prev) => {
        const next = prev.timeRemaining - 1;
        if (next <= 0) {
          if (timerIntervalRef.current) {
            clearInterval(timerIntervalRef.current);
            timerIntervalRef.current = null;
          }
          return { ...prev, timeRemaining: 0 };
        }
        return { ...prev, timeRemaining: next };
      });
    }, 1000);
  }, []);

  const stopTimer = useCallback(() => {
    if (timerIntervalRef.current) {
      clearInterval(timerIntervalRef.current);
      timerIntervalRef.current = null;
    }
  }, []);

  // --- WebSocket Message Handlers ---

  const handleMessage = useCallback(
    (event: MessageEvent) => {
      const data = JSON.parse(event.data);
      const { type, payload } = data;

      switch (type) {
        case "lobby_update":
          setGameState((prev) => ({
            ...prev,
            phase: "lobby",
            players: payload.players || [],
            spectators: payload.spectator_count || 0,
            isHost:
              payload.host_id ===
              Number(localStorage.getItem("user_id") || "0"),
          }));
          break;

        case "game_start":
          setGameState((prev) => ({
            ...prev,
            phase: "loading",
          }));
          break;

        case "question":
          setIsLocked(false);
          setOptionStatuses({});
          setGameState((prev) => ({
            ...prev,
            phase: "playing",
            question: {
              text: payload.text,
              options: payload.options,
              number: payload.question_number,
              total: payload.total_questions,
            },
            answeredPlayers: [],
            isSpectator: prev.isSpectator,
            playerAnswers: {},
            scryingActive: false,
            scryingAnswers: {},
            disabledOptions: [],
            doubleChanceRetry: false,
          }));
          startTimer(20);
          break;

        case "state_sync":
          // Spectator rejoining or late-join receiving game state
          const gameStateMap: Record<string, GamePhase> = {
            LOBBY: "lobby",
            START: "loading",
            QUESTION: "playing",
            WAITING: "loading",
            NEXT_QUESTION: "loading",
            END_QUIZ: "results",
            PAUSED: "paused",
            END_SESSION: "results",
          };
          const newPhase = gameStateMap[payload.game_state] || "connecting";
          const syncQuestion = newPhase === "playing" && payload.question
            ? {
                text: payload.question.text,
                options: payload.question.options,
                number: payload.question_number + 1,
                total: payload.total_questions,
              }
            : null;
          const syncIsPaused = payload.game_state === "PAUSED";
          setGameState((prev) => ({
            ...prev,
            phase: newPhase,
            players: payload.players || [],
            spectators: payload.spectator_count || 0,
            question: syncQuestion,
            scores: payload.scores ? Object.entries(payload.scores).map(([uid, score]) => ({
              user_id: parseInt(uid),
              display_name: payload.players?.find((p: PlayerInfo) => p.user_id === parseInt(uid))?.display_name || "Unknown",
              score: score as number,
              rank: 0,
            })) : [],
            isHost: payload.host_id === gameState.myUserId,
            isSpectator: payload.your_role
              ? payload.your_role === "spectator"
              : prev.isSpectator,
            correctAnswers: prev.correctAnswers || 0,
            pauseReason: syncIsPaused
              ? (payload.pause_reason as "disconnect" | "manual" | null) || "disconnect"
              : prev.pauseReason,
            disconnectedPlayerNames: syncIsPaused
              ? payload.disconnected_player_names ||
                payload.paused_disconnected_names ||
                []
              : prev.disconnectedPlayerNames,
          }));
          if (newPhase === "playing" && payload.time_remaining !== undefined) {
            startTimer(payload.time_remaining);
          }
          break;

        case "answer_result":
          stopTimer();
          if (!gameState.isSpectator) {
            setIsLocked(true);
          }
          // Build color statuses (applied after 1s)
          const statuses: Record<number, "green" | "red" | "yellow" | "none"> =
            {};
          const myUserId = Number(
            localStorage.getItem("user_id") || "0"
          );
          const myAnswer = payload.player_answers?.find(
            (a: { user_id: number; option_index: number; correct: boolean }) =>
              a.user_id === myUserId
          );
          if (payload.correct_option_index !== undefined) {
            statuses[payload.correct_option_index] = "green";
          }
          if (
            myAnswer &&
            myAnswer.option_index !== payload.correct_option_index
          ) {
            statuses[myAnswer.option_index] = "red";
          } else if (
            myAnswer &&
            myAnswer.option_index === payload.correct_option_index
          ) {
            statuses[myAnswer.option_index] = "green";
          }
          const increment = myAnswer && myAnswer.correct ? 1 : 0;
          // Show name badges immediately
          setGameState((prev) => {
            const mapping: Record<string, string> = { ...(prev.playerAnswers || {}) };
            if (Array.isArray(payload.player_answers)) {
              payload.player_answers.forEach((a: { user_id: number; option_index: number | null }) => {
                if (a.option_index === null || a.option_index === undefined) return;
                const player = prev.players?.find((p) => p.user_id === a.user_id);
                const name = player?.display_name || `User${a.user_id}`;
                mapping[name] = String.fromCharCode(65 + a.option_index);
              });
            }
            return { ...prev, playerAnswers: mapping };
          });
          // After 1s: reveal colors and update sidebar score bar
          setTimeout(() => {
            setOptionStatuses(statuses);
            setGameState((prev) => ({
              ...prev,
              correctAnswers: (prev.correctAnswers || 0) + increment,
            }));
          }, 1000);
          break;

        case "scores_update": {
          const rawScores = payload.scores || [];
          const scores = rawScores.map(
            (s: { user_id: number; display_name: string; score: number }) => {
                const correctRank = rawScores.findIndex((item: any) => item.score === s.score) + 1;
                return {
                  ...s,
                  rank: correctRank,
                };
              }
          );
          // Stay on the current question view for 2s so the player
          // can see the green/red answer feedback before ranking appears
          setTimeout(() => {
            setGameState((prev) => ({
              ...prev,
              phase: "ranking",
              scores,
              isSpectator: prev.isSpectator,
            }));
          }, 2000);
          break;
        }

        case "game_end":
          stopTimer();
          setGameState((prev) => ({
            ...prev,
            phase: "results",
            scores: payload.rankings || [],
          }));
          break;

        case "player_left":
          setGameState((prev) => ({
            ...prev,
            players: prev.players.filter((p) => p.user_id !== payload.user_id),
          }));
          break;

        case "player_disconnected":
          setGameState((prev) => ({
            ...prev,
            players: prev.players.map((p) =>
              p.user_id === payload.user_id ? { ...p, connected: false } : p
            ),
          }));
          break;

        case "player_reconnected": {
          const reconnectedPlayer = gameState.players.find(
            (p) => p.user_id === payload.user_id
          );
          const reconnectedName =
            payload.display_name ||
            reconnectedPlayer?.display_name ||
            `User${payload.user_id}`;
          setGameState((prev) => ({
            ...prev,
            players: prev.players.map((p) =>
              p.user_id === payload.user_id ? { ...p, connected: true } : p
            ),
          }));
          setNoticeMessage(`${reconnectedName} reconnected`);
          setTimeout(() => setNoticeMessage(null), 5000);
          break;
        }

        case "game_paused":
          stopTimer();
          setGameState((prev) => ({
            ...prev,
            phase: "paused",
            pauseReason:
              (payload.reason as "disconnect" | "manual") || "disconnect",
            disconnectedPlayerNames: payload.disconnected_players || [],
          }));
          break;

        case "game_resumed": {
          const restoredPhase: GamePhase =
            payload.restored_state === "QUESTION" ? "playing" : "ranking";
          setGameState((prev) => ({
            ...prev,
            phase: restoredPhase,
            pauseReason: null,
            disconnectedPlayerNames: [],
          }));
          if (
            restoredPhase === "playing" &&
            payload.remaining_time !== undefined
          ) {
            startTimer(payload.remaining_time);
          }
          break;
        }

        case "host_migrated": {
          const myUserId = Number(localStorage.getItem("user_id") || "0");
          setGameState((prev) => ({
            ...prev,
            isHost: payload.new_host_id === myUserId,
          }));
          setNoticeMessage(
            `${payload.display_name || "A player"} is now the host`
          );
          setTimeout(() => setNoticeMessage(null), 5000);
          break;
        }

        case "player_answered":
          setGameState((prev) => ({
            ...prev,
            answeredPlayers: [...prev.answeredPlayers, payload.user_id],
          }));
          break;

        case "hint_result": {
          const hintName = payload.hint as "fifty_fifty" | "double_chance" | "scrying";
          setGameState((prev) => ({
            ...prev,
            hints: { ...prev.hints, [hintName]: false },
            ...(hintName === "scrying" ? { scryingActive: true } : {}),
          }));
          if (hintName === "fifty_fifty" && payload.eliminated) {
            setGameState((prev) => ({
              ...prev,
              disabledOptions: [...prev.disabledOptions, ...payload.eliminated],
            }));
          }
          if (hintName === "scrying" && payload.existing_answers) {
            const answers: Record<string, string> = {};
            payload.existing_answers.forEach((a: { display_name: string; option_index: number }) => {
              answers[a.display_name] = String.fromCharCode(65 + a.option_index);
            });
            setGameState((prev) => ({ ...prev, scryingAnswers: answers }));
          }
          break;
        }

        case "scrying_update": {
          const letter = String.fromCharCode(65 + payload.option_index);
          setGameState((prev) => ({
            ...prev,
            scryingAnswers: { ...prev.scryingAnswers, [payload.display_name]: letter },
          }));
          break;
        }

        case "double_chance_retry": {
          setIsLocked(false);
          setGameState((prev) => ({
            ...prev,
            disabledOptions: [...prev.disabledOptions, payload.incorrect_option_index],
            doubleChanceRetry: true,
          }));
          setOptionStatuses((prev) => ({ ...prev, [payload.incorrect_option_index]: "red" }));
          setNoticeMessage("Wrong! You have one more chance.");
          setTimeout(() => setNoticeMessage(null), 3000);
          break;
        }

        case "double_chance_success": {
          setOptionStatuses((prev) => ({ ...prev, [payload.correct_option_index]: "green" }));
          break;
        }

        case "error":
          // Suppress spurious toast on reconnect
          if (payload.code === "ALREADY_JOINED") {
            return;
          }
          if (payload.code === "SESSION_NOT_FOUND" || payload.code === "SESSION_ENDED") {
            setTerminalError(payload.reason || "This session is no longer available.");
            return;
          }
          // If game already started on join attempt, auto-retry as spectator
          if (payload.code === "GAME_STARTED" && !gameStartedRetryRef.current) {
            gameStartedRetryRef.current = true;
            setGameState((prev) => ({ ...prev, isSpectator: true }));
            if (wsRef.current?.readyState === WebSocket.OPEN) {
              wsRef.current.send(JSON.stringify({ type: "spectate", payload: {} }));
            }
            return; // Don't show error toast
          }

          setErrorMessage(payload.reason || "An error occurred");
          // Auto-dismiss after 5 seconds
          setTimeout(() => setErrorMessage(null), 5000);
          break;

        default:
          break;
      }
    },
    [setTerminalError, startTimer, stopTimer]
  );

  // --- WebSocket Connection ---

  const connectWebSocket = useCallback(() => {
		if (wsRef.current?.readyState === WebSocket.OPEN) return;

		const WS_BASE = (process.env.NEXT_PUBLIC_API_URL!).replace('https', 'wss');
    const ws = new WebSocket(`${WS_BASE}/ws/game/${sessionId}/`);
    wsRef.current = ws;

    ws.onopen = () => {
      reconnectAttemptsRef.current = 0;
      setGameState((prev) => ({ ...prev, phase: "connecting" }));

      if (isSpectatorParam) {
        ws.send(
          JSON.stringify({
            type: "spectate",
            payload: {},
          })
        );
      } else {
        const displayName = username || "Player";
        ws.send(
          JSON.stringify({
            type: "join",
            payload: { display_name: displayName },
          })
        );
      }

      // Set myUserId from localStorage
      const userId = Number(localStorage.getItem("user_id") || "0");
      setGameState((prev) => ({ ...prev, myUserId: userId }));
    };

    ws.onmessage = handleMessage;

    ws.onclose = (event) => {
      if (intentionalCloseRef.current) return;

      if (event.code === 4004 || event.code === 4002 || event.code === 4401) {
        setTerminalError("This multiplayer session no longer exists or has already ended.");
        return;
      }

      // Abnormal close — attempt reconnection
      const isAbnormal = event.code !== 1000 && event.code !== 1001;
      if (isAbnormal && reconnectAttemptsRef.current < 3) {
        setGameState((prev) => ({ ...prev, phase: "disconnected" }));
        const delay = Math.pow(2, reconnectAttemptsRef.current) * 1000; // 1s, 2s, 4s
        reconnectAttemptsRef.current += 1;
        reconnectTimeoutRef.current = setTimeout(() => {
          connectWebSocket();
        }, delay);
      } else if (isAbnormal) {
        // Max attempts reached
        setGameState((prev) => ({ ...prev, phase: "disconnected" }));
        setErrorMessage("Connection lost. Please refresh the page.");
      }
    };

    ws.onerror = () => {
      // Error will trigger onclose
    };
  }, [handleMessage, sessionId, setTerminalError, username, isSpectatorParam]);

  // --- Effects ---

  useEffect(() => {
    // Wait for username to be loaded before connecting
    if (!username) return;

    connectWebSocket();

    return () => {
      intentionalCloseRef.current = true;
      stopTimer();
      if (reconnectTimeoutRef.current) {
        clearTimeout(reconnectTimeoutRef.current);
      }
      if (wsRef.current) {
        wsRef.current.close(1000);
        wsRef.current = null;
      }
    };
  }, [connectWebSocket, username, stopTimer]);

  // --- User Actions ---

  const sendMessage = useCallback(
    (type: string, payload: Record<string, unknown> = {}) => {
      if (wsRef.current?.readyState === WebSocket.OPEN) {
        wsRef.current.send(JSON.stringify({ type, payload }));
      }
    },
    []
  );

  const handleStart = useCallback(() => {
    sendMessage("start");
  }, [sendMessage]);

  const handlePause = useCallback(() => {
    sendMessage("pause");
  }, [sendMessage]);

  const handleResume = useCallback(() => {
    sendMessage("resume");
  }, [sendMessage]);

  const handleAnswer = useCallback(
    (optionIndex: number) => {
      if (isLocked || !gameState.question || gameState.isSpectator) return;
      setIsLocked(true);
      // Mark selected option as yellow (pending)
      setOptionStatuses((prev) => ({ ...prev, [optionIndex]: "yellow" }));
      sendMessage("answer", {
        option_index: optionIndex,
        question_number: gameState.question.number,
      });
    },
    [isLocked, gameState.question, gameState.isSpectator, sendMessage]
  );

  const handleBackToMenu = useCallback(() => {
    intentionalCloseRef.current = true;
    if (wsRef.current) {
      wsRef.current.close(1000);
    }
    router.push("/play");
  }, [router]);

  // --- Render ---

  const renderPhase = () => {
    switch (gameState.phase) {
      case "connecting":
        return <LoadingScreen message="Connecting..." />;

      case "lobby":
        return (
          <LobbyScreen
            players={gameState.players}
            spectatorCount={gameState.spectators}
            isHost={gameState.isHost}
            onStart={handleStart}
            sessionId={sessionId}
            username={username || ""}
            roomCode={sessionId}
          />
        );

      case "playing":
        if (!gameState.question) return <LoadingScreen message="Loading question..." />;
        const myDisplayName =
          gameState.players.find((p) => p.user_id === gameState.myUserId)
            ?.display_name || "";
        return (
          <div className="h-full w-full flex relative">
            <GameUI
              question={gameState.question}
              timeRemaining={gameState.timeRemaining}
              onAnswer={handleAnswer}
              isLocked={isLocked || gameState.isSpectator}
              answeredPlayers={gameState.answeredPlayers.length}
              totalPlayers={gameState.players.filter((p) => p.connected).length}
              optionStatuses={optionStatuses}
              isSpectator={gameState.isSpectator}
              correctAnswers={gameState.correctAnswers}
              playerAnswers={gameState.playerAnswers}
              myDisplayName={myDisplayName}
              scryingAnswers={gameState.scryingActive ? gameState.scryingAnswers : undefined}
              disabledOptionIndices={gameState.disabledOptions}
              hints={gameState.hints}
              onUseHint={(hintName) => sendMessage("use_hint", { hint: hintName })}
            />
          </div>
        );

      case "loading":
        return <LoadingScreen message="Loading next question..." />;

      case "ranking":
        return (
          <RankingScreen
            scores={gameState.scores}
            questionNumber={gameState.question?.number || 0}
            totalQuestions={gameState.question?.total || 15}
          />
        );

      case "results":
        return (
          <ResultsScreen
            rankings={gameState.scores}
            onBackToMenu={handleBackToMenu}
          />
        );

      case "disconnected":
        return <LoadingScreen message="Reconnecting..." />;

      case "error":
        return (
          <div className="min-h-screen w-screen flex items-center justify-center bg-[#0F172A] text-[#F8FAFC] px-4">
            <div className="max-w-xl w-full rounded-2xl border border-red-500/30 bg-[#111827] p-8 shadow-2xl shadow-red-950/30">
              <div className="text-sm uppercase tracking-[0.3em] text-red-400 mb-3">
                {t("error.session_unavailable")}
              </div>
              <h1 className="text-3xl font-black mb-4">{t("error.match_unavailable")}</h1>
              <p className="text-[#94A3B8] mb-8">
                {errorMessage || "The session has ended or does not exist anymore."}
              </p>
              <button
                onClick={() => router.push("/play")}
                className="rounded-xl bg-red-500 px-5 py-3 font-bold text-white hover:bg-red-400 transition"
              >
                {t("error.back_to_play")}
              </button>
            </div>
          </div>
        );

      default:
        return <LoadingScreen message="Loading..." />;
    }
  };

  return (
    <div className="h-screen w-screen bg-[#0F172A] relative">
      {renderPhase()}
      {/* Host pause control (shown during the between-questions ranking view) */}
      {gameState.isHost && gameState.phase === "ranking" && (
        <div className="absolute top-8 right-16 z-40">
          <PauseButton onClick={handlePause} disabled={false} />
        </div>
      )}
      {/* Pause overlay */}
      {gameState.phase === "paused" && (
        <PauseOverlay
          reason={gameState.pauseReason || "disconnect"}
          disconnectedPlayers={gameState.disconnectedPlayerNames}
          isHost={gameState.isHost}
          onResume={handleResume}
        />
      )}
      {/* Notice Toast */}
      {noticeMessage && (
        <div className="absolute top-20 left-1/2 -translate-x-1/2 z-50 bg-emerald-500/90 text-white px-6 py-3 rounded-xl font-bold text-sm shadow-lg animate-in fade-in slide-in-from-top-2 duration-300">
          {noticeMessage}
        </div>
      )}
      {/* Error Toast */}
      {errorMessage && (
        <div className="absolute top-20 left-1/2 -translate-x-1/2 z-50 bg-red-500/90 text-white px-6 py-3 rounded-xl font-bold text-sm shadow-lg animate-in fade-in slide-in-from-top-2 duration-300">
          {errorMessage}
        </div>
      )}
    </div>
  );
}
