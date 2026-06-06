# Project backend documentation

Make sure to run `npx prisma generate` to generate the database client. Also ensure that the database schema is up-to-date.

## API

Here is a list of methods, as well as the parameters they take:

## `GET /projects?id=project_id`

Where `id` parameter in a query is the id of the project.

Returns a single JSON object with either the project data, a single `error` field in case of an internal error, or a proper NestJS error.

Here is an example of a JSON object in case of a query and its successful response:

`http://127.0.0.1:3000/projects?id=2`

```json
{
  "id": 2,
  "title": "Cool stuff",
  "subject": "cub3d",
  "description": "The description of the project.\n",
  "screenshots": [],
  "source_link": "",
  "tags": [
    "awesome",
    "great"
  ]
}
```

Here is the description of each field in this response:

* `id` - the id of the project;
* `title` - the title of the project;
* `subject` - the subject of the project. This type is a string enum, generated from the prisma schema;
* `description` - full description of the project;
* `screenshots` - a JSON object that represents a list of unique URLs pointing towards the screenshot images;
* `source_link` - a URL that points towards the project source code archive;
* `tags` - a JSON object that represents a list of unique tag strings.

In case of an internal problem when trying to look up the project by id, the following error may be returned:

```json
{
  "error": "Project with id {project_id} was not found"
}
```

, where `{project_id}` is the requested id of the project.

In any other case, default NestJS error may be returned. For example, the following object may be returned if the validation for the project id parameter fails:

```json
{
  "message": "Validation failed (numeric string is expected)",
  "error": "Bad Request",
  "statusCode": 400
}
```

This API is not finalized. Additional fields may be added in the case of a successful response, and error reporting system will get revamped.

## `POST /projects/search`

Performs project search in the database with specified parameters in the request body block. Search parameters take the form of the following DTO:

```typescript
// Taken from dto.ts
interface ProjectSearchDTO {
	title: string | null
	subject: Subject | null
	tags: Array<string> | null
}
```

All parameters may be null, but nevertheless must be present in the query:

```typescript
const validQuery = {
	title: null,
	subject: null,
	tags: null
}

const invalidQuery = {
	// Missing `title` field, for example
	subject: null,
	tags: null
}
```

This endpoint returns a list of small objects, each individually describing a certain project.

```typescript
// From responses.ts
class ProjectCardData {
	id: number;
	title: string;
	shortDescription: string;
	userId: number; // Currently is always 0
	subject: Subject;

	//...
}
```

Thus the response has a type of `Array<ProjectCardData>`. Here is an example query:

```json
{
	"title": null,
	"subject": "so_long",
	"tags": null,
}
```

And here is the response:

```json
[
  {
    "id": 3,
    "title": "Neat thing",
    "shortDescription": "A neat project, ...",
    "userId": 0,
    "subject": "so_long"
  },
  {
    "id": 42,
    "title": "My awesome project",
    "shortDescription": "This is my aweso...",
    "userId": 0,
    "subject": "so_long"
  }
]
```

In case of malformed queries, a default NestJS error may be returned.

This endpoin API is not finalized. Additional query and response fields will be added in the future, particularly for ordering projects by the upload/evaluation dates.

## `POST /projects`

TODO. (Will be used for both project uploads and updates.)

## `DELETE /projects`

TODO.