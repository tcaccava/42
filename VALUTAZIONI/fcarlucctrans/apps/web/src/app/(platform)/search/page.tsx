import { SearchClient } from '@/components/SearchClient';
import { searchProjects } from '@/server/project';
import { SEARCH_PAGE_SIZE, SortMethod } from '@/types/search';

export default async function SearchPage() {
  const [foundProjects, pageCount, totalCount, projectError] = await searchProjects({
    title: '',
    subject: null,
    tags: [],
    sortBy: SortMethod.Date,
    descending: false,
    pageNumber: 0,
    pageSize: SEARCH_PAGE_SIZE,
  });

  return (
    <SearchClient
      initialCards={foundProjects}
      initialPageCount={pageCount}
      initialTotalCount={totalCount}
      initialProjectError={projectError}
    />
  );
}
