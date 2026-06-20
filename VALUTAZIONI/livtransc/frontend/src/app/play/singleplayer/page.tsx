"use client";

import { useState, useEffect, useCallback } from "react";
import Link from "next/link";
import { useUsername } from "@/hooks/navbarDatas";
import IntroScreen from "./components/IntroScreen";
import ResultsScreen from "./components/ResultsScreen";
import LoadingScreen from "./components/LoadingScreen";
import GameUI from "./components/GameUI";

interface Question {
	id: number;
	text: string;
	options: string[];
	number: number;
}

type Phase = "intro" | "loading" | "playing" | "results";

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';

export default function SingleplayerPage() {
	const username = useUsername();
	const [phase, setPhase] = useState<Phase>("loading");
	const [question, setQuestion] = useState<Question | null>(null);
	const [matchId, setMatchId] = useState<number | null>(null);
	const [time, setTime] = useState(60);
	const [message, setMessage] = useState("");
	const [isWrong, setIsWrong] = useState(false);
	const [isLocked, setIsLocked] = useState(false);
	const [doubleChanceActive, setDoubleChanceActive] = useState(false);
	const [optionStatuses, setOptionStatuses] = useState<Record<string, "green" | "red" | "yellow" | "none">>({});
	const [lifelines, setLifelines] = useState<Record<string, boolean>>({
		fiftyFifty: false,
		change: false,
		doubleChance: false,
		refillTime: false,
	});

	const colorAnswer = (ans: string, color: "green" | "red" | "yellow" | "none") => {
		setOptionStatuses(prev => ({ ...prev, [ans]: color }));
	};

	const useFiftyFifty = async () => {
		if (lifelines.fiftyFifty || isLocked || phase !== "playing") return;
		setLifelines(prev => ({ ...prev, fiftyFifty: true }));
		console.log("50:50 Lifeline used");
		const res = await fetch(`${API_BASE}/api/use_help`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				credentials: 'include',
				body: JSON.stringify({
					match_id: matchId,
					fifty: true,
				}),
			});
			const data = await res.json();
			if (data.success) {
				colorAnswer(data.wrong_answers[0], "red");
				colorAnswer(data.wrong_answers[1], "red");
			}
	};

	const useChange = async () => {
		if (lifelines.change || isLocked || phase !== "playing") return;
		setLifelines(prev => ({ ...prev, change: true }));
		console.log("Change Question Lifeline used");
		const res = await fetch(`${API_BASE}/api/use_help`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				credentials: 'include',
				body: JSON.stringify({
					match_id: matchId,
					change_question: true,
				}),
			});
			const data = await res.json();
			if (data.success) {
				setIsLocked(true);
				colorAnswer(data.correct_answer, "green");
				await new Promise(r => setTimeout(r, 1500));
				setIsLocked(false);
				setQuestion(data.next_question);
				setTime(60);
				setIsWrong(false);
			}
	};

	const useDoubleChance = async () => {
		if (lifelines.doubleChance || isLocked || phase !== "playing") return;
		setLifelines(prev => ({ ...prev, doubleChance: true }));
		console.log("Double Chance Lifeline used");
		setDoubleChanceActive(true);
	};

	const useRefillTime = async () => {
		if (lifelines.refillTime || isLocked || phase !== "playing") return;
		setLifelines(prev => ({ ...prev, refillTime: true }));
		console.log("Time Refill used");
		const res = await fetch(`${API_BASE}/api/use_help`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				credentials: 'include',
				body: JSON.stringify({
					match_id: matchId,
					refill_time: true,
				}),
			});
			const data = await res.json();
			if (data.success) {
				setTime(60);
				setIsWrong(false);
			}
	};

	// --- API CALLS ---

	const checkSession = async () => {
		try {
			const res = await fetch(`${API_BASE}/api/check_session`, {
				method: 'GET',
				headers: { 'Content-Type': 'application/json' },
				credentials: 'include',
			});
			const data = await res.json();
			if (data.success) {
				setMatchId(data.match_id);
				if (data.existing) {
					setQuestion(data.next_question);
					setPhase("playing");
					setTime(Math.round(data.remaining_time));
					setIsWrong(false);
					setLifelines({
						fiftyFifty: !data.help.fifty_fifty,
						change: !data.help.change_question,
						doubleChance: !data.help.double_chance,
						refillTime: !data.help.refill_time,
					});
				}
				return data.next_question;
			}
		} catch (error) {
			console.error('Check session error:', error);
		}
		return null;
	};

	const startGame = async () => {
		try {
			const res = await fetch(`${API_BASE}/api/start_match`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({}),
				credentials: 'include',
			});
			const data = await res.json();
			if (data.success) {
				setMatchId(data.match_id);
				return data.question;
			}
		} catch (error) {
			console.error('Start match error:', error);
		}
		return null;
	};

	const submitAnswer = async (answer: string) => {
		if (!matchId || question === null) return null;

		try {
			const res = await fetch(`${API_BASE}/api/update_match`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({
					match_id: matchId,
					user_answer: answer,
					question_index: question.number - 1,
					double_chance: doubleChanceActive,
				}),
				credentials: 'include',
			});
			const data = await res.json();

			if (res.status === 408) {
				setPhase("results");
				setMessage("Time's up! The server closed the match.");
				setIsWrong(true);
				return null;
			}

			if (data.success) {
				return {
					isCorrect: data.is_correct,
					correctAnswer: data.correct_answer,
					isCompleted: data.isCompleted,
					nextQuestion: data.next_question
				};
			}
		} catch (error) {
			console.error('Update match error:', error);
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
				setPhase("intro");
			}
		};
		init();
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

	useEffect(() => {
		if (time === 0 && phase === "playing" && !isLocked) {
			const handleTimeUp = async () => {
				setIsLocked(true);
				try {
					await fetch(`${API_BASE}/api/update_match`, {
						method: 'POST',
						headers: { 'Content-Type': 'application/json' },
						body: JSON.stringify({
							match_id: matchId,
						}),
						credentials: 'include',
					});
				} catch (error) {
					console.error("Failed to notify server about timeout:", error);
				}
				setPhase("results");
				setMessage("Time's up! You ran out of time.");
				setIsWrong(true);
				setOptionStatuses({});
				setIsLocked(false);
			};
			handleTimeUp();
		}
	}, [time, phase, isLocked, matchId]);

	const handleStart = async () => {
		setPhase("loading");
		const firstQ = await startGame();
		setQuestion(firstQ);
		setPhase("playing");
		setTime(60);
		setIsWrong(false);
		setLifelines({ fiftyFifty: false, change: false, doubleChance: false, refillTime: false });
	};

	const handleAnswer = async (ans: string) => {
		if (isLocked || (optionStatuses[ans] && optionStatuses[ans] !== "none")) return;
		setIsLocked(true);

		colorAnswer(ans, "yellow");
		const result = await submitAnswer(ans);
		if (!result) {
			setIsLocked(false);
			return;
		}

		await new Promise(r => setTimeout(r, 1500));

		if (result.isCorrect) {
			setDoubleChanceActive(false);
			colorAnswer(result.correctAnswer, "green");
			await new Promise(r => setTimeout(r, 1500));
			if (result.isCompleted) {
				setPhase("results");
				setMessage("CONGRATULATIONS! You cleared the Trivia Master challenge!");
				setOptionStatuses({});
				setIsLocked(false);
			} else {
				setQuestion(result.nextQuestion || null);
				setTime(60);
				setOptionStatuses({});
				setIsLocked(false);
			}
		} else if (doubleChanceActive) {
			setDoubleChanceActive(false);
			colorAnswer(ans, "red");
			setIsLocked(false);
			return;
		} else {
			colorAnswer(result.correctAnswer, "green");
			colorAnswer(ans, "red");
			await new Promise(r => setTimeout(r, 1500));
			setIsWrong(true);
			setPhase("results");
			setMessage("Nice try.");
			setOptionStatuses({});
			setIsLocked(false);
		}
	};

	// --- UI COMPONENTS ---

	if (phase === "intro") {
		return <IntroScreen onStart={handleStart} />;
	}

	if (phase === "results") {
		return <ResultsScreen isWrong={isWrong} message={message} questionNumber={question ? question.number - 1 : 0} onRetry={handleStart} />;
	}

	if (phase === "loading" || !question) {
		return <LoadingScreen />;
	}

	return (
		<GameUI
			question={question}
			time={time}
			optionStatuses={optionStatuses}
			isLocked={isLocked}
			onAnswer={handleAnswer}
			lifelines={lifelines}
			onUseFiftyFifty={useFiftyFifty}
			onUseChange={useChange}
			onUseDoubleChance={useDoubleChance}
			onUseRefillTime={useRefillTime}
		/>
	);
}
