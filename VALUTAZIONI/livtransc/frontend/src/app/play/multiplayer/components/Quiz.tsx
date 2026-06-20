import { FC, useEffect, useRef, useState } from "react";

type QuizProps = {
	sessionId: string;
}

const Quiz: FC<QuizProps> = ({sessionId}) => {

	const wsRef = useRef<WebSocket | null>(null)
	const [question, setQuestion] = useState<string>("")
	const [answers, setAnswers] = useState<string[]>([])

	console.log("Quiz sessionId:", sessionId);

	const connectWs = () => {
		if (wsRef.current) {
			console.warn("WebSocket already exists");
			return;
		}

  	const WS_BASE = (process.env.NEXT_PUBLIC_API_URL!).replace('https', 'wss');
		const ws = new WebSocket(`${WS_BASE}/ws/quiz/${sessionId}/`);
		wsRef.current = ws;
	}

	useEffect(() => {
		connectWs();
		return () => {
			wsRef.current?.close()  // cleanup on unmount
		}
	}, [])

	const handleWsMessages = () => {
		if (!wsRef.current) {
			alert("Connection to Server Lost, Reconnecting...")
			return;
		}

		wsRef.current.onmessage = (event) => {
			const data = JSON.parse(event.data);
			console.log("Received message:", data);

			if (data.type === "start") {
				// Start Logic
			}
			if (data.type === "question") {
				setQuestion(data.question);
			}
			if (data.type === "answers") {
				setAnswers(data.answers);
			}
		}
	}

	return(
		<div>
			<div>{question}</div>
			<button onClick={() => {
				wsRef.current?.send(JSON.stringify({ type: "phase", answer: "start" }));
			}}>
				Start game
			</button>
		</div>
	)
}

export default Quiz;
