'use client';

import { useCallback, useEffect, useState } from 'react';
import {
	Loader2,
	Search as SearchIcon,
	User as UserIcon,
	Hash,
	Calendar,
	X,
} from 'lucide-react';
import Image from 'next/image';
import { useRouter } from 'next/navigation';
import { Input } from '@/components/Input';
import {
	ProjectCard,
	SEARCH_PAGE_SIZE,
	SearchQuery,
	SortMethod,
	SortMethodValues,
} from '@/types/search';
import { searchProjects } from '@/server/project';
import { searchEvents } from '@/server/events';
import {
	fetchUserDirectory,
	searchUsers,
	type UserSearchResult,
} from '@/server/users';
import { ProjectKind, ProjectKindValues } from '@/types/project';
import type { Event } from '@/types';
import { Button } from './Button';
import { Badge } from '@/components/Badge';
import { ProjectSearchCard } from '@/components/ProjectSearchCard';
import { local } from '@/utilities';
import {
	AlertBanner,
	EmptyState,
	PageHeader,
	PageShell,
	PaginationBar,
	SectionCard,
	TabButton,
} from '@/components/ui/layout';
import { FormLabel, SelectField } from '@/components/ui/form';

type SearchTab = 'all' | 'users' | 'projects' | 'events';

type SearchProps = {
	initialCards: ProjectCard[];
	initialPageCount: number;
	initialTotalCount: number;
	initialProjectError?: string;
};

function displayName(user: UserSearchResult) {
	const full = `${user.first_name ?? ''} ${user.last_name ?? ''}`.trim();
	return full || user.displayname || user.login;
}

export function Search({
	initialCards,
	initialPageCount,
	initialTotalCount,
	initialProjectError,
}: SearchProps) {
	const router = useRouter();
	const [tab, setTab] = useState<SearchTab>('all');
	const [userQuery, setUserQuery] = useState('');
	const [subject, setSubject] = useState<ProjectKind | null>(null);
	const [tags, setTags] = useState<string[]>([]);
	const [projectTitle, setProjectTitle] = useState('');
	const [tagInput, setTagInput] = useState('');
	const [sortMethod, setSortMethod] = useState(SortMethod.Date);
	const [descendingMode, setDescendingMode] = useState(false);

	const [users, setUsers] = useState<UserSearchResult[]>([]);
	const [userPageNumber, setUserPageNumber] = useState(0);
	const [userPageCount, setUserPageCount] = useState(0);
	const [userTotalCount, setUserTotalCount] = useState(0);
	const [userError, setUserError] = useState<string | undefined>();
	const [loadingUsers, setLoadingUsers] = useState(true);

	const [events, setEvents] = useState<Event[]>([]);
	const [eventPageNumber, setEventPageNumber] = useState(0);
	const [eventPageCount, setEventPageCount] = useState(0);
	const [eventTotalCount, setEventTotalCount] = useState(0);
	const [eventError, setEventError] = useState<string | undefined>();
	const [loadingEvents, setLoadingEvents] = useState(true);

	const [projectPageNumber, setProjectPageNumber] = useState(0);
	const [projectPageCount, setProjectPageCount] = useState(initialPageCount);
	const [projectTotalCount, setProjectTotalCount] = useState(initialTotalCount);
	const [projectCards, setProjectCards] = useState(initialCards);
	const [projectError, setProjectError] = useState<string | undefined>(
		initialProjectError,
	);
	const [isSearchingProjects, setIsSearchingProjects] = useState(false);

	const t = local.useLocalization().t;
	const isUserSearch = userQuery.trim().length >= 2;
	const showUsers = tab === 'all' || tab === 'users';
	const showProjects = tab === 'all' || tab === 'projects';
	const showEvents = tab === 'all' || tab === 'events';

	const loadUsers = useCallback(async (query: string, page: number) => {
		setLoadingUsers(true);
		const res =
			query.trim().length >= 2
				? await searchUsers(query, page, SEARCH_PAGE_SIZE)
				: await fetchUserDirectory(page, SEARCH_PAGE_SIZE);

		if (res.status === 'success' && res.users) {
			setUsers(res.users);
			setUserPageCount(res.pageCount ?? 0);
			setUserTotalCount(res.totalCount ?? 0);
			setUserError(undefined);
		} else if (res.status === 'unauthorized') {
			setUserError('Sign in to browse students.');
			setUsers([]);
		} else {
			setUserError(res.message ?? 'Could not load students.');
			setUsers([]);
		}
		setLoadingUsers(false);
	}, []);

	const loadEvents = useCallback(async (query: string, page: number) => {
		setLoadingEvents(true);
		const res = await searchEvents(query, page, SEARCH_PAGE_SIZE);

		if (res.status === 'success') {
			setEvents(res.events);
			setEventPageCount(res.pageCount);
			setEventTotalCount(res.totalCount);
			setEventError(undefined);
		} else if (res.status === 'unauthorized') {
			setEventError('Sign in to browse events.');
			setEvents([]);
		} else {
			setEventError(res.message ?? 'Could not load events.');
			setEvents([]);
		}
		setLoadingEvents(false);
	}, []);

	useEffect(() => {
		const timer = setTimeout(() => {
			void loadUsers(userQuery, userPageNumber);
		}, isUserSearch ? 300 : 0);
		return () => clearTimeout(timer);
	}, [userQuery, userPageNumber, isUserSearch, loadUsers]);

	useEffect(() => {
		const timer = setTimeout(() => {
			void loadEvents(userQuery, eventPageNumber);
		}, isUserSearch ? 300 : 0);
		return () => clearTimeout(timer);
	}, [userQuery, eventPageNumber, isUserSearch, loadEvents]);

	function handleUserQueryChange(value: string) {
		setUserQuery(value);
		setUserPageNumber(0);
		setEventPageNumber(0);
	}

	function buildProjectQuery(overrides: Partial<SearchQuery> = {}): SearchQuery {
		return {
			title: projectTitle,
			subject,
			tags,
			sortBy: sortMethod,
			descending: descendingMode,
			pageNumber: overrides.pageNumber ?? projectPageNumber,
			pageSize: SEARCH_PAGE_SIZE,
			...overrides,
		};
	}

	async function runProjectSearch(overrides: Partial<SearchQuery> = {}) {
		setIsSearchingProjects(true);
		const query = buildProjectQuery(overrides);
		const [foundProjects, nextPageCount, nextTotalCount, error] =
			await searchProjects(query);
		setProjectCards(foundProjects);
		setProjectPageCount(nextPageCount);
		setProjectTotalCount(nextTotalCount);
		setProjectError(error);
		if (overrides.pageNumber !== undefined) {
			setProjectPageNumber(overrides.pageNumber);
		}
		setIsSearchingProjects(false);
	}

	async function addTag(raw: string) {
		const trimmed = raw.trim();
		if (!trimmed || tags.includes(trimmed)) return;
		const newTags = [...tags, trimmed];
		setTags(newTags);
		setTagInput('');
		setProjectPageNumber(0);
		await runProjectSearch({ tags: newTags, pageNumber: 0 });
	}

	return (
		<PageShell className="space-y-5">
			<PageHeader
				icon={<SearchIcon className="h-5 w-5 text-teal-600" />}
				title={t.search}
				description={
					t.searchPlaceholder ??
					'Search students, projects, and campus events.'
				}
				className="mb-2"
			/>

			<SectionCard contentClassName="space-y-3 p-4 sm:p-5">
				<div>
					<FormLabel htmlFor="search-users">{t.students}</FormLabel>
					<div className="relative">
						<SearchIcon
							className="absolute left-3 top-1/2 h-4 w-4 -translate-y-1/2 text-gray-400"
							aria-hidden="true"
						/>
						<Input
							id="search-users"
							type="text"
							placeholder={t.searchPlaceholder ?? 'Search users or events…'}
							value={userQuery}
							onChange={(e) => handleUserQueryChange(e.target.value)}
							className="pl-9"
						/>
					</div>
				</div>

				<div className="flex flex-wrap gap-2">
					<TabButton active={tab === 'all'} onClick={() => setTab('all')}>
						All
					</TabButton>
					<TabButton active={tab === 'users'} onClick={() => setTab('users')}>
						{t.students}
					</TabButton>
					<TabButton active={tab === 'projects'} onClick={() => setTab('projects')}>
						{t.projects}
					</TabButton>
					<TabButton active={tab === 'events'} onClick={() => setTab('events')}>
						{t.events}
					</TabButton>
				</div>

				{showProjects && (
					<div className="space-y-3 border-t border-border/60 pt-3">
						<div className="grid gap-3 md:grid-cols-[minmax(0,1fr)_9rem_9rem] md:items-end">
							<div>
								<FormLabel htmlFor="search-projects">{t.searchProjects}</FormLabel>
								<div className="relative">
									<SearchIcon
										className="absolute left-3 top-1/2 h-4 w-4 -translate-y-1/2 text-gray-400"
										aria-hidden="true"
									/>
									<Input
										id="search-projects"
										type="text"
										placeholder={t.searchProjects}
										value={projectTitle}
										onChange={async (e) => {
											const value = e.target.value;
											setProjectTitle(value);
											setProjectPageNumber(0);
											await runProjectSearch({ title: value, pageNumber: 0 });
										}}
										className="pl-9"
										disabled={isSearchingProjects}
									/>
								</div>
							</div>

							<SelectField
								id="project-subject-filter"
								label={t.projectSubject}
								value={subject ?? ''}
								onChange={async (e) => {
									const value = e.target.value;
									const newSubject = value ? (value as ProjectKind) : null;
									setSubject(newSubject);
									setProjectPageNumber(0);
									await runProjectSearch({ subject: newSubject, pageNumber: 0 });
								}}
								disabled={isSearchingProjects}
							>
								<option value="">{t.any}</option>
								{ProjectKindValues.map((p) => (
									<option key={p} value={p}>
										{p}
									</option>
								))}
							</SelectField>

							<SelectField
								id="sort-method"
								label={t.sortBy}
								value={sortMethod}
								onChange={async (e) => {
									const newMode = e.target.value as SortMethod;
									setSortMethod(newMode);
									setProjectPageNumber(0);
									await runProjectSearch({ sortBy: newMode, pageNumber: 0 });
								}}
								disabled={isSearchingProjects}
							>
								{SortMethodValues.map((p) => (
									<option key={p[0]} value={p[1]}>
										{p[0]}
									</option>
								))}
							</SelectField>
						</div>

						<div className="flex flex-wrap items-center gap-3">
							<label className="flex items-center gap-2 text-sm font-medium text-gray-700">
								<input
									type="checkbox"
									checked={descendingMode}
									onChange={async (e) => {
										setDescendingMode(e.target.checked);
										setProjectPageNumber(0);
										await runProjectSearch({
											descending: e.target.checked,
											pageNumber: 0,
										});
									}}
									disabled={isSearchingProjects}
								/>
								{t.descending}
							</label>
							{isSearchingProjects && (
								<Loader2
									className="h-4 w-4 animate-spin text-teal-600"
									aria-label="Searching projects"
								/>
							)}
						</div>

						<div className="flex flex-wrap items-end gap-2">
							<div className="w-full max-w-[14rem] sm:w-52">
								<FormLabel htmlFor="tag-input">Tags</FormLabel>
								<Input
									id="tag-input"
									type="text"
									placeholder={t.addTagPlaceholder}
									value={tagInput}
									onChange={(e) => setTagInput(e.target.value)}
									onKeyDown={async (e) => {
										if (e.key !== 'Enter') return;
										e.preventDefault();
										await addTag(tagInput);
									}}
									disabled={isSearchingProjects}
								/>
							</div>
							<Button
								type="button"
								variant="secondary"
								className="mb-0.5"
								disabled={isSearchingProjects}
								onClick={() => addTag(tagInput)}
							>
								{t.add}
							</Button>
							{tags.length > 0 && (
								<div className="flex w-full flex-wrap gap-1.5">
									{tags.map((tag) => (
										<Badge key={tag} variant="tag">
											{tag}
											<button
												type="button"
												aria-label={`Remove ${tag} tag`}
												onClick={async () => {
													const newTags = tags.filter((item) => item !== tag);
													setTags(newTags);
													setProjectPageNumber(0);
													await runProjectSearch({ tags: newTags, pageNumber: 0 });
												}}
												className="ml-1 hover:text-red-600"
												disabled={isSearchingProjects}
											>
												<X className="h-3 w-3" />
											</button>
										</Badge>
									))}
								</div>
							)}
						</div>
					</div>
				)}
			</SectionCard>

			<div className="space-y-5">
				{showUsers && userError && (
					<AlertBanner kind="error">{userError}</AlertBanner>
				)}

				{showUsers && loadingUsers && (
					<SectionCard contentClassName="p-4 text-center text-sm text-gray-600">
						{isUserSearch ? 'Searching students…' : 'Loading students…'}
					</SectionCard>
				)}

				{showUsers && !loadingUsers && users.length > 0 && (
					<SectionCard
						icon={<UserIcon className="h-5 w-5 text-teal-600" />}
						title={isUserSearch ? 'Student results' : t.students}
						description={
							!isUserSearch
								? 'Open a profile or type at least 2 characters to filter.'
								: undefined
						}
						contentClassName="p-0"
					>
						<div className="divide-y divide-border/60">
							{users.map((user) => (
								<button
									key={user.login}
									type="button"
									onClick={() => router.push(`/profile/${user.login}`)}
									className="flex w-full items-center gap-3 px-4 py-3 text-left transition-colors hover:bg-darker-background/60"
								>
									<Image
										src={user.image || '/globe.svg'}
										alt={user.login}
										width={36}
										height={36}
										className="h-9 w-9 shrink-0 rounded-full bg-gray-100 object-cover"
									/>
									<div className="min-w-0">
										<p className="truncate font-medium text-gray-900">
											{displayName(user)}
										</p>
										<p className="truncate text-sm text-gray-600">
											@{user.login}
											{(user.campus || user.location) && (
												<span> · {user.campus || user.location}</span>
											)}
										</p>
									</div>
								</button>
							))}
						</div>
						<div className="px-4 pb-4">
							<PaginationBar
								pageNumber={userPageNumber}
								pageCount={userPageCount}
								totalCount={userTotalCount}
								pageSize={SEARCH_PAGE_SIZE}
								disabled={loadingUsers}
								onPageChange={setUserPageNumber}
							/>
						</div>
					</SectionCard>
				)}

				{showEvents && eventError && (
					<AlertBanner kind="error">{eventError}</AlertBanner>
				)}

				{showEvents && loadingEvents && (
					<SectionCard contentClassName="p-4 text-center text-sm text-gray-600">
						Loading events…
					</SectionCard>
				)}

				{showEvents && !loadingEvents && events.length > 0 && (
					<SectionCard
						icon={<Calendar className="h-5 w-5 text-teal-600" />}
						title={t.events}
						contentClassName="p-0"
					>
						<div className="divide-y divide-border/60">
							{events.map((event) => (
								<button
									key={event.id}
									type="button"
									onClick={() => router.push('/events')}
									className="flex w-full gap-3 px-4 py-3 text-left transition-colors hover:bg-darker-background/60"
								>
									{event.coverImage ? (
										// eslint-disable-next-line @next/next/no-img-element
										<img
											src={event.coverImage}
											alt=""
											className="h-16 w-24 shrink-0 rounded-lg object-cover"
										/>
									) : (
										<div className="h-16 w-24 shrink-0 rounded-lg bg-gradient-to-br from-teal-600 to-teal-500" />
									)}
									<div className="min-w-0 flex-1">
										<p className="font-medium text-gray-900">{event.title}</p>
										<p className="line-clamp-2 text-sm text-gray-600">
											{event.description}
										</p>
										<p className="text-xs text-gray-500">
											{event.date} · {event.time} · {event.location}
										</p>
									</div>
								</button>
							))}
						</div>
						<div className="px-4 pb-4">
							<PaginationBar
								pageNumber={eventPageNumber}
								pageCount={eventPageCount}
								totalCount={eventTotalCount}
								pageSize={SEARCH_PAGE_SIZE}
								disabled={loadingEvents}
								onPageChange={setEventPageNumber}
							/>
						</div>
					</SectionCard>
				)}

				{showProjects && projectError && (
					<AlertBanner kind="error">{projectError}</AlertBanner>
				)}

				{showProjects && projectCards.length > 0 && (
					<SectionCard
						icon={<Hash className="h-5 w-5 text-teal-600" />}
						title={t.projects}
						contentClassName="space-y-4 p-4 sm:p-5"
					>
						<div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
							{projectCards.map((card) => (
								<ProjectSearchCard
									key={card.id}
									card={card}
									onClick={() => router.push(`/project/${card.id}`)}
								/>
							))}
						</div>

						<PaginationBar
							pageNumber={projectPageNumber}
							pageCount={projectPageCount}
							totalCount={projectTotalCount}
							pageSize={SEARCH_PAGE_SIZE}
							disabled={isSearchingProjects}
							onPageChange={(nextPage) =>
								runProjectSearch({ pageNumber: nextPage })
							}
						/>
					</SectionCard>
				)}

				{showUsers && !loadingUsers && users.length === 0 && !userError && (
					<EmptyState
						icon={<UserIcon className="h-12 w-12" />}
						message={
							isUserSearch
								? `${t.noResult}.`
								: 'No students in the directory yet.'
						}
					/>
				)}

				{showEvents && !loadingEvents && events.length === 0 && !eventError && (
					<EmptyState
						icon={<Calendar className="h-12 w-12" />}
						message={
							isUserSearch
								? `${t.noEvents ?? t.noResult}.`
								: `${t.noEvents ?? 'No events scheduled yet.'}`
						}
					/>
				)}

				{showProjects &&
					projectCards.length === 0 &&
					!projectError &&
					!isSearchingProjects && (
						<EmptyState
							icon={<SearchIcon className="h-12 w-12" />}
							message={`${t.noResult}.`}
						/>
					)}
			</div>
		</PageShell>
	);
}
