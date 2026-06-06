'use client';

import { Search } from '@/components/Search';
import type { ProjectCard } from '@/types/search';

type SearchClientProps = {
	initialCards: ProjectCard[];
	initialPageCount: number;
	initialTotalCount: number;
	initialProjectError?: string;
};

export function SearchClient({
	initialCards,
	initialPageCount,
	initialTotalCount,
	initialProjectError,
}: SearchClientProps) {
	return (
		<Search
			initialCards={initialCards}
			initialPageCount={initialPageCount}
			initialTotalCount={initialTotalCount}
			initialProjectError={initialProjectError}
		/>
	);
}
