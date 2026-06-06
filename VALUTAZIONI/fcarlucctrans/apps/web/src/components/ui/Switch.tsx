'use client';

type SwitchProps = {
	id: string;
	checked: boolean;
	onCheckedChange: (checked: boolean) => void;
	label: string;
	description?: string;
	disabled?: boolean;
};

export function Switch({
	id,
	checked,
	onCheckedChange,
	label,
	description,
	disabled = false,
}: SwitchProps) {
	return (
		<div className="flex items-start justify-between gap-4 rounded-xl border border-border bg-darker-background/50 p-4">
			<div className="min-w-0 flex-1">
				<label
					htmlFor={id}
					className="block text-sm font-semibold text-foreground"
				>
					{label}
				</label>
				{description && (
					<p className="mt-1 text-sm text-foreground/70">{description}</p>
				)}
			</div>
			<button
				id={id}
				type="button"
				role="switch"
				aria-checked={checked}
				disabled={disabled}
				onClick={() => onCheckedChange(!checked)}
				className={`relative inline-flex h-7 w-12 shrink-0 cursor-pointer items-center rounded-full border-2 border-transparent transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[var(--color-hvr-background)] focus-visible:ring-offset-2 focus-visible:ring-offset-background disabled:cursor-not-allowed disabled:opacity-50 ${
					checked ? 'bg-[var(--btn-background)]' : 'bg-border'
				}`}
			>
				<span className="sr-only">{label}</span>
				<span
					aria-hidden
					className={`pointer-events-none inline-block h-5 w-5 transform rounded-full bg-white shadow-sm ring-0 transition-transform ${
						checked ? 'translate-x-5' : 'translate-x-0.5'
					}`}
				/>
			</button>
		</div>
	);
}
