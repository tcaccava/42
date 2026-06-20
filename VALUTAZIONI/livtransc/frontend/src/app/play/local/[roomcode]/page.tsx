"use client";

import { useState, useEffect, useCallback, use } from "react";
import SetUsernameScreen from "./components/SetUsernameScreen";
import IntroScreen from "./components/IntroScreen";
import ResultsScreen from "./components/ResultsScreen";
import GameUI from "./components/GameUI";

type Phase = "loading" | "setUsername" | "intro" | "playing" | "results";

interface Question {
    id: number;
    text: string;
    options: string[];
    number: number;
}

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://127.0.0.1:8000';

export default function LocalPlayerPage({ params }: { params: Promise<{ roomcode: string }> }) {
    const { roomcode } = use(params);
    const [myUsername, setMyUsername] = useState("");
    const [inputUsername, setInputUsername] = useState("");

    const [players, setPlayers] = useState<string[]>([]);
    const [phase, setPhase] = useState<Phase>("loading");
    const [matchId, setMatchId] = useState<number | null>(null);
    const [questionNum, setQuestionNum] = useState<number>(-1);
    const [isWrong, setIsWrong] = useState(false);
    const [message, setMessage] = useState("");
    const [isLocked, setIsLocked] = useState(false);
    const [error, setError] = useState("");
    const [isJoining, setIsJoining] = useState(false);
    const [question, setQuestion] = useState<Question | null>(null);
    const [optionStatuses, setOptionStatuses] = useState<Record<string, "green" | "red" | "yellow" | "none">>({});

    // --- GAME LOGIC ---

    // --- API CALLS ---

    useEffect(() => {
        const stored = sessionStorage.getItem("local_player_username");
        const storedRoomcode = sessionStorage.getItem("local_roomcode");
        if (stored && storedRoomcode === roomcode) {
            setMyUsername(stored);
            setPlayers([stored]);
            setPhase("intro");
        } else {
            setPhase("setUsername");
        }
    }, [roomcode]);

    useEffect(() => {
        if (phase === "playing") {
            document.body.style.overflow = "hidden";
            return () => {
                document.body.style.overflow = "";
            };
        }
    }, [phase]);


    const handleSaveUsername = async () => {
        if (!inputUsername.trim() || isJoining) return;
        const name = inputUsername.trim();
        setError("");
        setIsJoining(true);

        try {
            console.log("Attempting to join room:", roomcode, "as", name);
            const response = await fetch(`${API_BASE}/api/add_MP_player`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    username: name,
                    roomcode: roomcode
                }),
            });

            if (!response.ok) {
                const data = await response.json().catch(() => ({}));
                throw new Error(data.error || 'Error joining room');
            }

            sessionStorage.setItem("local_player_username", name);
            if (sessionStorage.getItem("local_roomcode") !== roomcode) {
                sessionStorage.setItem("local_roomcode", roomcode);
            }
            setMyUsername(name);
            setPlayers([name]);
            setPhase("intro");
        } catch (err) {
            console.error('Join error:', err);
            setError(err instanceof Error ? err.message : "Connection failed");
        } finally {
            setIsJoining(false);
        }
    };

    const handleAnswer = async (ans: string) => {
        if (isLocked || (optionStatuses[ans] && optionStatuses[ans] !== "none")) return;
        setIsLocked(true);
        setOptionStatuses({ [ans]: "yellow" });
        try {
            const response = await fetch(`${API_BASE}/api/submit_answer`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    roomcode: roomcode,
                    username: myUsername,
                    answer: ans
                }),
            });

            if (response.ok) {
                console.log("Answer submitted successfully:", ans);
                sessionStorage.setItem("last_answer", ans);
                sessionStorage.setItem("last_question_num", questionNum.toString());
            } else {
                console.error("Failed to submit answer");
                // If it failed, let's unlock so the player might try again?
                // Actually usually you shouldn't, but for debugging:
                // setIsLocked(false);
            }
        } catch (err) {
            console.error("Error submitting answer:", err);
            // setIsLocked(false);
        }
    };


    useEffect(() => {
        if (phase !== "intro" && phase !== "playing") return;

        const fetchGameStatus = async () => {
            try {
                const response = await fetch(`${API_BASE}/api/get_MP_players?code=${roomcode}`);
                if (!response.ok) throw new Error('Failed to fetch status');
                const data = await response.json();

                if (data.status === "playing") {
                    if (phase !== "playing") {
                        setPhase("playing");
                    }

                    // Reset pulsantiera se la domanda è cambiata sul server
                    if (data.question_number !== undefined && data.question_number !== questionNum) {
                        console.log("New question detected. Resetting keypad.");
                        const newQNum = data.question_number;
                        setQuestionNum(newQNum);

                        // Verifichiamo se abbiamo una risposta salvata localmente per QUESTA domanda
                        const savedAns = sessionStorage.getItem("last_answer");
                        const savedQNum = sessionStorage.getItem("last_question_num");

                        if (savedAns && savedQNum === newQNum.toString()) {
                            setIsLocked(true);
                            setOptionStatuses({ [savedAns]: "yellow" });
                        } else {
                            setIsLocked(false);
                            setOptionStatuses({});
                        }
                    } else if (!isLocked) {
                        // Se la domanda è la stessa ma non siamo bloccati (es. post-refresh), verifichiamo lo storage
                        const savedAns = sessionStorage.getItem("last_answer");
                        const savedQNum = sessionStorage.getItem("last_question_num");
                        if (savedAns && savedQNum === questionNum.toString()) {
                            setIsLocked(true);
                            setOptionStatuses({ [savedAns]: "yellow" });
                        }
                    }
                } else if (data.status === "finished") {
                    setPhase("results");
                    setMessage("The game has ended!");
                }

                if (data.players) {
                    setPlayers(data.players);
                }
            } catch (err) {
                console.error("Error polling game status:", err);
            }
        };

        fetchGameStatus();
        const interval = setInterval(fetchGameStatus, 1000);
        return () => clearInterval(interval);
    }, [phase, roomcode, questionNum]);


    // --- UI COMPONENTS ---

    if (phase === "setUsername") {
        return (
            <SetUsernameScreen
                roomcode={roomcode}
                inputUsername={inputUsername}
                onUsernameChange={setInputUsername}
                onSubmit={handleSaveUsername}
                isJoining={isJoining}
                error={error}
            />
        );
    }

    if (phase === "results") {
        return <ResultsScreen message={message} onRestart={() => window.location.reload()} />;
    }

    if (phase === "intro") {
        return (
            <IntroScreen
                roomcode={roomcode}
                players={players}
                myUsername={myUsername}
                onChangeName={() => {
                    sessionStorage.removeItem("local_player_username");
                    sessionStorage.removeItem("local_roomcode");
                    setPhase("setUsername");
                }}
            />
        );
    }

    if (phase === "playing") {
        return <GameUI optionStatuses={optionStatuses} isLocked={isLocked} onAnswer={handleAnswer} />;
    }

}
