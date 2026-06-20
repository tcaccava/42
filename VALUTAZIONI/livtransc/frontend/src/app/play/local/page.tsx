"use client";

import { useState, useEffect, useCallback } from "react";
import IntroScreen from "./components/IntroScreen";
import ResultsScreen from "./components/ResultsScreen";
import RankingScreen from "./components/RankingScreen";
import LoadingScreen from "./components/LoadingScreen";
import GameUI from "./components/GameUI";

interface Question {
	id: number;
	text: string;
	options: string[];
	number: number;
	answer: string;
}

type Phase = "intro" | "loading" | "playing" | "results" | "ranking";

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://127.0.0.1:8000';

export default function LocalHostPage() {
	const [roomCode, setRoomCode] = useState("");
	const [players, setPlayers] = useState<string[]>([]);
	const [phase, setPhase] = useState<Phase>("loading");
	const [question, setQuestion] = useState<Question | null>(null);
	const [matchId, setMatchId] = useState<number | null>(null);
	const [time, setTime] = useState(20);
	const [message, setMessage] = useState("");
	const [isWrong, setIsWrong] = useState(false);
	const [isLocked, setIsLocked] = useState(false);
	const [optionStatuses, setOptionStatuses] = useState<Record<string, "green" | "red" | "yellow" | "none">>({});
	const [currentAnswers, setCurrentAnswers] = useState<Record<string, string>>({});
	const [showNicknames, setShowNicknames] = useState(false);
	const [scores, setScores] = useState<Record<string, number>>({});
	const [windowWidth, setWindowWidth] = useState(1200);

	const colorAnswer = (ans: string, color: "green" | "red" | "yellow" | "none") => {
		setOptionStatuses(prev => ({ ...prev, [ans]: color }));
	};



	// --- API CALLS ---

	const checkSession = async () => {
		try {
			const res = await fetch(`${API_BASE}/api/check_session_local`, {
				method: 'GET',
				headers: { 'Content-Type': 'application/json' },
				credentials: 'include',
			});
			const data = await res.json();
			if (data.success) {
				setMatchId(data.match_id);
				if (data.next_question) {
					setQuestion(data.next_question);
					setPhase("playing");
				}
				return data.next_question;
			}
		} catch (error) {
			console.error('Check session error:', error);
		}
		return null;
	};

	const createGame = async () => {
		try {
			const res = await fetch(`${API_BASE}/api/create_LMP_game`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				credentials: 'include',
			});
			const data = await res.json();
			if (data.success) {
				setRoomCode(data.code);
				setMatchId(data.match_id);
				return data.code;
			}
		} catch (error) {
			console.error('Create game error:', error);
		}
		return null;
	};

	// --- GAME LOGIC ---

	useEffect(() => {
		const init = async () => {
			const activeSession = await checkSession();
			if (activeSession) {
				setQuestion(activeSession);
				setPhase("playing");
			} else {
				const code = await createGame();
				if (code) {
					setPhase("intro");
				}
			}
		};
		init();
	}, []);

	useEffect(() => {
		if (phase !== "intro") return;

		const fetchPlayers = async () => {
			try {
				const response = await fetch(`${API_BASE}/api/get_MP_players?code=${roomCode}`);
				if (response.ok) {
					const data = await response.json();
					if (data.players) {
						setPlayers(data.players);
					}
				}
			} catch (err) {
				console.error("Error polling players:", err);
			}
		};

		fetchPlayers();
		const interval = setInterval(fetchPlayers, 1000);
		return () => clearInterval(interval);
	}, [phase, roomCode]);

	useEffect(() => {
		if (typeof window === 'undefined') return;
		// Set initial width on client to avoid SSR/client mismatch
		setWindowWidth(window.innerWidth);
		const handleResize = () => setWindowWidth(window.innerWidth);
		window.addEventListener("resize", handleResize);
		return () => window.removeEventListener("resize", handleResize);
	}, []);

	useEffect(() => {
		let interval: NodeJS.Timeout;
		if (phase === "playing" && !isLocked) {
			interval = setInterval(() => {
				setTime((prev) => Math.max(0, prev - 1));
			}, 1000);
		}
		return () => clearInterval(interval);
	}, [phase, isLocked]);

	const handleResetGame = async () => {
		try {
			const res = await fetch(`${API_BASE}/api/reset_LMP_game`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({ roomcode: roomCode }),
				credentials: 'include',
			});
			const data = await res.json();
			if (data.success) {
				setPhase("intro");
				setScores({});
				setTime(20);
				setIsWrong(false);
				setIsLocked(false);
				setOptionStatuses({});
				setCurrentAnswers({});
				setShowNicknames(false);
			}
		} catch (error) {
			console.error("Failed to reset game:", error);
		}
	};

	const handleNextQuestion = async () => {
		setPhase("loading");
		try {
			const res = await fetch(`${API_BASE}/api/get_next_question`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({ roomcode: roomCode }),
				credentials: 'include',
			});
			const data = await res.json();
			if (data.success) {
				if (data.finished) {
					setPhase("results");
					setMessage("Trivia Challenge Complete!");
					return;
				}
				setQuestion(data.next_question);
				setPhase("playing");
				setTime(20);
				setIsWrong(false);
				setIsLocked(false);
				setOptionStatuses({});
				setCurrentAnswers({});
				setShowNicknames(false);
			}
		} catch (error) {
			console.error("Failed to get next question:", error);
			setPhase("intro"); // Fallback
		}
	};

	const pollAnswers = useCallback(async () => {
		if (phase !== "playing" || !roomCode || showNicknames) return;

		try {
			const res = await fetch(`${API_BASE}/api/get_answers?roomcode=${roomCode}`);
			const data = await res.json();
			if (data.success) {
				setCurrentAnswers(data.answers);
				const answerCount = Object.keys(data.answers).length;
				if ((players.length > 0 && answerCount >= players.length) || time === 0) {
					setShowNicknames(true);
					setIsLocked(true);

					if (!question) return;
					// Mostra chi ha risposto cosa
					await new Promise(r => setTimeout(r, 2000));

					// Rivela la risposta corretta
					colorAnswer(question.answer, "green");

					const correctText = question.answer.trim().toLowerCase();
					const correctIdx = question.options.findIndex(opt => opt.trim().toLowerCase() === correctText);
					const correctLetter = correctIdx !== -1 ? String.fromCharCode(65 + correctIdx) : null;

					console.log("Correct text:", correctText, "-> Letter:", correctLetter);

					setScores(prev => {
						const newScores = { ...prev };
						if (!correctLetter) return newScores;

						Object.entries(data.answers).forEach(([name, ans]) => {
							const playerAns = (ans as string).trim().toUpperCase();
							console.log(`Checking player ${name}: chose ${playerAns}, expected ${correctLetter}`);

							if (playerAns === correctLetter) {
								newScores[name] = (newScores[name] || 0) + 1;
								console.log(`Point added to ${name}! New score: ${newScores[name]}`);
							} else {
								newScores[name] = (newScores[name] || 0);
							}
						});
						// Inizializza anche chi non ha risposto
						players.forEach(p => {
							if (!(p in newScores)) newScores[p] = 0;
						});
						return newScores;
					});

					await new Promise(r => setTimeout(r, 2500));
					setPhase("ranking");
					await new Promise(r => setTimeout(r, 2500));
					handleNextQuestion();
				}
			}
		} catch (error) {
			console.error("Error polling answers:", error);
		}
	}, [phase, roomCode, players.length, time, showNicknames]);

	useEffect(() => {
		let interval: NodeJS.Timeout;
		if (phase === "playing" && !showNicknames) {
			interval = setInterval(pollAnswers, 1000);
		}
		return () => clearInterval(interval);
	}, [phase, showNicknames, pollAnswers]);


	// --- UI COMPONENTS ---

	if (windowWidth < 766) {
		return (
			<div className="h-full flex flex-col items-center justify-center p-8 bg-[#0F172A] text-center gap-6">
				<div className="text-6xl animate-bounce">📱❌</div>
				<div className="flex flex-col gap-2">
					<h1 className="text-2xl font-black text-[#FACC15] uppercase tracking-tighter">SCREEN TOO SMALL</h1>
					<p className="text-[#94A3B8] font-medium text-sm max-w-[250px]">
						The Host screen requires a larger display area.<br />
						<span className="text-white font-bold">Please enlarge the window</span> or use a tablet/PC.
					</p>
				</div>
				<div className="w-12 h-1 bg-[#FACC15] rounded-full opacity-20"></div>
			</div>
		);
	}

	if (phase === "intro") {
		return <IntroScreen roomCode={roomCode} players={players} onStart={handleNextQuestion} />;
	}

	if (phase === "results") {
		return <ResultsScreen scores={scores} message={message} onPlayAgain={handleResetGame} />;
	}

	if (phase === "ranking") {
		return <RankingScreen scores={scores} questionNumber={question?.number || 0} />;
	}

	if (phase === "loading" || !question) {
		return <LoadingScreen />;
	}

	return (
		<GameUI
			question={question}
			time={time}
			optionStatuses={optionStatuses}
			showNicknames={showNicknames}
			currentAnswers={currentAnswers}
		/>
	);
}

