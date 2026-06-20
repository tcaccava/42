'use client';

import { useEffect, useRef, useState } from 'react';
import { useTranslation } from '@/hooks/useTranslation';

type Message = {
	message: string;
	author?: string;
	profile_url?: string;
	photo?: string;
	system?: boolean;
	error?: boolean;
	uuid?: string;
};

export default function GlobalChatWidget() {
	const { t } = useTranslation();
	const WS_BASE = (process.env.NEXT_PUBLIC_API_URL!).replace('https', 'wss');

	const [connected, setConnected] = useState(false);
	const [open, setOpen] = useState(false);
	const [message, setMessage] = useState('');
	const [messages, setMessages] = useState<Message[]>([]);
	const [unread, setUnread] = useState(0);
	const socketRef = useRef<WebSocket | null>(null);
	const panelRef = useRef<HTMLDivElement | null>(null);
	const listRef = useRef<HTMLDivElement | null>(null);

	useEffect(() => {
		const socket = new WebSocket(`${WS_BASE}/ws/chat/`);
		socketRef.current = socket;

		socket.addEventListener('open', () => {
			setConnected(true);
		});

		socket.addEventListener('message', (event) => {
			try {
				const data = JSON.parse(event.data as string);
				const entry: Message = {
					message: data.message || event.data,
					author: data.author,
					profile_url: data.profile_url,
					photo: data.photo,
					system: data.system,
					error: data.error,
					uuid: data.uuid,
				};
				setMessages((prev) => {
					const next = [...prev, entry].slice(-200);
					return next;
				});
				if (!open) setUnread((u) => u + 1);
			} catch {
				setMessages((prev) => [...prev, { message: event.data as string }]);
				if (!open) setUnread((u) => u + 1);
			}
		});

		socket.addEventListener('close', () => {
			setConnected(false);
			socketRef.current = null;
		});

		socket.addEventListener('error', () => {
			appendSystem(t('chat.error'));
		});

		return () => {
			try {
				socket.close();
			} catch {}
			socketRef.current = null;
		};
	}, []);

	useEffect(() => {
		if (open) {
			setUnread(0);
			setTimeout(() => {
				if (listRef.current)
					listRef.current.scrollTop = listRef.current.scrollHeight;
			}, 50);
		}
	}, [open, messages]);

	useEffect(() => {
		const handleClick = (e: MouseEvent) => {
			if (!open) return;
			if (panelRef.current && !panelRef.current.contains(e.target as Node)) {
				setOpen(false);
			}
		};
		if (open) document.addEventListener('click', handleClick);
		return () => document.removeEventListener('click', handleClick);
	}, [open]);

	const appendSystem = (text: string) => {
		setMessages((prev) => [...prev, { message: text, system: true }]);
	};

	const sendMessage = () => {
		if (!socketRef.current || socketRef.current.readyState !== WebSocket.OPEN)
			return;
		const value = message.trim();
		if (!value) return;
		socketRef.current.send(JSON.stringify({ message: value }));
		setMessage('');
	};

	return (
		<div>
			<div
				style={{
					position: 'fixed',
					bottom: 20,
					right: 20,
					zIndex: 120,
					display: 'flex',
					flexDirection: 'column',
					alignItems: 'flex-end',
				}}>
				{open && (
					<div
						ref={panelRef}
						style={{
							width: 340,
							height: 420,
							marginBottom: 10,
							background: '#0F172A',
							color: '#F8FAFC',
							border: '1px solid #334155',
							borderRadius: 12,
							boxShadow: '0 10px 30px rgba(2,6,23,0.6)',
							overflow: 'hidden',
							display: 'flex',
							flexDirection: 'column',
						}}
						onClick={(e) => e.stopPropagation()}>
						<div
							style={{
								padding: 10,
								borderBottom: '1px solid #1E293B',
								display: 'flex',
								justifyContent: 'space-between',
								alignItems: 'center',
							}}>
							<div style={{ fontWeight: 700 }}>{t('chat.title')}</div>
							<div style={{ display: 'flex', gap: 8, alignItems: 'center' }}>
								<div
									style={{
										fontSize: 12,
										color: connected ? '#86efac' : '#fca5a5',
									}}>
									{connected ? t('chat.connected') : t('chat.disconnected')}
								</div>
								<button
									aria-label={t('chat.close')}
									onClick={() => setOpen(false)}
									style={{
										background: 'transparent',
										border: 'none',
										color: '#93c5fd',
										cursor: 'pointer',
									}}>
									✕
								</button>
							</div>
						</div>
						<div
							ref={listRef}
							style={{
								padding: 10,
								flex: 1,
								overflowY: 'auto',
								gap: 8,
								display: 'flex',
								flexDirection: 'column',
							}}>
							{messages.length === 0 && (
								<div style={{ color: '#94A3B8' }}>{t('chat.no_messages')}</div>
							)}
							{messages.map((m, i) => {
								if (m.system) {
									return (
										<div
											key={i}
											style={{
												padding: '2px 8px',
												borderRadius: 999,
												background: m.error
													? 'rgba(239, 68, 68, 0.12)'
													: 'rgba(148, 163, 184, 0.08)',
												border: m.error
													? '1px solid rgba(239, 68, 68, 0.3)'
													: '1px solid rgba(148, 163, 184, 0.15)',
												color: m.error ? '#FCA5A5' : '#94A3B8',
												fontStyle: 'italic',
												fontSize: 12,
												lineHeight: 1.4,
												textOverflow: 'ellipsis',
												textAlign: 'center',
											}}
											title={m.message}>
											{m.message}
										</div>
									);
								}

								return (
									<div
										key={i}
										style={{
											padding: 8,
											borderRadius: 8,
											background: '#071032',
											border: '1px solid transparent',
										}}>
										<div
											style={{ display: 'flex', gap: 8, alignItems: 'center' }}>
											{m.photo ? <span>{m.photo}</span> : null}
											{m.author ? (
												<span style={{ fontWeight: 700 }}>{m.author}</span>
											) : null}
										</div>
										<div style={{ marginTop: 6, whiteSpace: 'pre-wrap' }}>
											{m.message}
										</div>
									</div>
								);
							})}
						</div>
						<div
							style={{
								padding: 10,
								borderTop: '1px solid #1E293B',
								display: 'flex',
								gap: 8,
							}}>
							<input
								aria-label={t('chat.placeholder')}
								value={message}
								onChange={(e) => setMessage(e.target.value)}
								onKeyDown={(e) => {
									if (e.key === 'Enter') sendMessage();
								}}
								placeholder={t('chat.placeholder')}
								style={{
									flex: 1,
									padding: 8,
									borderRadius: 8,
									border: '1px solid #334155',
									background: '#061126',
									color: '#F8FAFC',
								}}
							/>
							<button
								onClick={sendMessage}
								disabled={!connected}
								style={{
									padding: '8px 12px',
									background: '#2563eb',
									color: '#fff',
									borderRadius: 8,
									border: 'none',
									cursor: connected ? 'pointer' : 'not-allowed',
								}}>
								{t('chat.send')}
							</button>
						</div>
					</div>
				)}

				<button
					aria-label={t('chat.open')}
					onClick={() => setOpen((o) => !o)}
					title={t('chat.open')}
					style={{
						width: 52,
						height: 52,
						borderRadius: 999,
						background: '#2563eb',
						color: '#fff',
						border: 'none',
						boxShadow: '0 6px 20px rgba(37,99,235,0.3)',
						fontSize: 22,
						cursor: 'pointer',
					}}>
					💬
					{unread > 0 && !open && (
						<span
							style={{
								position: 'absolute',
								right: 14,
								top: 14,
								width: 10,
								height: 10,
								borderRadius: 10,
								background: '#ef4444',
								border: '2px solid #0F172A',
							}}
						/>
					)}
				</button>
			</div>
		</div>
	);
}
