import { JSX } from 'react';

export default function CodeTitle({ title, size, className }: { title: string, size: number, className?: string }) {
	const Tag = `h${size}` as keyof JSX.IntrinsicElements;
	return (
		<Tag className={`text-4xl font-bold mb-6 relative top-0.5 ${className || ''}`}>
			<span className="text-[#05BBBB]">{'< '}</span>
			<span className="text-foreground">{title} </span>
			<span className="text-green-500">{' /'}</span>
			<span className="text-[#05BBBB]">{'> '}</span>
		</Tag>
	)
}
